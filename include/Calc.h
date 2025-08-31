#pragma once

namespace Calc
{
	static const float sqrt2 = float(sqrt(2));

	float StaminaWeightBonus(float a_stamVal, float maxGradStam);
	float StaminaWeightBonus();
	float GradientAtStamina(float a_stamVal, float a_rate, uint32_t a_pivot);

	float LevelWeightBonus(float a_Lvl, float maxGradLvl);
	float LevelWeightBonus();
	float GradientAtLevel(float a_Lvl, float a_rate, uint32_t a_pivot);
}