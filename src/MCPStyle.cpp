#include "MCPStyle.h"
#include "MCPHelpers.h"
#include "MCPColour.h"
#include "MCP.h"
#include "GUIAssets.h"

using namespace ImGuiMCP;
namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace GUI::MCP
{
	const float borderThick = 1.0f;
	const float borderThin = 1.0f;
	
	bool CustomHeader(const char *text)
	{
		if (!Settings::Get<bool>("bCustomMenuStyling")) { return MCP_API::CollapsingHeader(text, SKSEMenuFramework::ImGuiTreeNodeFlags_DefaultOpen); }

		bool* pOpen = MCP_API::ImGuiStorageManger::GetBoolRef(MCP_API::GetStateStorage(), MCP_API::GetID(text), true);

		ImVec2 p0;
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		MCP_API::GetCursorScreenPos(&p0);
		p0.x += 10.0f;

		PushCustomText(1.35f, Colour::separatorText);

		float textHeight = MCP_API::GetTextLineHeightWithSpacing();
		float textVertPadding = textHeight * 0.05f;

		auto headerBox = DrawHeaderBar(drawList, p0, textHeight, textVertPadding);
		auto bp0 = ImVec2(headerBox.x, headerBox.y);
		auto bp1 = ImVec2(headerBox.z, headerBox.w);

		MCP_API::SetCursorScreenPos(bp0);
		auto textBox = AlignedText(text, bp0, bp1, CentreHAlign, CentreVAlign);
		auto tp0 = ImVec2(textBox.x, textBox.y);
		auto tp1 = ImVec2(textBox.z, textBox.w);

		DrawHeaderArrows(drawList, *pOpen, bp0, bp1, tp0, tp1, textVertPadding);

		PopCustomText();

		MCP_API::SetCursorScreenPos(bp0);
		MCP_API::Dummy(ImVec2(bp1.x-bp0.x, (bp1.y-bp0.y)+(textVertPadding*5)));
		if (MCP_API::IsItemClicked()) {
			*pOpen ^= 1;
		}

		return *pOpen;
	}

	ImVec4 DrawHeaderBar(ImDrawList* drawList, ImVec2 p0, float textSize, float textVertPadding)
	{
		const auto texSize = ImVec2(170,132);
		const float texAspRatio = 1.2879f;

		float textSpaceNeeded = (textSize + (textVertPadding * 2)); // textSpaceNeeded should represent the vertical space available between the lines/bars of the header
		auto scaledSize = ImVec2(textSpaceNeeded*1.5f*texAspRatio, textSpaceNeeded*1.5f); // The text area of the texture takes up 2/3 of the entire vertical space, so multiplying by 1.5 should give us the total texture height

		ImVec2 windowPos;
		MCP_API::GetWindowPos(&windowPos);
		float windowLeft = p0.x;
		float windowGap = windowLeft - windowPos.x;

		float headerWidth = MCP_API::GetWindowWidth() - (windowGap * 4);
		if (MCP_API::GetScrollMaxY() > 0.0f) { headerWidth -= MCP_API::GetStyle()->ScrollbarSize; }

		p0 = ImVec2(p0.x+windowGap, p0.y);
		auto p1 = ImVec2(p0.x+headerWidth, p0.y+scaledSize.y);

		float lineThickness = scaledSize.y * 0.0606f; //0.0682f
		float lineStartY = scaledSize.y * 0.1061f; //0.0985f
		float backStartY = scaledSize.y * 0.0076f;

		auto textBox = ImVec4(p0.x+scaledSize.x, p0.y+lineStartY+lineThickness, p1.x-scaledSize.x, p1.y-(lineStartY+lineThickness));

		MCPDraw::AddImage(drawList, Assets::HeaderBackLeft, p0, ImVec2(textBox.x, p0.y+scaledSize.y), ImVec2(), ImVec2(1,1), GUI::Colour::headerBG);
		MCPDraw::AddImage(drawList, Assets::HeaderBackRight, ImVec2(textBox.z, p0.y), p1, ImVec2(), ImVec2(1,1), GUI::Colour::headerBG);
		MCPDraw::AddRectFilled(drawList, ImVec2(textBox.x, p0.y+backStartY), ImVec2(textBox.z, p1.y-backStartY), GUI::Colour::headerBG, 0.0f, 0);

		MCPDraw::AddImage(drawList, Assets::HeaderFrontLeft, p0, ImVec2(textBox.x, p0.y+scaledSize.y), ImVec2(), ImVec2(1,1), GUI::Colour::separatorLine);
		MCPDraw::AddImage(drawList, Assets::HeaderFrontRight, ImVec2(textBox.z, p0.y), p1, ImVec2(), ImVec2(1,1), GUI::Colour::separatorLine);
		MCPDraw::AddRectFilled(drawList, ImVec2(textBox.x-5, p0.y+lineStartY), ImVec2(textBox.z+5, textBox.y), GUI::Colour::separatorLine, 0.0f, 0); // +5 to length on either end to add a slight overlap
		MCPDraw::AddRectFilled(drawList, ImVec2(textBox.x-5, p1.y-lineStartY), ImVec2(textBox.z+5, textBox.w), GUI::Colour::separatorLine, 0.0f, 0); // should help to prevent small gaps as a result of scaling and rounding

		return textBox;
	}

	void DrawHeaderArrows(ImDrawList* drawList, bool is_Open, ImVec2 hp0, ImVec2 hp1, ImVec2 tp0, ImVec2 tp1, float textVertPadding)
	{
		auto arrowTexSize = ImVec2(56,56);

		auto headerSize = ImVec2(hp1.x-hp0.x,hp1.y-hp0.y);
		float arrowSide = headerSize.y - (textVertPadding * 8);
		auto arrowScaled = ImVec2(arrowSide, arrowSide);

		float boxMidline = hp0.y + (headerSize.y * 0.5f);

/* Arrows near to text
		auto arrowL_p0 = ImVec2(tp0.x-(arrowSide+(textVertPadding*10.0f)), boxMidline-(arrowSide*0.5f));
		auto arrowL_p1 = ImVec2(arrowL_p0.x+arrowSide, arrowL_p0.y+arrowSide);
		auto arrowR_p0 = ImVec2(tp1.x+(textVertPadding*10.0f), boxMidline-(arrowSide*0.5f));
		auto arrowR_p1 = ImVec2(arrowR_p0.x+arrowSide, arrowL_p0.y+arrowSide);
*/
/* Arrows near to header ends
		auto arrowL_p0 = ImVec2(hp0.x+(textVertPadding*10.0f), boxMidline-(arrowSide*0.5f));
		auto arrowL_p1 = ImVec2(arrowL_p0.x+arrowSide, arrowL_p0.y+arrowSide);
		auto arrowR_p0 = ImVec2(hp1.x-(arrowSide+(textVertPadding*10.0f)), boxMidline-(arrowSide*0.5f));
		auto arrowR_p1 = ImVec2(arrowR_p0.x+arrowSide, arrowL_p0.y+arrowSide);
*/

		float endToText = (headerSize.x - (tp1.x-tp0.x)) * 0.5f;
		float endToArrow = endToText * 0.5f;

		auto arrowL_p0 = ImVec2(hp0.x+(endToArrow-(arrowSide*0.5f)), boxMidline-(arrowSide*0.5f));
		auto arrowL_p1 = ImVec2(arrowL_p0.x+arrowSide, arrowL_p0.y+arrowSide);
		auto arrowR_p0 = ImVec2(hp1.x-(endToArrow+(arrowSide*0.5f)), boxMidline-(arrowSide*0.5f));
		auto arrowR_p1 = ImVec2(arrowR_p0.x+arrowSide, arrowL_p0.y+arrowSide);

		MCPDraw::AddImage(drawList, (is_Open ? Assets::HeaderArrowDown : Assets::HeaderArrowUpLeft), arrowL_p0, arrowL_p1, ImVec2(), ImVec2(1,1), GUI::Colour::separatorLine);
		MCPDraw::AddImage(drawList, (is_Open ? Assets::HeaderArrowDown : Assets::HeaderArrowUpRight), arrowR_p0, arrowR_p1, ImVec2(), ImVec2(1,1), GUI::Colour::separatorLine);
	}

	void CustomSeparator(const char *text)
	{
		if (!Settings::Get<bool>("bCustomMenuStyling")) {
			MCP_API::SeparatorText(text);
			return;
		}

		//TODO: I suspect that this whole widget could maybe be simplified if each individual element (left line, text, right line) was its own separate widget and then combined in this function. Will have to see though
		ImVec2 p0;
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		ImVec2 windowPos;

		MCP_API::GetWindowPos(&windowPos);
		MCP_API::GetCursorScreenPos(&p0);
		float windowLeft = p0.x;
		float windowGap = windowLeft - windowPos.x;

		float scale = 0.5f;
		auto knotTexSize = ImVec2(80,56);
		float knotAspRatio = 1.4286f;
		
		float textDividerRatio = 0.6f;
		float dividerVerticalSpacing = (1-textDividerRatio)/2;

		float indentSize = 80 * scale;
		float textKnotGap = 32 * scale;

		PushCustomText(1.2f, Colour::separatorText);
		float textHeight = MCP_API::GetTextLineHeightWithSpacing();

		MCP_API::Dummy(ImVec2(indentSize+(textKnotGap*2), textHeight));
		MCP_API::SameLine();
		MCP_API::GetCursorScreenPos(&p0);

		MCP_API::Text(text);
		MCP_API::SameLine();

		PopCustomText();

		auto scaledKnotSize = ImVec2(textHeight*knotAspRatio, textHeight);
		auto knotSize = ImVec2(textHeight*textDividerRatio*knotAspRatio, textHeight*textDividerRatio);
		float lineThickness = knotSize.y * 0.0893f; // These numbers represent the thickness/heights of the lines and the gap between them, found by dividing their pixel count in the base texture by the height (y-axis) of the texture.
		float lineStartY = knotSize.y * 0.3393f;

		p0.y += scaledKnotSize.y*dividerVerticalSpacing;
		p0.x -= textKnotGap; 

		MCPDraw::AddImage(drawList, Assets::SeparatorLeft, ImVec2(p0.x-knotSize.x, p0.y+knotSize.y), p0, ImVec2(), ImVec2(1,1), GUI::Colour::separatorLine);
		MCPDraw::AddRectFilled(drawList, ImVec2(p0.x-knotSize.x, p0.y+lineStartY), ImVec2(windowLeft, p0.y+lineStartY+lineThickness), GUI::Colour::separatorLine, 0.0f, 0);
		MCPDraw::AddRectFilled(drawList, ImVec2(p0.x-knotSize.x, p0.y+knotSize.y-lineStartY), ImVec2(windowLeft, p0.y+knotSize.y-(lineStartY+lineThickness)), GUI::Colour::separatorLine, 0.0f, 0);
		MCPDraw::AddRectFilled(drawList, ImVec2(windowLeft, p0.y+lineStartY), ImVec2(windowLeft+lineThickness, p0.y+knotSize.y-lineStartY), GUI::Colour::separatorLine, 0.0f, 0);
		
		MCP_API::GetCursorScreenPos(&p0);
		p0.x += textKnotGap;
		auto p1 = ImVec2(p0.x+knotSize.x, p0.y+knotSize.y);

		MCP_API::GetWindowPos(&windowPos);
		float sepEndX = windowPos.x + MCP_API::GetWindowWidth() - windowGap;

		if (MCP_API::GetScrollMaxY() > 0.0f) {
			sepEndX -= MCP_API::GetStyle()->ScrollbarSize;
		}

		p0.y += scaledKnotSize.y*dividerVerticalSpacing;
		p1.y += scaledKnotSize.y*dividerVerticalSpacing;

		MCPDraw::AddImage(drawList, Assets::SeparatorRight, p0, p1, ImVec2(), ImVec2(1,1), GUI::Colour::separatorLine);
		MCPDraw::AddRectFilled(drawList, ImVec2(p1.x, p0.y+lineStartY), ImVec2(sepEndX, p0.y+lineStartY+lineThickness), GUI::Colour::separatorLine, 0.0f, 0);
		MCPDraw::AddRectFilled(drawList, ImVec2(p1.x, p1.y-lineStartY), ImVec2(sepEndX, p1.y-lineStartY-lineThickness), GUI::Colour::separatorLine, 0.0f, 0);
		MCPDraw::AddRectFilled(drawList, ImVec2(sepEndX-lineThickness, p0.y+lineStartY), ImVec2(sepEndX, p1.y-lineStartY), GUI::Colour::separatorLine, 0.0f, 0);

		//NOTE: This is/was an attempt to draw an angled end to the separator using ImDraw stuff. Couldn't get it to render nicely.
		/* 
		float middleGap = (p1.y - p0.y) - ((lineThickness + lineStartY) * 2);
		float topOvershoot = middleGap + lineThickness;
		float bottomOvershoot = lineThickness * 0.4;

		auto ep1 = ImVec2(p1.x+length+topOvershoot+lineThickness, p0.y+lineStartY);
		auto ep2 = ImVec2(p1.x+length+topOvershoot, p0.y+lineStartY);
		auto ep3 = ImVec2(p1.x+length, p1.y-lineStartY-lineThickness);
		auto ep4 = ImVec2(p1.x+length+bottomOvershoot, p1.y-lineStartY);

		MCPDraw::PathLineTo(drawList, ep1);
		MCPDraw::PathBezierCubicCurveTo(drawList, ep2, ep3, ep4, 3);
		MCPDraw::PathFillConvex(drawList, lineColour);
		*/

 		//NOTE: This is/was an attempt to use an actual texture for the separator end. Works better than the ImDraw method, but antialiasing fucks it up, so maybe come back to this at some point.
		/* 
		auto endTexture = SKSEMenuFramework::LoadTexture(".\\Data\\Interface\\CapacityOverhaulNG\\Icons\\Separator_End.png");
		float endAspRatio = 1.2143f;
		MCPDraw::AddImage(drawList, endTexture, ImVec2(p1.x+length, p0.y+lineStartY), ImVec2(p1.x+length+(((p1.y-lineStartY)-(p0.y+lineStartY))*endAspRatio), p1.y-lineStartY), ImVec2(), ImVec2(1,1), lineColour);
 		*/

		MCP_API::NewLine();
	}

	void CustomCheckbox(const char *text, bool *a_toggle)
	{
		if (!Settings::Get<bool>("bCustomMenuStyling")) {
			MCP_API::Checkbox(text, a_toggle);
			return;
		}

		MCP_API::BeginGroup();
		{
			MCP_API::PushStyleColor(ImGuiCol_Button, ImVec4());
			MCP_API::PushStyleColor(ImGuiCol_ButtonActive, ImVec4());
			MCP_API::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4());
			MCP_API::PushStyleColor(ImGuiCol_Border, ImVec4());
			MCP_API::PushStyleColor(ImGuiCol_BorderShadow, ImVec4());

			MCP_API::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2());

			//Note: Might be worth changing the size (ImVec2) to something adjustable, in case I implement some sort of dynamic scaling etc.
			if (MCP_API::ImageButton(text, (*a_toggle ? Assets::CheckboxFilled : Assets::CheckboxEmpty), ImVec2(40,40), ImVec2(), ImVec2(1,1), ImVec4(), ImVec4(1,1,1,1))) {
				*a_toggle = !*a_toggle;
			}

			MCP_API::SameLine();
			MCP_API::Text(text);

			MCP_API::PopStyleColor(5);
			MCP_API::PopStyleVar(1);
		}
		MCP_API::EndGroup();
	}

	void CustomSlider(const char *text, float* a_setting, float a_minValue, float a_maxValue) {
		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();

		auto scale = 0.75f;

		auto selectorData = DrawSliderTrack(drawList, scale, p0);

		selectorData.z = ValueToSliderXPos(*a_setting, selectorData.w, selectorData.x, a_minValue, a_maxValue);

		auto sliderData = DrawSliderSelector(drawList, scale, selectorData);

		if (sliderData.first == true) {
			Testing::testFloat = SliderXPosToValue(sliderData.second, selectorData.w, selectorData.x, a_minValue, a_maxValue);
		}

		MCP_API::Dummy(ImVec2(450, 60));
		MCP_API::SameLine();
		MCP_API::Text(text);
	}

	void CustomIntSlider(const char *text) {}

	void CustomFloatSlider(const char *text) {}

	ImVec4 DrawSliderTrack(ImGuiMCP::ImDrawList* drawList, float scale, SKSEMenuFramework::ImVec2 p0)
	{
		auto sliderLeft = SKSEMenuFramework::LoadTexture(".\\Data\\Interface\\CapacityOverhaulNG\\Icons\\Scrollbar_Arrow_Left.png");
		auto sliderRight = SKSEMenuFramework::LoadTexture(".\\Data\\Interface\\CapacityOverhaulNG\\Icons\\Scrollbar_Arrow_Right.png");

		auto arrowTexSize = ImVec2(80.0f, 56.0f);
		auto arrowSize = ImVec2(arrowTexSize.x*scale, arrowTexSize.y*scale);
		auto sliderLen = 400.0f * scale;
		auto trackGap = arrowSize.y * 0.0357f;
		auto trackWidth = arrowSize.y * 0.0714f;
		auto p1 = ImVec2(p0.x + arrowSize.x, p0.y + arrowSize.y);

		MCPDraw::AddRectFilled(drawList, ImVec2(p1.x, p0.y+trackGap), ImVec2(p1.x+sliderLen, p0.y+trackGap+trackWidth), MCP_API::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 0.0f, 0);
		MCPDraw::AddRectFilled(drawList, ImVec2(p1.x, p1.y-(trackGap+trackWidth)), ImVec2(p1.x+sliderLen, p1.y-trackGap), MCP_API::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 0.0f, 0);

		MCPDraw::AddRectFilled(drawList, ImVec2(p1.x, p0.y+trackGap+trackWidth), ImVec2(p1.x+sliderLen, p0.y+trackGap+(trackWidth*2.0f)), MCP_API::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)), 0.0f, 0);
		MCPDraw::AddRectFilled(drawList, ImVec2(p1.x, p1.y-(trackGap+(trackWidth*2.0f))), ImVec2(p1.x+sliderLen, p1.y-(trackGap+trackWidth)), MCP_API::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)), 0.0f, 0);

		MCPDraw::AddImage(drawList, sliderLeft, p0, p1, ImVec2(), ImVec2(1.0f, 1.0f), MCP_API::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));
		MCPDraw::AddImage(drawList, sliderRight, ImVec2(p1.x+sliderLen, p0.y), ImVec2(p1.x+sliderLen+arrowSize.x, p1.y), ImVec2(), ImVec2(1.0f, 1.0f), MCP_API::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

		//ImVec4(w = x0, x = x1, y = y1, z = sliderPos)
		return ImVec4(p1.x, p1.x+sliderLen, p1.y-(trackGap+trackWidth));
	}

	std::pair<bool, float> DrawSliderSelector(ImGuiMCP::ImDrawList* drawList, float scale, ImVec4 bounds)
	{
		auto selector = SKSEMenuFramework::LoadTexture(".\\Data\\Interface\\CapacityOverhaulNG\\Icons\\Scrollbar_Selector.png");
		std::pair<bool, float> valState = {false, bounds.z};

		auto selectorTexSize = ImVec2(46.0f, 60.0f);
		auto selectorSize = ImVec2(selectorTexSize.x*scale, selectorTexSize.y*scale);
		auto texOffset = (selectorSize.x/2);

		auto p0 = ImVec2(bounds.z-texOffset, bounds.y-selectorSize.y);
		auto p1 = ImVec2(bounds.z+texOffset, bounds.y);

		if (MCP_API::IsMouseHoveringRect(p0, p1) && MCP_API::IsMouseClicked(0)) {
			ImVec2 mousePos;
			MCP_API::GetMousePos(&mousePos);

			//TODO: Need to account for slider x-bounds
			p0.x = mousePos.x-texOffset;
			p1.x = mousePos.x+texOffset;

			valState.first = true;
		} 

		MCPDraw::AddImage(drawList, selector, p0, p1, ImVec2(), ImVec2(1.0f, 1.0f), MCP_API::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)));

		return valState;
	}

	float ValueToSliderXPos(float a_settingValue, float x0, float x1, float a_minValue, float a_maxValue)
	{
		float valuePercent = (a_settingValue - a_minValue) / (a_maxValue - a_minValue);

		float xPos = ((x1 - x0) * valuePercent) + x0;
		return xPos;
	}

	float SliderXPosToValue(float x_pos, float x0, float x1, float a_minValue, float a_maxValue)
	{
		float xPercent = (x_pos - x0) / (x1 - x0);

		float val = ((a_maxValue - a_minValue) * xPercent) + a_minValue;
		return val;
	}

	void SnapFloatSlider(float *a_target, float snap)
	{
		if (MCP_API::IsItemDeactivatedAfterEdit()) {
			*a_target = round(*a_target * (1/snap)) / (1/snap);
		}
	}
}