#include "Calc.h"
#include "Player.h"

float Calc::StaminaWeightBonus(float a_stamVal, float maxGradStam)
{
	float rate = *Settings::Get<float*>("fStaminaWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uStaminaWeightPivot");

	float eq1;
	float eq2 = (rate - (rate * (a_stamVal - Player::BaseStam)) + pivot
	);
	float eq3 = *Settings::Get<uint32_t*>("uBaseCarryWeight") / 4;
	float eq4;

	float eqGrad = GradientAtStamina(a_stamVal, rate, pivot);

	if (eq2 <= 0 || eqGrad >= 200) {
		eq1 = maxGradStam - (rate * maxGradStam);
		eq2 = rate - (rate * maxGradStam) + pivot;
		eq4 = (a_stamVal - maxGradStam) * 200;
	} else {
		eq1 = (a_stamVal - Player::BaseStam) - (rate * (a_stamVal - Player::BaseStam));
		eq4 = 0;
	}

	float staminaBonus = ((eq1/eq2) * eq3) + eq4;

	return { staminaBonus };
}

float Calc::StaminaWeightBonus()
{
	return { StaminaWeightBonus(Player::UpdateAndGetStamAV(), Player::StamAtMaxGrad) };
}

float Calc::GradientAtStamina(float stamVal, float a_rate, uint32_t a_pivot)
{
	float grad1 = (a_rate - 1) * (a_rate + a_pivot);
	float grad2 = a_rate + a_pivot - (a_rate * (stamVal - Player::BaseStam));
	float grad = grad1 / float(pow(grad2, 2));

	return { grad };
}

float Calc::LevelWeightBonus(float a_Lvl, float maxGradLvl)
{
	float rate = *Settings::Get<float*>("flevelWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uLevelWeightPivot");

	float eq1;
	float eq2 = rate - (rate * a_Lvl) + pivot;
	float eq3 = *Settings::Get<uint32_t*>("uBaseCarryWeight") / 2;
	float eq4;

	float eqGrad = GradientAtLevel(a_Lvl, rate, pivot);

	// With +ve values of fLevelWeightRate, as level increases, eq2 gets smaller - as it approaches 0, the value of levelBonus grows to infinity
	// At -ve eq2 values, the graph flips and levelBonus remains at a value lower than at a player level of 0
	// Here we just need to check that this hasn't happened (as it entirely depends on the user-set values of fLevelWeightRate and uLevelWeightPivot)
	// And if it has happened, swap to a placeholder growth rate (eq4, 200 weight per level) to stop the whole calculation breaking
	// Probably a tad overcomplicated and excessive, but then again so is this whole mod frankly
	//NOTE: Could probably at some point do with seeing if I can optimise or simplify it, unless it magically works with zero noticeable performance impact
	if (eq2 <= 0 || eqGrad > 200) {
		eq1 = maxGradLvl - (rate * maxGradLvl);
		eq2 = rate - (rate * maxGradLvl) + pivot;
		eq4 = (a_Lvl - maxGradLvl) * 200;
	} else {
		eq1 = a_Lvl - (rate * a_Lvl);
		eq4 = 0;
	}

	float levelBonus = ((eq1/eq2) * eq3) + eq4;

	return { levelBonus };
}

float Calc::LevelWeightBonus()
{
	return { LevelWeightBonus(Player::UpdateAndGetLevel(), Player::LevelAtMaxGrad) };
}

float Calc::GradientAtLevel(float a_Lvl, float a_rate, uint32_t a_pivot)
{
	float grad1 = (a_rate - 1) * (a_rate + a_pivot);
	float grad2 = a_rate + a_pivot - (a_rate * a_Lvl);
	float grad = grad1 / float(pow(grad2, 2));

	return grad;
}