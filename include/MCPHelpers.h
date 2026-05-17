#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"

namespace GUI::MCP
{
	struct VecBounds2
	{
		SKSEMenuFramework::ImVec2 p0, p1;
	};

	struct VecBounds4
	{
		SKSEMenuFramework::ImVec2 tl, tr, bl, br;
	};

	const enum hAlign {NoHAlign, LeftAlign, CentreHAlign, RightAlign};
	const enum vAlign {NoVAlign, TopAlign, CentreVAlign, BottomAlign};

	void PushCustomText(float a_scaleMult, ImU32 a_colour);
	void PopCustomText();

	SKSEMenuFramework::ImVec4 AlignedText(const char *text, SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1, hAlign kh, vAlign kv);
	
	void CenteredText(const char *text, SKSEMenuFramework::ImVec2 a_p0);
	//void VerticalText(SKSEMenuFramework::ImDrawList* draw_list, const char *text, SKSEMenuFramework::ImVec2* a_p0);

	std::string GetWidgetHelpText(std::string a_key);
	VecBounds2 DrawHelpBox(SKSEMenuFramework::ImDrawList *drawList);

	void DrawHatchFill(SKSEMenuFramework::ImDrawList *drawList, SKSEMenuFramework::ImVec2 p0, SKSEMenuFramework::ImVec2 p1);

	void CursorDebugMarker(unsigned int a_colour, SKSEMenuFramework::ImVec2 a_p0 = SKSEMenuFramework::ImVec2(0.0f,0.0f), float a_radius = 5.0f);
}