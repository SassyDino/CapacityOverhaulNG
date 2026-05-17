#include "MCPHeatmap.h"
#include "MCPColour.h"
#include "MCPStyle.h"
#include "MCPHelpers.h"
#include "MCPSettings.h"
#include "MCP.h"
#include "Calc.h"
#include "Player.h"

using namespace ImGuiMCP;
namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace GUI::MCP
{	
	Heatmap heatmap;
	
	void Heatmap::Init()
	{
		m_device = reinterpret_cast<ID3D11Device*>(RE::BSGraphics::Renderer::GetSingleton()->GetDevice());
		m_device->GetImmediateContext(&m_ctx);
	}

	void Heatmap::Update()
	{
		logger::trace("Creating heatmap image with new data...");

		//TODO: Probably need to handle these errors if they happen?
		if (!m_device || !m_ctx || m_data.empty()) { return; }

		if (!m_texture || m_maxStamina != m_width || m_maxLevel != m_height) { CreateTexture(); }

		GeneratePixels(m_data);

		Upload();

		logger::trace("Heatmap image created!");
	}

	ImTextureID Heatmap::GetTextureID() const
	{
		return static_cast<ImTextureID>(m_srv);
	}

	void Heatmap::Release()
	{
		if (m_srv) {
			m_srv->Release();
			m_srv = nullptr;
		}

		if (m_texture) {
			m_texture->Release();
			m_texture = nullptr;
		}	
	}

	void Heatmap::CreateTexture()
	{
		Release();

		m_width = m_maxStamina;
		m_height = m_maxLevel;

		m_padWidth = m_width + 2;
		m_padHeight = m_height + 2;

		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = m_padWidth;
		texDesc.Height = m_padHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.Usage = D3D11_USAGE_DYNAMIC;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = m_device->CreateTexture2D(&texDesc, nullptr, &m_texture);
		if (FAILED(hr)) { logger::error("Failed to create texture."); return; }

		hr = m_device->CreateShaderResourceView(m_texture, nullptr, &m_srv);
		if (FAILED(hr)) { logger::error("Failed to create shader resource view."); return; }
	}

	void Heatmap::GeneratePixels(const std::vector<float>& data)
	{
		logger::trace("Generating pixels...");
		clib_util::Timer timer;
		timer.start();

		m_padWidth = m_width + 2;
		m_padHeight = m_height + 2;
		m_pixels.resize(m_padWidth * m_padHeight * 4);

		m_lowestVal = *std::min_element(data.begin(), data.end());
		m_highestVal = *std::max_element(data.begin(), data.end());

		if (Selections::heatmapConstrainGradient && (m_highestVal > Selections::heatmapConstraintVal)) {
			m_highestVal = Selections::heatmapConstraintVal;
		}

		auto SetPixel = [&](int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			int idx = ((y * m_padWidth) + x) * 4;
			m_pixels[idx] = r;
			m_pixels[idx + 1] = g;
			m_pixels[idx + 2] = b;
			m_pixels[idx + 3] = a;
		};

		for (int y = 0; y < m_height; y++) {
			for (int x = 0; x < m_width; x++) {
				float dataVal = data[(y * m_width) + x];

				if (Selections::heatmapConstrainGradient && (dataVal > Selections::heatmapConstraintVal)) {
					dataVal = Selections::heatmapConstraintVal;
				}

				float val = (dataVal - m_lowestVal) / (m_highestVal - m_lowestVal);

				ImU32 hexCol = Colour::Gradient::GetLUTVal(val);
				uint8_t r, g, b, a;
				Colour::Gradient::HexToRGBA(hexCol, r, g, b, a);

				SetPixel(x+1, y+1, r, g, b, a);
			}
		}

		for (int x = 0; x < m_width; x++) {
			memcpy(
				&m_pixels[((0 * m_padWidth) + (x + 1)) * 4],
				&m_pixels[((1 * m_padWidth) + (x + 1)) * 4],
				4
			);

			memcpy(
				&m_pixels[(((m_padHeight - 1) * m_padWidth) + (x + 1)) * 4],
				&m_pixels[(((m_padHeight - 2) * m_padWidth) + (x + 1)) * 4],
				4
			);
		}

		for (int y = 0; y < m_padHeight; y++) {
			memcpy(
				&m_pixels[((y * m_padWidth) + 0) * 4],
				&m_pixels[((y * m_padWidth) + 1) * 4],
				4
			);

			memcpy(
				&m_pixels[((y * m_padWidth) + (m_padWidth - 1)) * 4],
				&m_pixels[((y * m_padWidth) + (m_padWidth - 2)) * 4],
				4
			);
		}

		timer.stop();
		logger::trace("Pixels generated! Time taken: {}μs / {}ms", timer.duration_μs(), timer.duration_ms());
	}

	void Heatmap::Upload()
	{
		logger::trace("Uploading texture to memory...");

		D3D11_MAPPED_SUBRESOURCE mapped;

		if (FAILED(m_ctx->Map(m_texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped))) { logger::error("Failed to upload texture to GPU."); return; }

		for (int y = 0; y < m_padHeight; y++) {
			memcpy(static_cast<uint8_t*>(mapped.pData) + y * mapped.RowPitch,
				m_pixels.data() + y * m_padWidth * 4,
				m_padWidth * 4
			);
		}

		m_ctx->Unmap(m_texture, 0);

		logger::trace("Texture uploaded!");
	}

	float Heatmap::GetDataAtIndex(int a_index)
	{
		return m_data.at(a_index);
	}

	bool Heatmap::HasData()
	{
		return !heatmap.m_data.empty();
	}

	void HeatmapWidget(int plot_width, int plot_height)
	{
		if (!heatmap.HasData()) {
			logger::debug("No heatmap data found ---> Generating...");
			heatmap.ComputeData();
			heatmap.Update();
			logger::debug("Heatmap successfully generated!");
		}

		HeatmapPlot(plot_width, plot_height);

		HeatmapUpdateButton();

		CustomSliderInt("$MCP.Widgets.Heatmap.MaxStamina", &heatmap.m_maxStamina, 150, 400);
		CustomSliderInt("$MCP.Widgets.Heatmap.MaxLevel", &heatmap.m_maxLevel, 50, 200);

		HeatmapGradientSelector();
		CustomCheckbox("$MCP.Widgets.Heatmap.AdjustScaling", &Selections::heatmapConstrainGradient);
		MCP_API::SliderFloat("$MCP.Widgets.Heatmap.ScalingLimit"_tr, &Selections::heatmapConstraintVal, 100, 3000, "%f");

		//TODO: Add button to allow toggling of applying a maximum value for heatmap colour scaling
	}

	void HeatmapUpdateButton()
	{
		if (MCP_API::Button("$MCP.Widgets.Heatmap.Update"_tr)) {
			logger::debug("New heatmap requested...");
			heatmap.ComputeData();
			heatmap.Update();
			logger::debug("Heatmap update completed!");
		}
	}
	
	void HeatmapGradientSelector()
	{
		const char* previewGradient = Selections::heatmapGradient->GetName();

		MCP_API::SetNextItemWidth(300.0f);

		if (MCP_API::BeginCombo("$MCP.Widgets.Heatmap.Gradient"_tr, previewGradient)) {
			for (int n = 0; n < Colour::Gradient::availableSchemes.size(); n++) {
				const bool is_selected = (Selections::heatmapGradient == Colour::Gradient::availableSchemes[n]);

				if (MCP_API::Selectable(Colour::Gradient::availableSchemes[n]->GetName(), is_selected)) {
					Selections::heatmapGradient = Colour::Gradient::availableSchemes[n];

					Colour::Gradient::RequestNewLUT(Colour::Gradient::availableSchemes[n]);
				}

				if (is_selected) { MCP_API::SetItemDefaultFocus(); }
			}

			MCP_API::EndCombo();
		}
	}
	
	void HeatmapPlot(int a_width, int a_height)
	{
		ImVec2 p0in;
		MCP_API::GetCursorScreenPos(&p0in);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();

		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);
		float lineThick = 5.0f;
		float topBuffer = 50.0f;
		float tick = 20.0f;

		ImVec2 pEnd = {p0in.x, p0in.y+a_height+topBuffer+40};

		p0in = {p0in.x+80, p0in.y+topBuffer};
		ImVec2 p1in = {p0in.x + a_width, p0in.y + a_height};
		ImVec2 p0out = {p0in.x-lineThick, p0in.y-lineThick}; // top L
		ImVec2 p1out = {p1in.x+lineThick, p1in.y+lineThick}; // bottom R
		ImVec2 p2out = {p0out.x, p1out.y}; // bottom L
		ImVec2 p3out = {p1out.x, p0out.y}; // top R

		float u0 = 1.0f / heatmap.m_padWidth;
		float v0 = 1.0f / heatmap.m_padHeight;
		float u1 = (heatmap.m_padWidth - 1.0f) / heatmap.m_padWidth;
		float v1 = (heatmap.m_padHeight - 1.0f) / heatmap.m_padHeight;

		MCPDraw::AddImage(drawList, heatmap.GetTextureID(), p0in, p1in, {u0,v1}, {u1,v0}, HEX_COL32(0xFFFFFFFF));

		MCPDraw::AddRectFilled(drawList, p0in, {p0out.x, p2out.y+tick}, borderCol, 0.0f, NULL); // left
		MCPDraw::AddRectFilled(drawList, {p3out.x, p0in.y}, {p1in.x, p1out.y+tick}, borderCol, 0.0f, NULL); // right
		MCPDraw::AddRectFilled(drawList, p3out, {p0out.x-tick, p0in.y}, borderCol, 0.0f, NULL); // top
		MCPDraw::AddRectFilled(drawList, p1in, {p2out.x-tick, p2out.y}, borderCol, 0.0f, NULL); // bottom

		CenteredText("0", ImVec2(p2out.x-15, p2out.y+15));
		CenteredText("$MCP.Widgets.Heatmap.Plot.X"_tr, ImVec2(p2out.x+(a_width*0.5f), p2out.y+15));
		CenteredText("$MCP.Widgets.Heatmap.Plot.Y"_tr, ImVec2(p0out.x-40, p0in.y+(a_height*0.5f)));

		MCP_API::SetCursorScreenPos({p1out.x+5, p1out.y});
		MCP_API::Text("%i", heatmap.m_maxStamina);

		MCP_API::SetCursorScreenPos({p0out.x-tick, p0out.y-35});
		MCP_API::Text("%i", heatmap.m_maxLevel);


		if (MCP_API::IsMouseHoveringRect(p0in, p1in)) {
			MCP_API::BeginTooltip();

			auto io = MCP_API::GetIO();
			auto cursorData = GetHeatmapCursorVal(io->MousePos, p0in, p1in);

			MCP_API::Text("%s: %.0f", "$MCP.Widgets.Heatmap.Plot.Tooltip"_tr, cursorData.weight);
			MCP_API::Text("%s (X): %i", "$MCP.Widgets.Heatmap.Plot.X"_tr, cursorData.stamina);
			MCP_API::Text("%s (Y): %i", "$MCP.Widgets.Heatmap.Plot.Y"_tr, cursorData.level);

			MCP_API::EndTooltip();
		}

		MCP_API::SetCursorScreenPos(pEnd);
	}

	HeatmapCursorData GetHeatmapCursorVal(ImVec2 a_pCursor, ImVec2 a_p0, ImVec2 a_p1)
	{
		HeatmapCursorData cursorData;

		float u = (a_pCursor.x - a_p0.x) / (a_p1.x - a_p0.x);
		float v = (a_pCursor.y - a_p0.y) / (a_p1.y - a_p0.y);

		u = std::clamp(u, 0.0f, 1.0f);
		v = std::clamp(v, 0.0f, 1.0f);

		cursorData.stamina = static_cast<int>(u * heatmap.m_maxStamina);
		cursorData.level = static_cast<int>((1.0f - v) * heatmap.m_maxLevel);

		int index = (cursorData.level * heatmap.m_maxStamina) + cursorData.stamina;

		//logger::trace("Stamina (X) = {} | Level (Y) = {} | heatmapData[{}]", cursorData.stamina, cursorData.level, index);

		cursorData.stamina++, cursorData.level++;
		cursorData.weight = heatmap.GetDataAtIndex(index);
		return cursorData;
	}

	void Heatmap::ComputeData()
	{
		logger::trace("Computing heatmap data...");
		clib_util::Timer timer;
		timer.start();

		int d = 0;

		m_data.clear();

		for (int l = 1; l <= heatmap.m_maxLevel; l++) {
			for (int s = 1; s <= heatmap.m_maxStamina; s++) {
				m_data.push_back(static_cast<float>(Settings::Get<uint32_t>("uBaseCarryWeight")));

				if (Settings::Get<bool>("bStaminaAffectsWeight")) {
					PlayerState::UpdateStamAtMaxGrad();

					m_data.at(d) += (
						Calc::StaminaWeightBonus(
							s,
							Settings::Get<float>("fStaminaWeightRate"),
							Settings::Get<uint32_t>("uStaminaWeightPivot"),
							Settings::Get<uint32_t>("uBaseCarryWeight"),
							PlayerState::StamAtMaxGrad
						)
						* Settings::Get<float>("fStaminaWeightMod"));
				}

				if (Settings::Get<bool>("bLevelAffectsWeight")) {
					PlayerState::UpdateLevelAtMaxGrad();

					m_data.at(d) += (
						Calc::LevelWeightBonus(
							l,
							Settings::Get<float>("fLevelWeightRate"),
							Settings::Get<uint32_t>("uLevelWeightPivot"),
							Settings::Get<uint32_t>("uBaseCarryWeight"),
							PlayerState::LevelAtMaxGrad
						)
						* Settings::Get<float>("fLevelWeightMod"));
				}

				if (Settings::Get<bool>("bRaceAffectsWeight")) {
					m_data.at(d) *= PlayerState::raceWeightMod;
				}

				ceil(m_data.at(d));

				d++;
			}
		}

		timer.stop();
		logger::trace("Finished computing heatmap data ({} entries). Time taken: {}μs / {}ms", m_data.size(), timer.duration_μs(), timer.duration_ms());
	}
}