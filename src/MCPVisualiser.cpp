#include "MCPVisualiser.h"
#include "MCPHelpers.h"
#include "MCPStyle.h"
#include "MCPSettings.h"
#include "MCP.h"
#include "CapacityHandler.h"
#include "Player.h"

using namespace ImGuiMCP;
using namespace CapacityHandler;

namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace GUI::MCP
{
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
		if (Selections::visualiserShowFilled && PlayerState::Char) {
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

	void CapacityVisualiser(bool isPlayerLoaded)
	{
		ImDrawList *drawList = MCP_API::GetWindowDrawList();

		CapacityHandler::UpdateBaseCapacities();

		if (isPlayerLoaded) {
			if (Selections::visualiserBaseValues) {
				CapacityHandler::CalculateActualCapacities();
			}
			if (Selections::visualiserShowFilled) {
				CapacityHandler::UpdateAllCategories(true);
			}
		}

		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);

		ImVec2 windowPos;
		MCP_API::GetWindowPos(&windowPos);
		Layout::windowWidth = MCP_API::GetWindowWidth() - ((p0.x - windowPos.x) * 2);
		if (MCP_API::GetScrollMaxY() > 0.0f) { Layout::windowWidth -= MCP_API::GetStyle()->ScrollbarSize; }

		Layout::cvBorderCol = MCP_API::GetColorU32(ImGuiCol_Border);

		float pEnd;
		MCP_API::BeginGroup();
		{
			if (Selections::visualiserShowFilled && isPlayerLoaded) {
				MCP_API::Text("$MCP.Widgets.CapacityVisualiser.Summary"_tr);
				CapacityVisualiserTotal(drawList, p0);
			}

			MCP_API::BeginGroup();
			{
				MCP_API::Text("$MCP.Widgets.CapacityVisualiser.Main"_tr);
				CapacityVisualiserMain(drawList, p0, isPlayerLoaded);

				MCP_API::Text("$MCP.Widgets.CapacityVisualiser.Weapon"_tr);
				pEnd = CapacityVisualiserWeapons(drawList, p0, isPlayerLoaded);
				MCP_API::EndGroup();
			}
			MCP_API::SameLine();

			MCP_API::BeginGroup();
			{
				MCP_API::Text("$MCP.Widgets.CapacityVisualiser.Misc"_tr);
				CapacityVisualiserMisc(drawList, p0, isPlayerLoaded, pEnd);
				MCP_API::EndGroup();
			}
		}
		MCP_API::NewLine();
	}

	void CapacityVisualiserWindow(bool *isOpen, bool isPlayerLoaded)
	{
		if (*isOpen) {
			MCP_API::Begin("$MCP.Widgets.CapacityVisualiserWindow.Title"_tr, isOpen, 0);
			CapacityVisualiser(isPlayerLoaded);
			MCP_API::End();
		}
	}

	void CapacityVisualiserMain(ImDrawList* drawList, ImVec2 &p0, bool isPlayerLoaded)
	{
		MCP_API::GetCursorScreenPos(&p0);

		// Determine row/column dimensions depending on what data is being shown
		int rowCount = (Settings::Get<bool>("bHugeCapacityShared")) ? 5 : 4;
		Layout::cvMainSize = ImVec2(Layout::windowWidth * 0.65f, Layout::cvBarThickness*rowCount);
		Layout::cvHugeDivGap = Layout::cvMainSize.x/cHuge.GetCapacityForGUI();
		Layout::cvLargeDivGap = Layout::cvMainSize.x/cLarge.GetCapacityForGUI();
		Layout::cvMediumDivGap = Layout::cvMainSize.x/cMedium.GetCapacityForGUI();
		Layout::cvSmallDivGap = Layout::cvMainSize.x/cSmall.GetCapacityForGUI();
		Layout::cvTinyDivGap = Layout::cvMainSize.x/cTiny.GetCapacityForGUI();

		float dummyHeight = Layout::cvMainSize.y;

		// Draw independent "huge" category bar, if settings are set as so
		if (!Settings::Get<bool>("bHugeCapacityShared")) {
			dummyHeight += Layout::cvBarThickness + Layout::smallGap;
			CapacityVisualiserSeparateHuge(drawList, p0, isPlayerLoaded);
		}

		CapacityVisualiserFillMain(drawList, p0, isPlayerLoaded);

		// Draw visualiser box/outer borders
		MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+Layout::cvMainSize.x, p0.y+Layout::cvMainSize.y), Layout::cvBorderCol, 0.0f, 0, Layout::borderThin);

		// Draw horizontal dividers for each main storage category
		for (int i = 1; i < rowCount; i++) {
			MCPDraw::AddLine(drawList,
				ImVec2(p0.x, p0.y+(Layout::cvBarThickness*i)),
				ImVec2(p0.x+Layout::cvMainSize.x-1, p0.y+(Layout::cvBarThickness*i)),
				Layout::cvBorderCol,
				Layout::borderThin
			);
		}

		float itRow1 = 0;
		float itRow2 = 1;
		int itDiv = 0;

		// Draw vertical dividers for each main storage category
		for (auto category: Layout::cvCategoryVec) {
			// Skip drawing kHuge row/dividers if the setting is disabled
			if (!Settings::Get<bool>("bHugeCapacityShared") && (category == &CapacityHandler::cHuge)) {
				itDiv++;
				continue;
			}

			// Draw vertical dividers
			for (int i = 1; i < category->GetCapacityForGUI(); i++) {
				MCPDraw::AddLine(drawList,
					ImVec2(p0.x+(*Layout::cvDividerVec[itDiv]*i), p0.y+(Layout::cvBarThickness*itRow1)),
					ImVec2(p0.x+(*Layout::cvDividerVec[itDiv]*i), p0.y+(Layout::cvBarThickness*itRow2)),
					Layout::cvBorderCol,
					Layout::borderThin
				);
			}

			itRow1++;
			itRow2++;
			itDiv++;
		}

		MCP_API::Dummy({Layout::windowWidth*0.65f, dummyHeight});
	}

	void CapacityVisualiserSeparateHuge(ImDrawList* drawList, ImVec2 &p0, bool isPlayerLoaded)
	{
		if (isPlayerLoaded && Selections::visualiserShowFilled) {

			ImU32 fillColour = PercentageColour(
				CapacityHandler::cHuge.GetCountForGUI(),
				CapacityHandler::cHuge.GetCapacityForGUI()
			);

			if (CapacityHandler::cHuge.GetCountForGUI() < CapacityHandler::cHuge.GetCapacityForGUI()) {
				MCPDraw::AddRectFilled(drawList, 
					ImVec2(p0.x, p0.y), 
					ImVec2(p0.x+(Layout::cvHugeDivGap*CapacityHandler::cHuge.GetCountForGUI()), p0.y+Layout::cvBarThickness), 
					fillColour, 0.0f, 0
				);
			} else {
				MCPDraw::AddRectFilled(drawList, 
					ImVec2(p0.x, p0.y), 
					ImVec2(p0.x+Layout::cvMainSize.x, p0.y+Layout::cvBarThickness), 
					fillColour, 0.0f, 0
				);
			}
		}
		
		CapacityCategoryTooltip(p0, ImVec2(p0.x+Layout::cvMainSize.x, p0.y+Layout::cvBarThickness), CapacityHandler::cHuge.GetTooltipText(), &CapacityHandler::cHuge);

		MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+Layout::cvMainSize.x, p0.y+Layout::cvBarThickness), Layout::cvBorderCol, 0.0f, 0, Layout::borderThin);

		for (int i = 1; i < CapacityHandler::cHuge.GetCapacityForGUI(); i++) {
			MCPDraw::AddLine(drawList,
				ImVec2(p0.x+(Layout::cvHugeDivGap*i), p0.y),
				ImVec2(p0.x+(Layout::cvHugeDivGap*i), p0.y+Layout::cvBarThickness),
				Layout::cvBorderCol,
				Layout::borderThin);
		}

		p0.y += (Layout::cvBarThickness + Layout::smallGap); // Add height of bar, plus 5 for padding - need to check that this number is ok
	}

	void CapacityVisualiserFillMain(ImDrawList* drawList, ImVec2 &p0, bool isPlayerLoaded)
	{
		float itRow1 = 0;
		float itRow2 = 1;
		int itDiv = 0;

		// Draw the filled & coloured progress/fill bars for each main category
		for (auto category: Layout::cvCategoryVec) {
			// Skip drawing kHuge progress bar if the setting is disabled
			if (!Settings::Get<bool>("bHugeCapacityShared") && (category == &CapacityHandler::cHuge)) {
				itDiv++;
				continue;
			}

			if (Selections::visualiserShowFilled && isPlayerLoaded) {
				// Determine colour (on a green-yellow-red scale) based on capacity filled per category
				ImU32 fillColour = PercentageColour(category->GetCountForGUI(), category->GetCapacityForGUI());

				// Draw progress bar
				if (category->GetCountForGUI() < category->GetCapacityForGUI()) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(Layout::cvBarThickness*itRow1)), 
						ImVec2(p0.x+(*Layout::cvDividerVec[itDiv]*(category->GetCountForGUI())), p0.y+(Layout::cvBarThickness*itRow2)), 
						fillColour, 0.0f, 0
					);
				} else {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(Layout::cvBarThickness*itRow1)), 
						ImVec2(p0.x+Layout::cvMainSize.x, p0.y+(Layout::cvBarThickness*itRow2)), 
						fillColour, 0.0f, 0
					);
				}
				
			}

			CapacityCategoryTooltip(ImVec2(p0.x, p0.y+(Layout::cvBarThickness*itRow1)), ImVec2(p0.x+Layout::cvMainSize.x, p0.y+(Layout::cvBarThickness*itRow2)), category->GetTooltipText(), category);

			itRow1++;
			itRow2++;
			itDiv++;
		}
	}

	void CapacityVisualiserTotal(ImDrawList* drawList, ImVec2 &p0)
	{
		MCP_API::GetCursorScreenPos(&p0);

		ImVec2 boxSize = ImVec2(Layout::windowWidth, Layout::cvBarThickness);
		
		//? Could possibly save the percent variables somehow, and then maybe check for changes to any of the capacities/counts etc, to save calling GetMCPPercent multiple times.
		
		std::vector<ItemCat*> drawCategories;

		if (Settings::Get<bool>("bHugeCapacityShared")) { drawCategories.push_back(&cHuge); }

		drawCategories.insert(drawCategories.end(), {&cLarge, &cMedium, &cSmall, &cTiny, &cAlchemy, &cAmmo, &cCoin});

		if (Settings::Get<bool>("bSeparateWeaponCategories")) {
			drawCategories.insert(drawCategories.end(), {&cWeaponLarge, &cWeaponMedium, &cWeaponSmall, &cWeaponRanged, &cShield});
		}

		float percentTotal = 0;
		for (auto category : drawCategories) { percentTotal += category->GetMCPPercent(); }

		float refitMult = 1 / percentTotal;

		float fillX;
		float px = p0.x;
		for (auto category : drawCategories) {
			if (refitMult >= 1) {
				fillX = boxSize.x * category->GetMCPPercent();
			} else {
				fillX = (boxSize.x * category->GetMCPPercent()) * refitMult;
			}

			MCPDraw::AddRectFilled(drawList, 
				ImVec2(px, p0.y), 
				ImVec2(px+fillX, p0.y+boxSize.y), 
				category->visualiserColour, 0.0f, 0
			);

			if (MCP_API::IsMouseHoveringRect(ImVec2(px, p0.y), ImVec2(px+fillX, p0.y+boxSize.y))) {
				MCP_API::BeginTooltip();
				MCP_API::Text(category->GetTooltipText());
				MCP_API::EndTooltip();
			}

			px += fillX;
		}

		if (refitMult < 1) {
			MCPDraw::AddLine(drawList,
				ImVec2(p0.x+(boxSize.x*refitMult), p0.y),
				ImVec2(p0.x+(boxSize.x*refitMult), p0.y+boxSize.y),
				Layout::cvBorderCol, Layout::borderThick
			);

			DrawHatchFill(drawList, ImVec2(p0.x+(boxSize.x*refitMult)+2.0f, p0.y+1.0f), ImVec2(p0.x+boxSize.x-1.0f, p0.y+boxSize.y-1.0f));
		}

		MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+boxSize.x, p0.y+boxSize.y), Layout::cvBorderCol, 0.0f, 0, Layout::borderThin);

		MCP_API::Dummy(ImVec2(Layout::windowWidth, 40));
	}

	void CapacityVisualiserMisc(ImDrawList* drawList, ImVec2 &p0, bool isPlayerLoaded, float y_max)
	{
		MCP_API::GetCursorScreenPos(&p0);

		ImVec2 boxSize = ImVec2(Layout::cvBarThickness, (y_max-p0.y));

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
			if (Selections::visualiserShowFilled && isPlayerLoaded) {
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
			MCPDraw::AddRect(drawList, p0, p1, Layout::cvBorderCol, 0.0f, 0, Layout::borderThin);

			// Capacity dividers (skip drawing the dividing lines if there are too many, as it just makes the fill colour go weird)
			if (category->GetCapacityForGUI() < (p1.y - p0.y)) {
				for (int i = 1; i < category->GetCapacityForGUI(); i++) {
					MCPDraw::AddLine(drawList,
						ImVec2(p0.x, p0.y+(dividerVec[itDiv]*i)),
						ImVec2(p1.x-1, p0.y+(dividerVec[itDiv]*i)),
						Layout::cvBorderCol, Layout::borderThin
					);
				}
			}
			
			itDiv++;
			p0.x += boxSize.x + Layout::smallGap;
			p1.x += boxSize.x + Layout::smallGap;
		}
	}

	float CapacityVisualiserWeapons(ImDrawList* drawList, ImVec2 &p0, bool isPlayerLoaded)
	{
		MCP_API::GetCursorScreenPos(&p0);

		// Determine row/column dimensions depending on what data is being shown
		int rowCount = 5;
		ImVec2 mainSize = ImVec2(Layout::windowWidth * 0.65f, Layout::cvBarThickness*rowCount);
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
			if (Selections::visualiserShowFilled && isPlayerLoaded) {
				// Determine colour (on a green-yellow-red scale) based on capacity filled per category
				fillColour = PercentageColour(category->GetCountForGUI(), category->GetCapacityForGUI());

				// Draw progress bar
				if (category->GetCountForGUI() < category->GetCapacityForGUI()) {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(Layout::cvBarThickness*itRow1)), 
						ImVec2(p0.x+(dividerVec[itDiv]*(category->GetCountForGUI())), p0.y+(Layout::cvBarThickness*itRow2)), 
						fillColour, 0.0f, 0
					);
				} else {
					MCPDraw::AddRectFilled(drawList, 
						ImVec2(p0.x, p0.y+(Layout::cvBarThickness*itRow1)), 
						ImVec2(p0.x+mainSize.x, p0.y+(Layout::cvBarThickness*itRow2)), 
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

			CapacityCategoryTooltip(ImVec2(p0.x, p0.y+(Layout::cvBarThickness*itRow1)), ImVec2(p0.x+mainSize.x, p0.y+(Layout::cvBarThickness*itRow2)), tooltipText, category);

			itRow1++;
			itRow2++;
			itDiv++;
		}

		// Draw visualiser box/outer borders
		MCPDraw::AddRect(drawList, p0, ImVec2(p0.x+mainSize.x, p0.y+mainSize.y), Layout::cvBorderCol, 0.0f, 0, Layout::borderThin);

		// Draw horizontal dividers for each main storage category
		for (i = 1; i < rowCount; i++) {
			MCPDraw::AddLine(drawList,
				ImVec2(p0.x, p0.y+(Layout::cvBarThickness*i)),
				ImVec2(p0.x+mainSize.x-1, p0.y+(Layout::cvBarThickness*i)),
				Layout::cvBorderCol, Layout::borderThin
			);
		}

		itRow1 = 0;
		itRow2 = 1;
		itDiv = 0;

		// Draw vertical dividers for each main storage category
		for (auto category: categoryVec) {
			for (i = 1; i < category->GetCapacityForGUI(); i++) {
				MCPDraw::AddLine(drawList,
					ImVec2(p0.x+(dividerVec[itDiv]*i), p0.y+(Layout::cvBarThickness*itRow1)),
					ImVec2(p0.x+(dividerVec[itDiv]*i), p0.y+(Layout::cvBarThickness*itRow2)),
					Layout::cvBorderCol, Layout::borderThin
				);
			}

			itRow1++;
			itRow2++;
			itDiv++;
		}

		MCP_API::Dummy(ImVec2(Layout::windowWidth*0.65f, 200));

		return { p0.y + mainSize.y };
	}
}