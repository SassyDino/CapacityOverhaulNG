#pragma once
#include "MCPColour.h"

namespace GUI::MCP
{
	struct Selections
	{
		static inline bool visualiserSeparateWindow = false;
		static inline bool visualiserBaseValues = false;
		static inline bool visualiserShowFilled = true;

		static inline Colour::Gradient::Scheme* heatmapGradient = &Colour::Gradient::gClassic;
		static inline bool heatmapConstrainGradient = true;
		static inline float heatmapConstraintVal = 1500;
	};
	
}