#include "MCPHelpers.h"
#include "MCPStyle.h"
#include "MCP.h"
#include "GUIAssets.h"

using namespace ImGuiMCP;

namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace GUI::MCP
{
	void PushCustomText(float a_scaleMult, unsigned int a_colour)
	{
		// NOTE: This is the best way (that worked) that I found to change the font size for text, but I imagine there has got to be a better way.
		Assets::defaultFontSize = MCP_API::GetFont()->Scale;

		MCP_API::GetFont()->Scale *= a_scaleMult;
		MCP_API::PushFont(MCP_API::GetFont());
		MCP_API::PushStyleColor(ImGuiCol_Text, a_colour);
	}

	void PopCustomText()
	{
		MCP_API::PopStyleColor(1);
		MCP_API::GetFont()->Scale = Assets::defaultFontSize;
		MCP_API::PopFont();
	}

	ImVec4 AlignedText(const char *text, ImVec2 a_p0, ImVec2 a_p1, hAlign kh, vAlign kv)
	{
		ImVec2 textSize;
		MCP_API::CalcTextSize(&textSize, text, NULL, false, 0.0f);

		float availWidth = a_p1.x - a_p0.x;
		float availHeight = a_p1.y - a_p0.y;

		float hOffset = 0.0f;
		float vOffset = 0.0f;

		switch(kh) {
			case CentreHAlign:
				hOffset = 0.5f;
				break;
			case RightAlign:
				hOffset = 1.0f;
				break;
			default:
				break;
		}

		switch(kv) {
			case CentreVAlign:
				vOffset = 0.5f;
				break;
			case BottomAlign:
				vOffset = 1.0f;
				break;
			default:
				break;
		}

		ImVec2 p2 = a_p0;
		p2.x += (availWidth - textSize.x) * hOffset;
		p2.y += (availHeight - textSize.y) * vOffset;

		MCP_API::SetCursorScreenPos(p2);

		MCP_API::Text(text);

		return ImVec4(p2.x, p2.y, p2.x+textSize.x, p2.y+textSize.y);
	}

	void CenteredText(const char *text, SKSEMenuFramework::ImVec2 a_p0)
	{
		ImVec2 textSize;
		MCP_API::CalcTextSize(&textSize, text, NULL, false, 0.0f);

		MCP_API::SetCursorScreenPos(ImVec2(a_p0.x-(textSize.x*0.5), a_p0.y-(textSize.y*0.5)));
		MCP_API::Text(text);
	}

	void VerticalText(ImDrawList* draw_list, const char *text, SKSEMenuFramework::ImVec2* a_p0)
	{
		//TODO: try and figure out a way of doing this
		return;
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

	void CursorDebugMarker(unsigned int a_colour, ImVec2 a_p0, float a_radius)
	{
		if ((a_p0.x == 0) && (a_p0.y == 0)) {
			ImVec2 pDebug;
			MCP_API::GetCursorScreenPos(&pDebug);
			MCPDraw::AddCircleFilled(MCP_API::GetForegroundDrawList(), pDebug, a_radius, a_colour, 0);
		} else {
			MCPDraw::AddCircleFilled(MCP_API::GetForegroundDrawList(), a_p0, a_radius, a_colour, 0);
		}
	}
}