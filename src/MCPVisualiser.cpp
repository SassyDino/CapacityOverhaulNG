#include "MCPVisualiser.h"
#include "MCPHelpers.h"
#include "MCPStyle.h"
#include "MCP.h"
#include "CapacityHandler.h"

using namespace ImGuiMCP;
using namespace CapacityHandler;

namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace GUI::MCP
{
	const std::unordered_map<CategoryID, const char *> categoryTooltips = {
		{CategoryID::kHuge, "Huge Items"},
		{CategoryID::kLarge, "Large Items"},
		{CategoryID::kMedium, "Medium Items"},
		{CategoryID::kSmall, "Small Items"},
		{CategoryID::kTiny, "Tiny Items"},
		{CategoryID::kAlchemy, "Alchemy Items (Overflow)"},
		{CategoryID::kAmmo, "Ammunition (Overflow)"},
		{CategoryID::kCoin, "Coins (Overflow)"},
		{CategoryID::kWeaponLarge, "Large Weapons (Overflow)"},
		{CategoryID::kWeaponMedium, "Medium Weapons (Overflow)"},
		{CategoryID::kWeaponSmall, "Small Weapons (Overflow)"},
		{CategoryID::kWeaponRanged, "Ranged Weapons (Overflow)"},
		{CategoryID::kShield, "Shields (Overflow)"}
	};

	//TODO: Try and rework this to use my main colour system
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

	void CapacityCategoryTooltip(ImVec2 a_p0, ImVec2 a_p1, const char* a_title, ItemCat* a_category)
	{
		std::string tooltipQtyStr;
		if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
			tooltipQtyStr = std::format("{}/{}", a_category->GetCountForGUI(), a_category->GetCapacityForGUI());
		} else {
			tooltipQtyStr = std::format("-/{}", a_category->GetCapacityForGUI());
		}
		const char* tooltipQuantity = tooltipQtyStr.c_str();

		if (MCP_API::IsMouseHoveringRect(a_p0, a_p1)) {
			MCP_API::BeginTooltip();
			MCP_API::Text(a_title);
			MCP_API::Text(tooltipQuantity);
			MCP_API::EndTooltip();
		}
	}

	void CapacityVisualiser()
	{
		CapacityHandler::UpdateBaseCapacities();
		if (!Settings::Get<bool>("bCapacityVisualiserBaseValues")) {
			CapacityHandler::CalculateActualCapacities();
		}
		if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
			CapacityHandler::UpdateAllCategories(true);
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
		float hugeDivGap = mainSize.x/CapacityHandler::cHuge.GetCapacityForGUI();
		float largeDivGap = mainSize.x/CapacityHandler::cLarge.GetCapacityForGUI();
		float mediumDivGap = mainSize.x/CapacityHandler::cMedium.GetCapacityForGUI();
		float smallDivGap = mainSize.x/CapacityHandler::cSmall.GetCapacityForGUI();
		float tinyDivGap = mainSize.x/CapacityHandler::cTiny.GetCapacityForGUI();
		ImU32 fillColour;

		std::vector<float> dividerVec = {hugeDivGap, largeDivGap, mediumDivGap, smallDivGap, tinyDivGap};
		std::vector<CapacityHandler::ItemCat*> categoryVec = {
			&cHuge,
			&cLarge,
			&cMedium,
			&cSmall,
			&cTiny
		};

		// Just some misc iterator values
		float itRow1 = 0;
		float itRow2 = 1;
		int itDiv = 0;
		int i = 1;

		// Draw independent "huge" category bar, if settings are set as so
		if (!Settings::Get<bool>("bHugeCapacityShared")) {
			fillColour = PercentageColour(
				CapacityHandler::cHuge.GetCountForGUI(),
				CapacityHandler::cHuge.GetCapacityForGUI()
			);

			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				if (CapacityHandler::cHuge.GetCountForGUI() < CapacityHandler::cHuge.GetCapacityForGUI()) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y), 
						ImVec2(p0.x+(hugeDivGap*CapacityHandler::cHuge.GetCountForGUI()), p0.y+mainRowGap), 
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
			
			CapacityCategoryTooltip(p0, ImVec2(p0.x+mainSize.x, p0.y+mainRowGap), CapacityHandler::cHuge.GetTooltipName(), &CapacityHandler::cHuge);

			MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+mainSize.x, p0.y+mainRowGap), borderCol, 0.0f, 0, borderThick);

			while (i < CapacityHandler::cHuge.GetCapacityForGUI()) {
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
			if (!Settings::Get<bool>("bHugeCapacityShared") && (category == &CapacityHandler::cHuge)) {
				itDiv++;
				continue;
			}

			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				// Determine colour (on a green-yellow-red scale) based on capacity filled per category
				fillColour = PercentageColour(category->GetCountForGUI(), category->GetCapacityForGUI());

				// Draw progress bar
				if (category->GetCountForGUI() < category->GetCapacityForGUI()) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), 
						ImVec2(p0.x+(dividerVec[itDiv]*(category->GetCountForGUI())), p0.y+(mainRowGap*itRow2)), 
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

			CapacityCategoryTooltip(ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), ImVec2(p0.x+mainSize.x, p0.y+(mainRowGap*itRow2)), category->GetTooltipName(), category);

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
			if (!Settings::Get<bool>("bHugeCapacityShared") && (category == &CapacityHandler::cHuge)) {
				itDiv++;
				continue;
			}

			// Draw vertical dividers
			i = 1;
			while (i < category->GetCapacityForGUI()) {
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
		ImDrawList* drawList = MCP_API::GetWindowDrawList();
		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);

		ImVec2 boxSize = ImVec2(MCP_API::GetWindowWidth()-35.0f, 40.0f);
		
		std::unordered_map<CategoryID, ImU32> categoryColours = {
			{CategoryID::kHuge, HEX_COL32(0xA8005BFF)},
			{CategoryID::kLarge, HEX_COL32(0xBA422FFF)},
			{CategoryID::kMedium, HEX_COL32(0xA87A00FF)},
			{CategoryID::kSmall, HEX_COL32(0x7AA62EFF)},
			{CategoryID::kTiny, HEX_COL32(0x00CC85FF)},
			{CategoryID::kAlchemy, HEX_COL32(0xBF4FB0FF)},
			{CategoryID::kAmmo, HEX_COL32(0xA64FD6FF)},
			{CategoryID::kCoin, HEX_COL32(0x635CFFFF)},
			{CategoryID::kWeaponLarge, HEX_COL32(0x292E57FF)},
			{CategoryID::kWeaponMedium, HEX_COL32(0x6B4573FF)},
			{CategoryID::kWeaponSmall, HEX_COL32(0xAD597DFF)},
			{CategoryID::kWeaponRanged, HEX_COL32(0xE07D78FF)},
			{CategoryID::kShield, HEX_COL32(0xFAB270FF)}
		};
		
		//TODO: I absolutely hate all of this, I really need to find some better way of doing it bruh
		//NOTE: Also, even aside from the stupidly dense blocks of code that look terrible, there's no way that this is gonna run anywhere near to optimal when going multiple times a second.
		//? Could possibly make the xxxxPercent variables static, and then check for changes to any of the capacities/counts, and only recalculate each percentage as and when needed.

		std::unordered_map<CategoryID, float> categoryPercent = {
			{CategoryID::kHuge, CapacityHandler::cHuge.GetMCPPercent()},
			{CategoryID::kLarge, CapacityHandler::cLarge.GetMCPPercent()},
			{CategoryID::kMedium, CapacityHandler::cMedium.GetMCPPercent()},
			{CategoryID::kSmall, CapacityHandler::cSmall.GetMCPPercent()},
			{CategoryID::kTiny, CapacityHandler::cTiny.GetMCPPercent()},
			{CategoryID::kAlchemy, CapacityHandler::cAlchemy.GetMCPPercent()},
			{CategoryID::kAmmo, CapacityHandler::cAmmo.GetMCPPercent()},
			{CategoryID::kCoin, CapacityHandler::cCoin.GetMCPPercent()},
			{CategoryID::kWeaponLarge, CapacityHandler::cWeaponLarge.GetMCPPercent()},
			{CategoryID::kWeaponMedium, CapacityHandler::cWeaponMedium.GetMCPPercent()},
			{CategoryID::kWeaponSmall, CapacityHandler::cWeaponSmall.GetMCPPercent()},
			{CategoryID::kWeaponRanged, CapacityHandler::cWeaponRanged.GetMCPPercent()},
			{CategoryID::kShield, CapacityHandler::cShield.GetMCPPercent()}
		};
		
		std::vector<CategoryID> drawCategories;

		if (Settings::Get<bool>("bHugeCapacityShared")) { drawCategories.push_back(CategoryID::kHuge); }
		drawCategories.push_back(CategoryID::kLarge);
		drawCategories.push_back(CategoryID::kMedium);
		drawCategories.push_back(CategoryID::kSmall);
		drawCategories.push_back(CategoryID::kTiny);
		drawCategories.push_back(CategoryID::kAlchemy);
		drawCategories.push_back(CategoryID::kAmmo);
		drawCategories.push_back(CategoryID::kCoin);
		if (Settings::Get<bool>("bSeparateWeaponCategories")) {
			drawCategories.push_back(CategoryID::kWeaponLarge);
			drawCategories.push_back(CategoryID::kWeaponMedium);
			drawCategories.push_back(CategoryID::kWeaponSmall);
			drawCategories.push_back(CategoryID::kWeaponRanged);
			drawCategories.push_back(CategoryID::kShield);
		}

		float percentTotal = 0;
		for (auto category : drawCategories) { percentTotal += categoryPercent.at(category); }

		float refitMult = 1 / percentTotal;

		float fillX;
		float px = p0.x;
		for (auto category : drawCategories) {
			if (refitMult >= 1) {
				fillX = boxSize.x * categoryPercent.at(category);
			} else {
				fillX = (boxSize.x * categoryPercent.at(category)) * refitMult;
			}

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
		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		ImU32 borderCol = MCP_API::GetColorU32(ImGuiCol_Border);

		auto barGap = 10.0f;

		ImVec2 boxSize = ImVec2(40.0f, (y_max-p0.y));

		float alchemyDivGap = boxSize.y/CapacityHandler::cAlchemy.GetCapacityForGUI();
		float ammoDivGap = boxSize.y/CapacityHandler::cAmmo.GetCapacityForGUI();
		float coinDivGap = boxSize.y/CapacityHandler::cCoin.GetCapacityForGUI();
		ImU32 fillColour;
		ImVec2 p1 = ImVec2(p0.x+boxSize.x, p0.y+boxSize.y);

		std::vector<float> dividerVec = {alchemyDivGap, ammoDivGap, coinDivGap};
		std::vector<CapacityHandler::ItemCat*> categoryVec = {
			&cAlchemy,
			&cAmmo,
			&cCoin
		};

		int itDiv = 0;
		for (auto category: categoryVec) {
			// Draw progress bar fill colour
			if (Settings::Get<bool>("bCapacityVisualiserShowFilled")) {
				// Determine colour (on a green-yellow-red scale) based on percentage of capacity filled, per category
				fillColour = PercentageColour(category->GetCountForGUI(), category->GetCapacityForGUI());

				if (category->GetCountForGUI() < category->GetCapacityForGUI()) {
					MCPDraw::AddRectFilled(drawList, ImVec2(p0.x, p1.y-(dividerVec[itDiv]*category->GetCountForGUI())), p1, fillColour, 0.0f, 0);
				} else {
					MCPDraw::AddRectFilled(drawList, p0, p1, fillColour, 0.0f, 0);
				}
			}

			const char *tooltipText;
			if (category == &cAlchemy) {
				tooltipText = "Alchemy";
			} else if (category == &cAmmo) {
				tooltipText = "Ammunition";
			} else if (category == &cCoin) {
				tooltipText = "Coins";
			} else {
				tooltipText = "CATEGORY ERROR";
			}

			CapacityCategoryTooltip(p0, p1, tooltipText, category);

			// Bounding box
			MCPDraw::AddRect(drawList, p0, p1, borderCol, 0.0f, 0, borderThick);

			int i = 1;
			// Capacity dividers (skip drawing the dividing lines if there are too many, as it just makes the fill colour go weird)
			if (category->GetCapacityForGUI() < (p1.y - p0.y)) {
				while (i < category->GetCapacityForGUI()) {
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
		float largeDivGap = mainSize.x/CapacityHandler::cWeaponLarge.GetCapacityForGUI();
		float mediumDivGap = mainSize.x/CapacityHandler::cWeaponMedium.GetCapacityForGUI();
		float smallDivGap = mainSize.x/CapacityHandler::cWeaponSmall.GetCapacityForGUI();
		float rangedDivGap = mainSize.x/CapacityHandler::cWeaponRanged.GetCapacityForGUI();
		float shieldDivGap = mainSize.x/CapacityHandler::cShield.GetCapacityForGUI();
		ImU32 fillColour;

		std::vector<float> dividerVec = {largeDivGap, mediumDivGap, smallDivGap, rangedDivGap, shieldDivGap};
		std::vector<CapacityHandler::ItemCat*> categoryVec = {
			&cWeaponLarge,
			&cWeaponMedium,
			&cWeaponSmall,
			&cWeaponRanged,
			&cShield
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
				fillColour = PercentageColour(category->GetCountForGUI(), category->GetCapacityForGUI());

				// Draw progress bar
				if (category->GetCountForGUI() < category->GetCapacityForGUI()) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(mainRowGap*itRow1)), 
						ImVec2(p0.x+(dividerVec[itDiv]*(category->GetCountForGUI())), p0.y+(mainRowGap*itRow2)), 
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
			if (category == &cWeaponLarge) {
				tooltipText = "Large Weapons";
			} else if (category == &cWeaponMedium) {
				tooltipText = "Medium Weapons";
			} else if (category == &cWeaponSmall) {
				tooltipText = "Small Weapons";
			} else if (category == &cWeaponRanged) {
				tooltipText = "Ranged Weapons";
			} else if (category == &cShield) {
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
			while (i < category->GetCapacityForGUI()) {
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