#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"
#include "CapacityHandler.h"

using ImU32 = ImGuiMCP::ImU32;

namespace GUI
{
	struct Assets
	{
		static const std::string dir;
		static const std::string type;

		static ImGuiMCP::ImTextureID CheckboxEmpty;
		static ImGuiMCP::ImTextureID CheckboxFilled;

		static ImGuiMCP::ImTextureID SeparatorLeft;
		static ImGuiMCP::ImTextureID SeparatorRight;

		static ImGuiMCP::ImTextureID HeaderBackLeft;
		static ImGuiMCP::ImTextureID HeaderBackRight;
		static ImGuiMCP::ImTextureID HeaderFrontLeft;
		static ImGuiMCP::ImTextureID HeaderFrontRight;
		static ImGuiMCP::ImTextureID HeaderArrowDown;
		static ImGuiMCP::ImTextureID HeaderArrowUpLeft;
		static ImGuiMCP::ImTextureID HeaderArrowUpRight;

		static ImGuiMCP::ImTextureID ScrollbarLeft;
		static ImGuiMCP::ImTextureID ScrollbarRight;
		static ImGuiMCP::ImTextureID ScrollbarSelector;

		static float defaultFontSize;

		static std::string GetTexPath(std::string a_filename);
		static void LoadTextures();
	};

	struct Colour
	{
		#define IM_COL32_R_SHIFT 0
		#define IM_COL32_G_SHIFT 8
		#define IM_COL32_B_SHIFT 16
		#define IM_COL32_A_SHIFT 24
		#define IM_COL32_A_MASK 0xFF000000
		#define IM_COL32(R, G, B, A)                                                                                  \
		(((ImU32)(A) << IM_COL32_A_SHIFT) | ((ImU32)(B) << IM_COL32_B_SHIFT) | ((ImU32)(G) << IM_COL32_G_SHIFT) | \
		((ImU32)(R) << IM_COL32_R_SHIFT))

		#define HEX_COL32(hex) IM_COL32(((hex >> 24) & 0xFF), ((hex >> 16) & 0xFF), ((hex >> 8) & 0xFF), ((hex >> 0) & 0xFF))

		//NOTE: RGB/CMY definitions only here for ease of use during development & testing, can probably remove later.
		static const ImU32 RED = HEX_COL32(0xFF0000FF);
		static const ImU32 YELLOW = HEX_COL32(0xFFFF00FF);
		static const ImU32 GREEN = HEX_COL32(0x00FF00FF);
		static const ImU32 CYAN = HEX_COL32(0x00FFFFFF);
		static const ImU32 BLUE = HEX_COL32(0x0000FFFF);
		static const ImU32 MAGENTA = HEX_COL32(0xFF00FFFF);

		static const ImU32 separatorText = HEX_COL32(0xDDDDDDFF);
		static const ImU32 separatorLine = HEX_COL32(0x999999FF);
		static const ImU32 headerBG = HEX_COL32(0x00000099);
	};

	namespace MCP
	{
		const extern float borderThick;
		const extern float borderThin;
		const extern std::unordered_map<CapacityHandler::ItemCategories, const char *> categoryTooltips;

		const enum hAlign {NoHAlign, LeftAlign, CentreHAlign, RightAlign};
		const enum vAlign {NoVAlign, TopAlign, CentreVAlign, BottomAlign};

		void PushCustomText(float a_scaleMult, ImU32 a_colour);
		void PopCustomText();
		SKSEMenuFramework::ImVec4 AlignedText(const char *text, SKSEMenuFramework::ImVec2* a_p0, SKSEMenuFramework::ImVec2* a_p1, hAlign kh, vAlign kv);

		bool CustomHeader(const char *text);
		SKSEMenuFramework::ImVec4 DrawHeaderBar(ImGuiMCP::ImDrawList* drawList, SKSEMenuFramework::ImVec2 p0, float textSize, float textVertPadding);
		void DrawHeaderArrows(ImGuiMCP::ImDrawList* drawList, bool is_Open, SKSEMenuFramework::ImVec2 hp0, SKSEMenuFramework::ImVec2 hp1, SKSEMenuFramework::ImVec2 tp0, SKSEMenuFramework::ImVec2 tp1, float textVertPadding);

		void CustomSeparator(const char *text);

		void CustomCheckbox(const char *text, bool *a_toggle);

		void CustomSlider(const char *text, float* a_setting, float a_minValue, float a_maxValue);
		void CustomIntSlider(const char *text);
		void CustomFloatSlider(const char *text);

		SKSEMenuFramework::ImVec4 DrawSliderTrack(ImGuiMCP::ImDrawList* drawList, float scale, SKSEMenuFramework::ImVec2 p0);
		std::pair<bool, float> DrawSliderSelector(ImGuiMCP::ImDrawList* drawList, float scale, SKSEMenuFramework::ImVec4 bounds);
		float ValueToSliderXPos(float a_settingValue, float x0, float x1, float a_minValue, float a_maxValue);
		float SliderXPosToValue(float x_pos, float x0, float x1, float a_minValue, float a_maxValue);

		void SnapFloatSlider(float *a_target, float snap);

		void FileManagerButtons();

		void RefreshLogLevelButton();
		
		SKSEMenuFramework::ImU32 PercentageColour(float a_count, float a_capacity);
		void CapacityCategoryTooltip(SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1, const char* a_title, CapacityHandler::ItemCategories a_category);
		void DrawHatchFill(SKSEMenuFramework::ImDrawList *drawList, SKSEMenuFramework::ImVec2 p0, SKSEMenuFramework::ImVec2 p1);

		void CapacityVisualiser();
		void CapacityVisualiserWindow(bool *open_state);

		void CapacityVisualiserMain();
		void CapacityVisualiserTotal();
		void CapacityVisualiserMisc(float y_max);
		float CapacityVisualiserWeapons();

		void CursorDebugMarker(unsigned int a_colour, SKSEMenuFramework::ImVec2 a_p0 = SKSEMenuFramework::ImVec2(0.0f,0.0f), float a_radius = 5.0f);
	}
}