#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"
#include "CapacityHandler.h"

namespace GUI::MCP
{
	namespace Layout
	{
		inline float cvBarThickness = 40.0f;
		inline SKSEMenuFramework::ImVec2 cvMainSize = {};
		inline SKSEMenuFramework::ImU32 cvBorderCol = 0;

		inline float cvHugeDivGap = 0.0f;
		inline float cvLargeDivGap = 0.0f;
		inline float cvMediumDivGap = 0.0f;
		inline float cvSmallDivGap = 0.0f;
		inline float cvTinyDivGap = 0.0f;

		inline std::vector<float*> cvDividerVec = {&cvHugeDivGap, &cvLargeDivGap, &cvMediumDivGap, &cvSmallDivGap, &cvTinyDivGap};
		inline std::vector<CapacityHandler::ItemCat*> cvCategoryVec = {&CapacityHandler::cHuge, &CapacityHandler::cLarge, &CapacityHandler::cMedium, &CapacityHandler::cSmall, &CapacityHandler::cTiny};
	}

	SKSEMenuFramework::ImU32 PercentageColour(float a_count, float a_capacity);

	void CapacityCategoryTooltip(SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1, const char* a_title, CapacityHandler::ItemCat* a_category);

	void CapacityVisualiser(bool isPlayerLoaded);
	void CapacityVisualiserWindow(bool *isOpen, bool isPlayerLoaded);

	void CapacityVisualiserMain(SKSEMenuFramework::ImDrawList* drawList, SKSEMenuFramework::ImVec2 &p0, bool isPlayerLoaded);
	void CapacityVisualiserSeparateHuge(SKSEMenuFramework::ImDrawList* drawList, SKSEMenuFramework::ImVec2 &p0, bool isPlayerLoaded);
	void CapacityVisualiserFillMain(SKSEMenuFramework::ImDrawList* drawList, SKSEMenuFramework::ImVec2 &p0, bool isPlayerLoaded);

	void CapacityVisualiserTotal(SKSEMenuFramework::ImDrawList* drawList, SKSEMenuFramework::ImVec2 &p0);
	void CapacityVisualiserMisc(SKSEMenuFramework::ImDrawList* drawList, SKSEMenuFramework::ImVec2 &p0, bool isPlayerLoaded, float y_max);
	float CapacityVisualiserWeapons(SKSEMenuFramework::ImDrawList* drawList, SKSEMenuFramework::ImVec2 &p0, bool isPlayerLoaded);
}