#include "GUICustom.h"
#include "MCP.h"
#include "CapacityHandler.h"
using namespace SKSEMenuFramework;
using namespace CapacityHandler;
namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace GUI
{
	std::string_view Assets::path = "Data/Textures/SassyDino/CapacityOverhaulNG/";

	void Assets::LoadTextures()
	{
		
	}
}

namespace GUI::MCP
{	
	const float borderThick = 1.0f;
	const float borderThin = 1.0f;

	const std::unordered_map<ItemCategories, const char *> categoryTooltips = {
		{ItemCategories::kHuge, "Huge Items"},
		{ItemCategories::kLarge, "Large Items"},
		{ItemCategories::kMedium, "Medium Items"},
		{ItemCategories::kSmall, "Small Items"},
		{ItemCategories::kTiny, "Tiny Items"},
		{ItemCategories::kAlchemy, "Alchemy Items (Overflow)"},
		{ItemCategories::kAmmo, "Ammunition (Overflow)"},
		{ItemCategories::kCoin, "Coins (Overflow)"},
		{ItemCategories::kWeaponLarge, "Large Weapons (Overflow)"},
		{ItemCategories::kWeaponMedium, "Medium Weapons (Overflow)"},
		{ItemCategories::kWeaponSmall, "Small Weapons (Overflow)"},
		{ItemCategories::kWeaponRanged, "Ranged Weapons (Overflow)"},
		{ItemCategories::kShield, "Shields (Overflow)"}
	};

