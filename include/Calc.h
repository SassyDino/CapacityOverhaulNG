#pragma once

namespace Calc
{
	static const float sqrt2 = float(sqrt(2));

	struct Data
	{
		struct Plot
		{
			//TODO: Make these values adjustable
			const static int heatmapMaxLevel = 150;
			const static int heatmapMaxStamina = 300;
			
			static std::vector<float> heatmapData;
			static float heatmapMax;
			static float heatmapMin;
		};
	};

	/**
	 * @brief Uses player-set values to calculate the bonus to carry weight gained from the player's maximum stamina.
	 * @param a_stamVal A max stamina value (typically the player's Actor Value)
	 * @param a_rate A value defining the rate of change in the stamina bonus equation (should be between x and y)
	 * @param a_pivot A value defining the approximate X-axis location of the graph's "knee", i.e. where the growth picks up or drops off
	 * @param baseCarry A value representing the player's base carry weight (i.e. with no adjustments applied), or the minimum possible carry weight
	 * @param maxGradStam A calculated value signifying the stamina value at which the rate of growth becomes excessive (only applies with rate values > 0)
	 * @return 
	 */
	float StaminaWeightBonus(float a_stamVal, float a_rate, uint32_t a_pivot, uint32_t baseCarry, float maxGradStam);
	/**
	 * @brief Calls the function Calc::StaminaWeightBonus, with the player's current settings and stamina value as parameters.
	 * @return 
	 */
	float StaminaWeightBonusCurrent();
	/**
	 * @brief Calculates the gradient at the point calculated by Calc::StaminaWeightBonus
	 * @param a_stamVal A max stamina value (typically the player's Actor Value)
	 * @param a_rate A value defining the rate of change in the stamina bonus equation (should be between x and y)
	 * @param a_pivot A value defining the approximate X-axis location of the graph's "knee", i.e. where the growth picks up or drops off
	 * @return 
	 */
	float GradientAtStamina(float a_stamVal, float a_rate, uint32_t a_pivot);

	float LevelWeightBonus(float a_Lvl, float a_rate, uint32_t a_pivot, uint32_t baseCarry, float maxGradLvl);
	float LevelWeightBonusCurrent();
	float GradientAtLevel(float a_Lvl, float a_rate, uint32_t a_pivot, uint32_t baseCarry);

	void ComputeHeatmapData();

	float* GetStaminaPlotData(int x_max, float a_rate, uint32_t a_pivot, uint32_t baseCarry, int sampleRate);
	float* GetLevelPlotData();
}