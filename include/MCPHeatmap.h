#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"
#include "CapacityHandler.h"

namespace GUI::MCP
{
	const extern REX::W32::ID3D11Device* BSRendererDevice;

	// Disclaimer: Pretty much all of the DX11 and rendering stuff in this 'Heatmap' class is vibe coded. Or at least, AI did all of the heavy lifting and then I just made the code work with my mod. Not a fan of doing it that way, but the rendering stuff mostly just flew straight over my head, so I took the easy way out :(
	class Heatmap
	{
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_ctx = nullptr;
		ID3D11Texture2D* m_texture = nullptr;
		ID3D11ShaderResourceView* m_srv = nullptr;

		std::vector<uint8_t> m_pixels;

		int m_width;
		int m_height;

		public:
			Heatmap() = default;
			~Heatmap() = default;

			int m_padWidth;
			int m_padHeight;

			void Init();

			void Update(const std::vector<float>& data);

			ImGuiMCP::ImTextureID GetTextureID() const;

			void Release();

			void CreateTexture();
			void GeneratePixels(const std::vector<float>& data);
			void Upload();
	};

	extern Heatmap heatmap;

	struct HeatmapCursorData
	{
		HeatmapCursorData() = default;
		HeatmapCursorData(int x, int y, float val): stamina(x), level(y), weight(val) {};

		int stamina;
		int level;
		float weight;
	};

	void HeatmapWidget(int plot_width, int plot_height);
	void HeatmapUpdateButton();
	void HeatmapGradientSelector();
	void HeatmapPlot(int a_width, int a_height);
	HeatmapCursorData GetHeatmapCursorVal(SKSEMenuFramework::ImVec2 a_pCursor, SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1);
}