	const std::unordered_map<ItemCategories, ImGuiMCP::ImU32> categoryColours = {
		{ItemCategories::kHuge, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.659f , 0.0f, 0.357f, 1.0f))},
		{ItemCategories::kLarge, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.729f , 0.259f, 0.184f, 1.0f))},
		{ItemCategories::kMedium, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.659f , 0.478f, 0.0f, 1.0f))},
		{ItemCategories::kSmall, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.48f, 0.65f, 0.18f, 1.0f))},
		{ItemCategories::kTiny, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.8f, 0.52f, 1.0f))},
		{ItemCategories::kAlchemy, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.75f, 0.31f, 0.69f, 1.0f))},
		{ItemCategories::kAmmo, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.65f, 0.31f, 0.84f, 1.0f))},
		{ItemCategories::kCoin, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.39f, 0.36f, 1.0f, 1.0f))},
		{ItemCategories::kWeaponLarge, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.16f, 0.18f, 0.34f, 1.0f))},
		{ItemCategories::kWeaponMedium, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.42f, 0.27f, 0.45f, 1.0f))},
		{ItemCategories::kWeaponSmall, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.68f, 0.35f, 0.49f, 1.0f))},
		{ItemCategories::kWeaponRanged, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.88f, 0.49f, 0.47f, 1.0f))},
		{ItemCategories::kShield, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.98f, 0.7f, 0.44f, 1.0f))}
	};

	void CustomSeparator(const char *text)
	{
		MCP_API::SeparatorText(text);
		MCP_API::SameLine();
	}

	void SnapFloatSlider(float *a_target, float snap)
	{
		if (MCP_API::IsItemDeactivatedAfterEdit()) {
			*a_target = round(*a_target * (1/snap)) / (1/snap);
		}
	}

	void FileManagerButtons()
	{
		if (MCP_API::Button("Save")) {
			logger::info("Saving current user settings.");
			Settings::SaveToFile(Settings::userPath);
			Settings::Init();
		}
		MCP_API::SameLine();

		if (MCP_API::Button("Load")) {
			logger::info("Loading saved user settings.");
			Settings::Init();
		}
		MCP_API::SameLine();

		if (MCP_API::Button("Reset to Default")) {
			logger::info("Loading default mod settings.");
			Settings::Load(Settings::defaultPath);
		}
	}

	SKSEMenuFramework::ImU32 PercentageColour(float a_count, float a_capacity)
	{
		float count = (float)a_count;
		float capacity = (float)a_capacity;

		// Colour default is blue, this will appear if any category is over capacity
		float colH = 0.6f;
		float colS = 1;
		float colV = 1;

		float colR = 0;
		float colG = 0;
		float colB = 0;

		if (capacity > 0) {
			float percent = 1- (count / capacity);
			if (percent <= 1.0f) { colH = percent / 3; }
		}

		MCP_API::ColorConvertHSVtoRGB(colH, colS, colV, &colR, &colG, &colB);

		return MCP_API::ColorConvertFloat4ToU32(ImVec4(colR , colG, colB, 1.0f));
	}

	void CapacityCategoryTooltip(ImVec2 a_p0, ImVec2 a_p1, const char* a_title, ItemCategories a_category)
	{
		std::string tooltipQuanStr;
		if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
			tooltipQuanStr = std::format("{}/{}", CapacityHandler::GetCountForGUI(a_category), CapacityHandler::GetCapacityForGUI(a_category));
		} else {
			tooltipQuanStr = std::format("-/{}", CapacityHandler::GetCapacityForGUI(a_category));
		}
		const char *tooltipQuantity = tooltipQuanStr.c_str();

		if (MCP_API::IsMouseHoveringRect(a_p0, a_p1)) {
			MCP_API::BeginTooltip();
			MCP_API::Text(a_title);
			MCP_API::Text(tooltipQuantity);
			MCP_API::EndTooltip();
		}
	}

	void DrawHatchFill(ImDrawList *drawList, ImVec2 p0, ImVec2 p1)
	{
		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);
		float w = p1.x - p0.x;
		float h = p1.y - p0.y;

		float spacing = 15.0f;

		for (float offset = -h; offset < w; offset += spacing)
		{
			float x_start = p0.x + std::max(0.0f, offset);
			float y_start = p1.y + std::max(0.0f, -offset);
			
			if ((p0.x + offset) > p0.x) {
				x_start = p0.x + offset;
				y_start = p1.y;
			} else {
				x_start = p0.x;
				y_start = p1.y + offset;
			}

			float x_end = x_start + h;
			float y_end = y_start - h;

			if (x_end > p1.x) {
				y_end += (x_end - p1.x);
				x_end = p1.x;
			}
			if (y_end < p0.y) {
				x_end -= (p0.y - y_end);
				y_end = p0.y;
			}

			MCPDraw::AddLine(drawList, ImVec2(x_start, y_start), ImVec2(x_end, y_end), borderCol, borderThick);
		}
	}

	void CapacityVisualiser()
	{
		CapacityHandler::Base::UpdateBaseCapacities();
		if (!Settings::Get<bool>("bCapacityVisualiserBaseValues")) {
			CapacityHandler::Player::CalculateActualCapacities();
		}
		if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
			CapacityHandler::Player::UpdateAllCategories();
		}
		
		float windowWidth = MCP_API::GetWindowWidth();
		float pEnd;

		MCP_API::BeginGroup();
		{
			MCP_API::Checkbox("Show Base Capacity Values (Unaltered by player stats)", Settings::Get<bool*>("bCapacityVisualiserBaseValues"));
			MCP_API::SameLine();
			MCP_API::Checkbox("Show Fill Status of Categories", Settings::Get<bool*>("bCapacityVisualiserShowFilled"));

			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				MCP_API::Text("Total Capacity Summary");
				CapacityVisualiserTotal();
				MCP_API::Dummy(ImVec2(windowWidth, 45));
			}

			MCP_API::BeginGroup();
			{
				MCP_API::Text("Main/Standard Categories");
				CapacityVisualiserMain();
				MCP_API::Dummy(ImVec2(windowWidth*0.65f, 205));
				MCP_API::Text("Weapon Categories");
				pEnd = CapacityVisualiserWeapons();
				MCP_API::Dummy(ImVec2(windowWidth*0.65f, 200));
				MCP_API::EndGroup();
			}
			MCP_API::SameLine();

			MCP_API::BeginGroup();
			{
				MCP_API::Text("Misc Categories");
				CapacityVisualiserMisc(pEnd);
				MCP_API::EndGroup();
			}
		}
		MCP_API::NewLine();
	}

	void CapacityVisualiserWindow(bool *open_state)
	{
		if (*open_state) {
			MCP_API::Begin("Capacity Visualiser", open_state, 0);
			CapacityVisualiser();
			MCP_API::End();
		}
	}

	void CapacityVisualiserMain()
	{
		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);

		// Determine row/column dimensions depending on what data is being shown
		int rowCount = (Settings::Get<bool>("bHugeCapacityShared")) ? 5 : 4;
		float mainRowGap = 40.0f;
		ImVec2 mainSize = ImVec2(MCP_API::GetWindowWidth() * 0.65f, mainRowGap*rowCount);
		float hugeDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kHuge);
		float largeDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kLarge);
		float mediumDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kMedium);
		float smallDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kSmall);
		float tinyDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kTiny);
		ImU32 fillColour;

		std::vector<float> dividerVec = {hugeDivGap, largeDivGap, mediumDivGap, smallDivGap, tinyDivGap};
		std::vector<ItemCategories> categoryVec = {
			ItemCategories::kHuge,
			ItemCategories::kLarge,
			ItemCategories::kMedium,
			ItemCategories::kSmall,
			ItemCategories::kTiny
		};

		// Just some misc iterator values
		float itRow1 = 0;
		float itRow2 = 1;
		int itDiv = 0;
		int i = 1;

		// Draw independent "huge" category bar, if settings are set as so
		if (!Settings::Get<bool>("bHugeCapacityShared")) {
			fillColour = PercentageColour(
				CapacityHandler::GetCountForGUI(ItemCategories::kHuge),
				CapacityHandler::GetCapacityForGUI(ItemCategories::kHuge)
			);

			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				if (CapacityHandler::GetCountForGUI(ItemCategories::kHuge) < CapacityHandler::GetCapacityForGUI(ItemCategories::kHuge)) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y), 
						ImVec2(p0.x+(hugeDivGap*CapacityHandler::GetCountForGUI(ItemCategories::kHuge)), p0.y+mainRowGap), 
						fillColour, 0.0f, 0
					);
				} else {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y), 
						ImVec2(p0.x+mainSize.x, p0.y+mainRowGap), 
						fillColour, 0.0f, 0
					);
				}
			}
			
			CapacityCategoryTooltip(p0, ImVec2(p0.x+mainSize.x, p0.y+mainRowGap), categoryTooltips.at(ItemCategories::kHuge), ItemCategories::kHuge);

			MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+mainSize.x, p0.y+mainRowGap), borderCol, 0.0f, 0, borderThick);

			while (i < CapacityHandler::GetCapacityForGUI(ItemCategories::kHuge)) {
				MCPDraw::AddLine(drawList, ImVec2(p0.x+(hugeDivGap*i), p0.y), ImVec2(p0.x+(hugeDivGap*i), p0.y+mainRowGap), borderCol, borderThin);
				i++;
			}

			p0.y += (mainRowGap + 5.0f); // Add height of bar, plus 5 for padding - need to check that this number is ok
		}

		itRow1 = 0;
		itRow2 = 1;
		itDiv = 0;

		// Draw the filled & coloured progress/fill bars for each main category
		for (auto category: categoryVec) {
			// Skip drawing kHuge progress bar if the setting is disabled
			if (!Settings::Get<bool>("bHugeCapacityShared") && (category == ItemCategories::kHuge)) {
				itDiv++;
				continue;
			}

			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				// Determine colour (on a green-yellow-red scale) based on capacity filled per category
				fillColour = PercentageColour(CapacityHandler::GetCountForGUI(category), CapacityHandler::GetCapacityForGUI(category));

				// Draw progress bar
				if (CapacityHandler::GetCountForGUI(category) < CapacityHandler::GetCapacityForGUI(category)) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), 
						ImVec2(p0.x+(dividerVec[itDiv]*(CapacityHandler::GetCountForGUI(category))), p0.y+(mainRowGap*itRow2)), 
						fillColour, 0.0f, 0
					);
				} else {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), 
						ImVec2(p0.x+mainSize.x, p0.y+(mainRowGap*itRow2)), 
						fillColour, 0.0f, 0
					);
				}
				
			}

			CapacityCategoryTooltip(ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), ImVec2(p0.x+mainSize.x, p0.y+(mainRowGap*itRow2)), categoryTooltips.at(category), category);

			itRow1++;
			itRow2++;
			itDiv++;
		}

		// Draw visualiser box/outer borders
		MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+mainSize.x, p0.y+mainSize.y), borderCol, 0.0f, 0, borderThick);

		// Draw horizontal dividers for each main storage category
		i = 1;
		while (i < rowCount) {
			MCPDraw::AddLine(drawList, ImVec2(p0.x, p0.y+(mainRowGap*i)), ImVec2(p0.x+mainSize.x-1, p0.y+(mainRowGap*i)), borderCol, borderThick);
			i++;
		}

		itRow1 = 0;
		itRow2 = 1;
		itDiv = 0;

		// Draw vertical dividers for each main storage category
		for (auto category: categoryVec) {
			// Skip drawing kHuge row/dividers if the setting is disabled
			if (!Settings::Get<bool>("bHugeCapacityShared") && (category == ItemCategories::kHuge)) {
				itDiv++;
				continue;
			}

			// Draw vertical dividers
			i = 1;
			while (i < CapacityHandler::GetCapacityForGUI(category)) {
				MCPDraw::AddLine(drawList, ImVec2(p0.x+(dividerVec[itDiv]*i), p0.y+(mainRowGap*itRow1)), ImVec2(p0.x+(dividerVec[itDiv]*i), p0.y+(mainRowGap*itRow2)), borderCol, borderThin);
				i++;
			}

			itRow1++;
			itRow2++;
			itDiv++;
		}
	}

	void CapacityVisualiserTotal()
	{
		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);

		ImVec2 boxSize = ImVec2(MCP_API::GetWindowWidth()-35.0f, 40.0f);

		std::unordered_map<ItemCategories, ImU32> categoryColours = {
			{ItemCategories::kHuge, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.659f , 0.0f, 0.357f, 1.0f))},
			{ItemCategories::kLarge, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.729f , 0.259f, 0.184f, 1.0f))},
			{ItemCategories::kMedium, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.659f , 0.478f, 0.0f, 1.0f))},
			{ItemCategories::kSmall, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.48f, 0.65f, 0.18f, 1.0f))},
			{ItemCategories::kTiny, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.8f, 0.52f, 1.0f))},
			{ItemCategories::kAlchemy, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.75f, 0.31f, 0.69f, 1.0f))},
			{ItemCategories::kAmmo, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.65f, 0.31f, 0.84f, 1.0f))},
			{ItemCategories::kCoin, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.39f, 0.36f, 1.0f, 1.0f))},
			{ItemCategories::kWeaponLarge, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.16f, 0.18f, 0.34f, 1.0f))},
			{ItemCategories::kWeaponMedium, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.42f, 0.27f, 0.45f, 1.0f))},
			{ItemCategories::kWeaponSmall, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.68f, 0.35f, 0.49f, 1.0f))},
			{ItemCategories::kWeaponRanged, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.88f, 0.49f, 0.47f, 1.0f))},
			{ItemCategories::kShield, MCP_API::ColorConvertFloat4ToU32(ImVec4(0.98f, 0.7f, 0.44f, 1.0f))}
		};

		//TODO: I absolutely hate all of this, I really need to find some better way of doing it bruh
		//NOTE: Also, even aside from the stupidly dense blocks of code that look terrible, there's no way that this is gonna run anywhere near to optimal when going multiple times a second.
		//? Could possibly make the xxxxPercent variables static, and then check for changes to any of the capacities/counts, and only recalculate each percentage as and when needed.
		float hugePercent = CapacityHandler::GetCountForGUI(ItemCategories::kHuge) / CapacityHandler::GetCapacityForGUI(ItemCategories::kHuge);
		float largePercent = CapacityHandler::GetCountForGUI(ItemCategories::kLarge) / CapacityHandler::GetCapacityForGUI(ItemCategories::kLarge);
		float mediumPercent = CapacityHandler::GetCountForGUI(ItemCategories::kMedium) / CapacityHandler::GetCapacityForGUI(ItemCategories::kMedium);
		float smallPercent = CapacityHandler::GetCountForGUI(ItemCategories::kSmall) / CapacityHandler::GetCapacityForGUI(ItemCategories::kSmall);
		float tinyPercent = CapacityHandler::GetCountForGUI(ItemCategories::kTiny) / CapacityHandler::GetCapacityForGUI(ItemCategories::kTiny);

		float alchemyPercent = (
			CapacityHandler::GetCountForGUI(ItemCategories::kAlchemy) - CapacityHandler::GetCapacityForGUI(ItemCategories::kAlchemy))
			/ CapacityHandler::GetCapacityForGUI(ItemCategories::kTiny);
		float ammoPercent = (
			CapacityHandler::GetCountForGUI(ItemCategories::kAmmo) - CapacityHandler::GetCapacityForGUI(ItemCategories::kAmmo))
			/ CapacityHandler::GetCapacityForGUI(ItemCategories::kTiny);
		float coinPercent = (
			(CapacityHandler::GetCountForGUI(ItemCategories::kCoin) - CapacityHandler::GetCapacityForGUI(ItemCategories::kCoin))
			/ Settings::Get<uint32_t>("uCoinsPerTiny")) / CapacityHandler::GetCapacityForGUI(ItemCategories::kTiny);
		
		float lWeapPercent = (
			CapacityHandler::GetCountForGUI(ItemCategories::kWeaponLarge) 
			- CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponLarge))
			/ CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponLarge);
		float mWeapPercent = (
			CapacityHandler::GetCountForGUI(ItemCategories::kWeaponMedium) 
			- CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponMedium))
			/ CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponMedium);
		float sWeapPercent = (
			CapacityHandler::GetCountForGUI(ItemCategories::kWeaponSmall) 
			- CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponSmall))
			/ CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponSmall);
		float rWeapPercent = (
			CapacityHandler::GetCountForGUI(ItemCategories::kWeaponRanged) 
			- CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponRanged))
			/ CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponRanged);
		float shieldPercent = (
			CapacityHandler::GetCountForGUI(ItemCategories::kShield) 
			- CapacityHandler::GetCapacityForGUI(ItemCategories::kShield))
			/ CapacityHandler::GetCapacityForGUI(ItemCategories::kShield);
		
		std::unordered_map<ItemCategories, float> categoryPercent = {
			{ItemCategories::kHuge, hugePercent},
			{ItemCategories::kLarge, largePercent},
			{ItemCategories::kMedium, mediumPercent},
			{ItemCategories::kSmall, smallPercent},
			{ItemCategories::kTiny, tinyPercent},
			{ItemCategories::kAlchemy, alchemyPercent},
			{ItemCategories::kAmmo, ammoPercent},
			{ItemCategories::kCoin, coinPercent},
			{ItemCategories::kWeaponLarge, lWeapPercent},
			{ItemCategories::kWeaponMedium, mWeapPercent},
			{ItemCategories::kWeaponSmall, sWeapPercent},
			{ItemCategories::kWeaponRanged, rWeapPercent},
			{ItemCategories::kShield, shieldPercent}
		};
		
		std::vector<ItemCategories> drawCategories;

		if (Settings::Get<bool>("bHugeCapacityShared")) { drawCategories.push_back(ItemCategories::kHuge); }
		drawCategories.push_back(ItemCategories::kLarge);
		drawCategories.push_back(ItemCategories::kMedium);
		drawCategories.push_back(ItemCategories::kSmall);
		drawCategories.push_back(ItemCategories::kTiny);
		drawCategories.push_back(ItemCategories::kAlchemy);
		drawCategories.push_back(ItemCategories::kAmmo);
		drawCategories.push_back(ItemCategories::kCoin);
		if (Settings::Get<bool>("bSeparateWeaponCategories")) {
			drawCategories.push_back(ItemCategories::kWeaponLarge);
			drawCategories.push_back(ItemCategories::kWeaponMedium);
			drawCategories.push_back(ItemCategories::kWeaponSmall);
			drawCategories.push_back(ItemCategories::kWeaponRanged);
			drawCategories.push_back(ItemCategories::kShield);
		}

		float percentTotal = 0;
		for (auto category : drawCategories) {
			if (categoryPercent.at(category) < 0) { categoryPercent.at(category) = 0; }
			percentTotal += categoryPercent.at(category);
		}
		float refitMult = 1 / percentTotal;
		logger::debug("percentTotal = {} | refitMult = {}", percentTotal, refitMult);

		float fillX;
		float px = p0.x;
		for (auto category : drawCategories) {
			if (refitMult >= 1) {
				fillX = boxSize.x * categoryPercent.at(category);
			} else {
				fillX = (boxSize.x * categoryPercent.at(category)) * refitMult;
			}

			logger::debug("Category: {} | Percent = {} | Count = {}/{}", 
				CapacityHandler::categoryNames.at(category), categoryPercent.at(category), CapacityHandler::GetCountForGUI(category), CapacityHandler::GetCapacityForGUI(category)
			);

			MCPDraw::AddRectFilled(drawList, 
				ImVec2(px, p0.y), 
				ImVec2(px+fillX, p0.y+boxSize.y), 
				categoryColours.at(category), 0.0f, 0
			);

			if (MCP_API::IsMouseHoveringRect(ImVec2(px, p0.y), ImVec2(px+fillX, p0.y+boxSize.y), ImGuiHoveredFlags_DelayNone)) {
				MCP_API::BeginTooltip();
				MCP_API::Text(categoryTooltips.at(category));
				MCP_API::EndTooltip();
			}

			px += fillX;
		}

		if (refitMult < 1) {
			MCPDraw::AddLine(drawList, ImVec2(p0.x+(boxSize.x*refitMult), p0.y), ImVec2(p0.x+(boxSize.x*refitMult), p0.y+boxSize.y), borderCol, 5.0f);
			DrawHatchFill(drawList, ImVec2(p0.x+(boxSize.x*refitMult)+2.0f, p0.y+1.0f), ImVec2(p0.x+boxSize.x-1.0f, p0.y+boxSize.y-1.0f));
		}

		MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+boxSize.x, p0.y+boxSize.y), borderCol, 0.0f, 0, borderThick);
	}

	void CapacityVisualiserMisc(float y_max)
	{
		logger::debug("Starting CapacityVisualiserMisc");
		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);

		auto barGap = 10.0f;

		ImVec2 boxSize = ImVec2(40.0f, (y_max-p0.y));

		float alchemyDivGap = boxSize.y/CapacityHandler::GetCapacityForGUI(ItemCategories::kAlchemy);
		float ammoDivGap = boxSize.y/CapacityHandler::GetCapacityForGUI(ItemCategories::kAmmo);
		float coinDivGap = boxSize.y/CapacityHandler::GetCapacityForGUI(ItemCategories::kCoin);
		ImU32 fillColour;
		ImVec2 p1 = ImVec2(p0.x+boxSize.x, p0.y+boxSize.y);

		std::vector<float> dividerVec = {alchemyDivGap, ammoDivGap, coinDivGap};
		std::vector<ItemCategories> categoryVec = {
			ItemCategories::kAlchemy,
			ItemCategories::kAmmo,
			ItemCategories::kCoin
		};

		int itDiv = 0;
		for (auto category: categoryVec) {
			// Draw progress bar fill colour
			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				// Determine colour (on a green-yellow-red scale) based on percentage of capacity filled, per category
				fillColour = PercentageColour(CapacityHandler::GetCountForGUI(category), CapacityHandler::GetCapacityForGUI(category));

				if (CapacityHandler::GetCountForGUI(category) < CapacityHandler::GetCapacityForGUI(category)) {
					MCPDraw::AddRectFilled(drawList, ImVec2(p0.x, p1.y-(dividerVec[itDiv]*CapacityHandler::GetCountForGUI(category))), p1, fillColour, 0.0f, 0);
				} else {
					MCPDraw::AddRectFilled(drawList, p0, p1, fillColour, 0.0f, 0);
				}
			}

			const char *tooltipText;
			if (category == ItemCategories::kAlchemy) {
				tooltipText = "Alchemy";
			} else if (category == ItemCategories::kAmmo) {
				tooltipText = "Ammunition";
			} else if (category == ItemCategories::kCoin) {
				tooltipText = "Coins";
			} else {
				tooltipText = "CATEGORY ERROR";
			}

			CapacityCategoryTooltip(p0, p1, tooltipText, category);

			// Bounding box
			MCPDraw::AddRect(drawList, p0, p1, borderCol, 0.0f, 0, borderThick);

			int i = 1;
			// Capacity dividers (skip drawing the dividing lines if there are too many, as it just makes the fill colour go weird)
			if (CapacityHandler::GetCapacityForGUI(category) < (p1.y - p0.y)) {
				while (i < CapacityHandler::GetCapacityForGUI(category)) {
					MCPDraw::AddLine(drawList, ImVec2(p0.x, p0.y+(dividerVec[itDiv]*i)), ImVec2(p1.x-1, p0.y+(dividerVec[itDiv]*i)), borderCol, borderThin);
					i++;
				}
			}
			
			itDiv++;
			p0.x += boxSize.x + barGap;
			p1.x += boxSize.x + barGap;
		}
	}

	float CapacityVisualiserWeapons()
	{
		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);

		// Determine row/column dimensions depending on what data is being shown
		int rowCount = 5;
		float mainRowGap = 40.0f;
		ImVec2 mainSize = ImVec2(MCP_API::GetWindowWidth() * 0.65f, mainRowGap*rowCount);
		float largeDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponLarge);
		float mediumDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponMedium);
		float smallDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponSmall);
		float rangedDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kWeaponRanged);
		float shieldDivGap = mainSize.x/CapacityHandler::GetCapacityForGUI(ItemCategories::kShield);
		ImU32 fillColour;

		std::vector<float> dividerVec = {largeDivGap, mediumDivGap, smallDivGap, rangedDivGap, shieldDivGap};
		std::vector<ItemCategories> categoryVec = {
			ItemCategories::kWeaponLarge,
			ItemCategories::kWeaponMedium,
			ItemCategories::kWeaponSmall,
			ItemCategories::kWeaponRanged,
			ItemCategories::kShield
		};

		// Just some misc iterator values
		float itRow1 = 0;
		float itRow2 = 1;
		int itDiv = 0;
		int i = 1;

		// Draw the filled & coloured progress/fill bars for each main category
		for (auto category: categoryVec) {
			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				// Determine colour (on a green-yellow-red scale) based on capacity filled per category
				fillColour = PercentageColour(CapacityHandler::GetCountForGUI(category), CapacityHandler::GetCapacityForGUI(category));

				// Draw progress bar
				if (CapacityHandler::GetCountForGUI(category) < CapacityHandler::GetCapacityForGUI(category)) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), 
						ImVec2(p0.x+(dividerVec[itDiv]*(CapacityHandler::GetCountForGUI(category))), p0.y+(mainRowGap*itRow2)), 
						fillColour, 0.0f, 0
					);
				} else {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), 
						ImVec2(p0.x+mainSize.x, p0.y+(mainRowGap*itRow2)), 
						fillColour, 0.0f, 0
					);
				}
			}

			const char *tooltipText;
			if (category == ItemCategories::kWeaponLarge) {
				tooltipText = "Large Weapons";
			} else if (category == ItemCategories::kWeaponMedium) {
				tooltipText = "Medium Weapons";
			} else if (category == ItemCategories::kWeaponSmall) {
				tooltipText = "Small Weapons";
			} else if (category == ItemCategories::kWeaponRanged) {
				tooltipText = "Ranged Weapons";
			} else if (category == ItemCategories::kShield) {
				tooltipText = "Shields";
			} else {
				tooltipText = "CATEGORY ERROR";
			}

			CapacityCategoryTooltip(ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), ImVec2(p0.x+mainSize.x, p0.y+(mainRowGap*itRow2)), tooltipText, category);

			itRow1++;
			itRow2++;
			itDiv++;
		}

		// Draw visualiser box/outer borders
		MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+mainSize.x, p0.y+mainSize.y), borderCol, 0.0f, 0, borderThick);

		// Draw horizontal dividers for each main storage category
		i = 1;
		while (i < rowCount) {
			MCPDraw::AddLine(drawList, ImVec2(p0.x, p0.y+(mainRowGap*i)), ImVec2(p0.x+mainSize.x-1, p0.y+(mainRowGap*i)), borderCol, borderThick);
			i++;
		}

		itRow1 = 0;
		itRow2 = 1;
		itDiv = 0;

		// Draw vertical dividers for each main storage category
		for (auto category: categoryVec) {
			i = 1;
			while (i < CapacityHandler::GetCapacityForGUI(category)) {
				MCPDraw::AddLine(drawList, ImVec2(p0.x+(dividerVec[itDiv]*i), p0.y+(mainRowGap*itRow1)), ImVec2(p0.x+(dividerVec[itDiv]*i), p0.y+(mainRowGap*itRow2)), borderCol, borderThin);
				i++;
			}

			itRow1++;
			itRow2++;
			itDiv++;
		}

		return { p0.y + mainSize.y };
	}
}