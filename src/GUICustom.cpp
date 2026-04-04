#include "GUICustom.h"
#include "MCP.h"
#include "CapacityHandler.h"
//using namespace SKSEMenuFramework;
using namespace ImGuiMCP;
using namespace CapacityHandler;
namespace SKSEMF = SKSEMenuFramework;
namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace GUI
{
	const std::string Assets::dir = ".\\Data\\Interface\\CapacityOverhaulNG\\Icons\\";
	const std::string Assets::type = "png";

	ImGuiMCP::ImTextureID Assets::CheckboxEmpty;
	ImGuiMCP::ImTextureID Assets::CheckboxFilled;

	ImGuiMCP::ImTextureID Assets::SeparatorLeft;
	ImGuiMCP::ImTextureID Assets::SeparatorRight;

	ImGuiMCP::ImTextureID Assets::HeaderBackLeft;
	ImGuiMCP::ImTextureID Assets::HeaderBackRight;
	ImGuiMCP::ImTextureID Assets::HeaderFrontLeft;
	ImGuiMCP::ImTextureID Assets::HeaderFrontRight;
	ImGuiMCP::ImTextureID Assets::HeaderArrowDown;
	ImGuiMCP::ImTextureID Assets::HeaderArrowUpLeft;
	ImGuiMCP::ImTextureID Assets::HeaderArrowUpRight;

	ImGuiMCP::ImTextureID Assets::ScrollbarLeft;
	ImGuiMCP::ImTextureID Assets::ScrollbarRight;
	ImGuiMCP::ImTextureID Assets::ScrollbarSelector;

	float Assets::defaultFontSize;

	std::string Assets::GetTexPath(std::string a_filename)
	{
		std::string path = std::string(dir + a_filename + "." + type);
		logger::debug("{}", path);
		return path;
	}

	void Assets::LoadTextures()
	{
		if (!SKSEMenuFramework::IsInstalled()) {
			logger::warn("SKSE Menu Framework is not installed: loading textures skipped.");
			return;
		}

		//NOTE: Loading textures here rather than in the actual widget function (and making use of the caching) because otherwise the game freezes for a brief moment when opening the CONG MCP while it loads the textures for the first time.
		logger::info("Loading textures...");

		CheckboxEmpty = SKSEMF::LoadTexture(GetTexPath("Checkbox_Empty"));
		CheckboxFilled = SKSEMF::LoadTexture(GetTexPath("Checkbox_Filled"));

		SeparatorLeft = SKSEMF::LoadTexture(GetTexPath("Separator_Knot_L"));
		SeparatorRight = SKSEMF::LoadTexture(GetTexPath("Separator_Knot_R"));

		HeaderBackLeft = SKSEMF::LoadTexture(GetTexPath("Header_Back_L"));
		HeaderBackRight = SKSEMF::LoadTexture(GetTexPath("Header_Back_R"));
		HeaderFrontLeft = SKSEMF::LoadTexture(GetTexPath("Header_Detail_L"));
		HeaderFrontRight = SKSEMF::LoadTexture(GetTexPath("Header_Detail_R"));
		HeaderArrowDown = SKSEMF::LoadTexture(GetTexPath("Header_Arrow_Down"));
		HeaderArrowUpLeft = SKSEMF::LoadTexture(GetTexPath("Header_Arrow_Up_L"));
		HeaderArrowUpRight = SKSEMF::LoadTexture(GetTexPath("Header_Arrow_Up_R"));

		ScrollbarLeft = SKSEMF::LoadTexture(GetTexPath("Scrollbar_Arrow_Left"));
		ScrollbarRight = SKSEMF::LoadTexture(GetTexPath("Scrollbar_Arrow_Right"));
		ScrollbarSelector = SKSEMF::LoadTexture(GetTexPath("Scrollbar_Selector"));

		logger::trace("Checkbox_Empty = {:X} | Checkbox_Filled = {:X}", reinterpret_cast<uint64_t>(CheckboxEmpty), reinterpret_cast<uint64_t>(CheckboxFilled));

		logger::info("Textures loaded!\n{}", std::string(100, '='));
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

		ImVec2 p2;
		MCP_API::GetCursorScreenPos(&p2);

		switch(kh) {
			case CentreHAlign:
				MCP_API::SetCursorScreenPos(ImVec2(p2.x + ((availWidth - textSize.x) * 0.5f), p2.y));
				break;
			case RightAlign:
				MCP_API::SetCursorScreenPos(ImVec2(p2.x + (availWidth - textSize.x), p2.y));
				break;
			default:
				break;
		}

		MCP_API::GetCursorScreenPos(&p2);
		switch(kv) {
			case CentreVAlign:
				MCP_API::SetCursorScreenPos(ImVec2(p2.x, p2.y + ((availHeight - textSize.y) * 0.5f)));
				break;
			case BottomAlign:
				MCP_API::SetCursorScreenPos(ImVec2(p2.x, p2.y + (availHeight - textSize.y)));
				break;
			default:
				break;
		}

		MCP_API::Text(text);

		return ImVec4(p2.x, p2.y, p2.x+textSize.x, p2.y+textSize.y);
	}

	bool CustomHeader(const char *text)
	{
		if (!Settings::Get<bool>("bCustomMenuStyling")) { return MCP_API::CollapsingHeader(text, SKSEMenuFramework::ImGuiTreeNodeFlags_DefaultOpen); }

		bool* pOpen = MCP_API::ImGuiStorageManger::GetBoolRef(MCP_API::GetStateStorage(), MCP_API::GetID(text), true);

		ImVec2 p0;
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		MCP_API::GetCursorScreenPos(&p0);

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

	//TODO: Custom styling, perhaps
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

		//TODO: Add a modal to prevent accidental clicks
		if (MCP_API::Button("Reset to Default")) {
			logger::info("Loading default mod settings.");
			Settings::Load(Settings::defaultPath);
		}
	}

	void LanguageSelector()
	{
		std::string defaultText = "No Language Selected";
		std::filesystem::path filePath = R"(Data\Interface\Translations\)";

		std::vector<std::string> availableFiles = {defaultText};
		auto selected = 0;
		const char* previewLang = availableFiles[selected].c_str();

		// Button to refresh & search folder to get available translation files
		// Code is based on clib_util::distribution::get_config_paths()
		if (MCP_API::Button("Locate Translation Files")) {
			for (const auto it = std::filesystem::directory_iterator(filePath); const auto& entry : it) {
				if (entry.exists()) {
					if (const auto& path = entry.path(); !path.empty() && path.extension() == ".toml") {
						if (const auto& fileName = entry.path().string(); fileName.rfind("CapacityOverhaulNG_") != std::string::npos) {
							availableFiles.push_back(fileName);
						}
					}
				}
			}
		}

		// Dropdown containing available language files
		if (MCP_API::BeginCombo("Available Languages", previewLang)) {
			for (int n = 0; n < availableFiles.size(); n++) {
				const bool is_selected = (selected == n);
				if (MCP_API::Selectable(availableFiles[n].c_str(), is_selected)) {
					selected = n;
				}

				if (is_selected) {
					MCP_API::SetItemDefaultFocus();
				}
			}

			MCP_API::EndCombo();
		}

		// Button to confirm & apply change
		if (MCP_API::Button("Apply Language File")) {
			if (availableFiles[selected] == defaultText) {
				MCP_API::Text("Select a language file to apply.");
			} else {
				MCP_API::Text("Changing language...");

				Settings::Get<std::string>("sLanguage") = Lang::GetLanguageFromFileName(availableFiles[selected]);

				Lang::LoadTranslations(filePath.string() + availableFiles[selected]);

				MCP_API::Text("Language file applied.");
			}
		}
	}

	void RefreshLogLevelButton()
	{
		if (MCP_API::Button("Update From Settings")) { Utils::UpdateLogLevel(); }
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
			CapacityHandler::Player::UpdateAllCategories(true);
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
			{ItemCategories::kHuge, HEX_COL32(0xA8005BFF)},
			{ItemCategories::kLarge, HEX_COL32(0xBA422FFF)},
			{ItemCategories::kMedium, HEX_COL32(0xA87A00FF)},
			{ItemCategories::kSmall, HEX_COL32(0x7AA62EFF)},
			{ItemCategories::kTiny, HEX_COL32(0x00CC85FF)},
			{ItemCategories::kAlchemy, HEX_COL32(0xBF4FB0FF)},
			{ItemCategories::kAmmo, HEX_COL32(0xA64FD6FF)},
			{ItemCategories::kCoin, HEX_COL32(0x635CFFFF)},
			{ItemCategories::kWeaponLarge, HEX_COL32(0x292E57FF)},
			{ItemCategories::kWeaponMedium, HEX_COL32(0x6B4573FF)},
			{ItemCategories::kWeaponSmall, HEX_COL32(0xAD597DFF)},
			{ItemCategories::kWeaponRanged, HEX_COL32(0xE07D78FF)},
			{ItemCategories::kShield, HEX_COL32(0xFAB270FF)}
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