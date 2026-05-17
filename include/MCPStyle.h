#pragma once

#include "SKSEMCP/SKSEMenuFramework.hpp"

namespace GUI::MCP
{
	//TODO: Change 'text' parameters to 'textKey' where relevant, to differentiate between funcs/classes that take the translated text as an arg vs those that take the raw key.
	//TODO: Also maybe consider creating custom textKey class to ensure no mixups happen

	namespace Layout
	{
		inline float windowWidth = 0.0f;
		const inline float smallGap = 5.0f;

		const inline float borderThick = 5.0f;
		const inline float borderMedium = 3.0f;
		const inline float borderThin = 1.0f;
	}
	
	bool CustomHeader(std::string a_key);
	SKSEMenuFramework::ImVec4 DrawHeaderBar(ImGuiMCP::ImDrawList* drawList, SKSEMenuFramework::ImVec2 p0, float textSize, float textVertPadding);
	void DrawHeaderArrows(ImGuiMCP::ImDrawList* drawList, bool is_Open, SKSEMenuFramework::ImVec2 hp0, SKSEMenuFramework::ImVec2 hp1, SKSEMenuFramework::ImVec2 tp0, SKSEMenuFramework::ImVec2 tp1, float textVertPadding);

	void CustomSeparator(std::string a_key);

	void CustomHelpDialogueBox(std::string a_key);
	void CustomHelpDialogueBoxWidget(std::string a_key);

	void CustomCheckbox(std::string a_key, bool *a_toggle, bool isEnabled = true);

	void CustomSlider(std::string a_key, float* a_setting, float a_minValue, float a_maxValue, bool isEnabled = true);
	void CustomSliderInt(std::string a_key, uint32_t* a_setting, int a_minValue, int a_maxValue, bool isEnabled = true);
	void CustomSliderFloat(std::string a_key, float* a_setting, float a_minValue, float a_maxValue, bool isEnabled = true);

	SKSEMenuFramework::ImVec4 DrawSliderTrack(ImGuiMCP::ImDrawList* drawList, float scale, SKSEMenuFramework::ImVec2 p0);
	std::pair<bool, float> DrawSliderSelector(ImGuiMCP::ImDrawList* drawList, float scale, SKSEMenuFramework::ImVec4 bounds);
	float ValueToSliderXPos(float a_settingValue, float x0, float x1, float a_minValue, float a_maxValue);
	float SliderXPosToValue(float x_pos, float x0, float x1, float a_minValue, float a_maxValue);
	void SnapFloatSlider(float *a_target, float snap);
}