#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"
#include "CapacityHandler.h"

namespace GUI::MCP
{
	const extern std::unordered_map<CapacityHandler::CategoryID, const char *> categoryTooltips;
	
	SKSEMenuFramework::ImU32 PercentageColour(float a_count, float a_capacity);

	void CapacityCategoryTooltip(SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1, const char* a_title, CapacityHandler::ItemCat* a_category);

	void CapacityVisualiser();
	void CapacityVisualiserWindow(bool *open_state);

	void CapacityVisualiserMain();
	void CapacityVisualiserTotal();
	void CapacityVisualiserMisc(float y_max);
	float CapacityVisualiserWeapons();
}