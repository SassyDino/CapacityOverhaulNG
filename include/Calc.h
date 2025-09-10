#pragma once

namespace Calc
{
	static const float sqrt2 = float(sqrt(2));

	float StaminaWeightBonus(float a_stamVal, float a_rate, uint32_t a_pivot, uint32_t baseCarry, float maxGradStam);
	float StaminaWeightBonusCurrent();
	float GradientAtStamina(float a_stamVal, float a_rate, uint32_t a_pivot);

	float LevelWeightBonus(float a_Lvl, float a_rate, uint32_t a_pivot, uint32_t baseCarry, float maxGradLvl);
	float LevelWeightBonusCurrent();
	float GradientAtLevel(float a_Lvl, float a_rate, uint32_t a_pivot, uint32_t baseCarry);

	float* GetStaminaPlotData(int x_max, float a_rate, uint32_t a_pivot, uint32_t baseCarry, int sampleRate);
	float* GetLevelPlotData();
}