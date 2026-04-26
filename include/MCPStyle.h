#pragma once

#include "SKSEMCP/SKSEMenuFramework.hpp"

namespace GUI::MCP
{
	const extern float borderThick;
	const extern float borderThin;

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
}