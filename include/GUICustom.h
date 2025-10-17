#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"
#include "CapacityHandler.h"

namespace GUI::Assets
{

}

namespace GUI::MCP
{
	const extern float borderThick;
	const extern float borderThin;
	const extern std::unordered_map<CapacityHandler::ItemCategories, const char *> categoryTooltips;

	void CustomSeparator(const char *text);

	void SnapFloatSlider(float *a_target, float snap);

	void FileManagerButtons();
	
	SKSEMenuFramework::ImU32 PercentageColour(float a_count, float a_capacity);
	void CapacityCategoryTooltip(SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1, const char* a_title, CapacityHandler::ItemCategories a_category);
	void DrawHatchFill(SKSEMenuFramework::ImDrawList *drawList, SKSEMenuFramework::ImVec2 p0, SKSEMenuFramework::ImVec2 p1);

	void CapacityVisualiser();
	void CapacityVisualiserWindow(bool *open_state);

	void CapacityVisualiserMain();
	void CapacityVisualiserTotal();
	void CapacityVisualiserMisc(float y_max);
	float CapacityVisualiserWeapons();
}