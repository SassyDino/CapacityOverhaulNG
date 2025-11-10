#include "Calc.h"
#include "Player.h"

float Calc::StaminaWeightBonus(float a_stamVal, float a_rate, uint32_t a_pivot, uint32_t baseCarry, float maxGradStam)
{
	if (a_stamVal <= PlayerStatus::BaseStam) {return { 0.0f };}

	float eq1;
	float eq2 = (a_rate - (a_rate * (a_stamVal - PlayerStatus::BaseStam)) + a_pivot);
	float eq3 = baseCarry / 4;
	float eq4;

	float eqGrad = GradientAtStamina(a_stamVal, a_rate, a_pivot);

	if (eq2 <= 0 || eqGrad >= 200) {
		eq1 = maxGradStam - (a_rate * maxGradStam);
		eq2 = a_rate - (a_rate * maxGradStam) + a_pivot;
		eq4 = (a_stamVal - maxGradStam) * 200;
	} else {
		eq1 = (a_stamVal - PlayerStatus::BaseStam) - (a_rate * (a_stamVal - PlayerStatus::BaseStam));
		eq4 = 0;
	}

	float staminaBonus = ((eq1/eq2) * eq3) + eq4;

	return { staminaBonus };
}

float Calc::StaminaWeightBonusCurrent()
{
	return { StaminaWeightBonus(
		PlayerStatus::UpdateAndGetStamAV(), 
		*Settings::Get<float*>("fStaminaWeightRate"), 
		*Settings::Get<uint32_t*>("uStaminaWeightPivot"), 
		*Settings::Get<uint32_t*>("uBaseCarryWeight"),
		PlayerStatus::StamAtMaxGrad
	) };
}

float Calc::GradientAtStamina(float stamVal, float a_rate, uint32_t a_pivot)
{
	float grad1 = (a_rate - 1) * (a_rate + a_pivot);
	float grad2 = a_rate + a_pivot - (a_rate * (stamVal - PlayerStatus::BaseStam));
	float grad = grad1 / float(pow(grad2, 2));

	return { grad };
}

float Calc::LevelWeightBonus(float a_Lvl, float a_rate, uint32_t a_pivot, uint32_t baseCarry, float maxGradLvl)
{
	float eq1;
	float eq2 = a_rate - (a_rate * a_Lvl) + a_pivot;
	float eq3 = baseCarry / 2;
	float eq4;

	float eqGrad = GradientAtLevel(a_Lvl, a_rate, a_pivot, baseCarry);

	// With +ve values of fLevelWeightRate, as level increases, eq2 gets smaller - as it approaches 0, the value of levelBonus grows to infinity
	// At -ve eq2 values, the graph flips and levelBonus remains at a value lower than at a player level of 0
	// Here we just need to check that this hasn't happened (as it entirely depends on the user-set values of fLevelWeightRate and uLevelWeightPivot)
	// And if it has happened, swap to a placeholder growth rate (eq4, 200 weight per level) to stop the whole calculation breaking
	if (eq2 <= 0 || eqGrad > 200) {
		//if (eqGrad > 200) {logger::debug("[eqGrad > 200] at Level {}", a_Lvl);}
		eq1 = maxGradLvl - (a_rate * maxGradLvl);
		eq2 = a_rate - (a_rate * maxGradLvl) + a_pivot;
		eq4 = (a_Lvl - maxGradLvl) * 200;
	} else {
		eq1 = a_Lvl - (a_rate * a_Lvl);
		eq4 = 0;
	}

	//logger::debug("Level = {} | eqGrad = {} | maxGradLvl = {} | eq1 = {} | eq2 = {} | eq3 = {} | eq4 = {}", a_Lvl, eqGrad, maxGradLvl, eq1, eq2, eq3, eq4);

	float levelBonus = ((eq1/eq2) * eq3) + eq4;

	return { levelBonus };
}

float Calc::LevelWeightBonusCurrent()
{
	return { LevelWeightBonus(
		PlayerStatus::UpdateAndGetLevel(), 
		*Settings::Get<float*>("fLevelWeightRate"), 
		*Settings::Get<uint32_t*>("uLevelWeightPivot"), 
		*Settings::Get<uint32_t*>("uBaseCarryWeight"),
		PlayerStatus::LevelAtMaxGrad
	) };
}

float Calc::GradientAtLevel(float a_Lvl, float a_rate, uint32_t a_pivot, uint32_t baseCarry)
{
	float grad1 = baseCarry * (1 - a_rate) * (a_rate + a_pivot);
	float grad2 = a_rate + a_pivot - (a_rate * a_Lvl);
	float grad = grad1 / (2 * float(pow(grad2, 2)));

	return { grad };
}

float* Calc::GetStaminaPlotData(int x_max, float a_rate, uint32_t a_pivot, uint32_t baseCarry, int sampleRate)
{
	std::vector<float> data;
	int i = 0;
	float maxGradStam = PlayerStatus::CalcStamAtMaxGrad(a_rate, a_pivot, baseCarry);

	while (i <= x_max) {
		data.push_back(StaminaWeightBonus(i, a_rate, a_pivot, baseCarry, maxGradStam));
		i += sampleRate;
	}

	float *floatData = &data[0];

	return { floatData };
}