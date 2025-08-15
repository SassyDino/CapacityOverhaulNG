#pragma once

namespace WeightHandler
{
    class Stamina
    {
		static float playerBaseStamina;
		static float playerStamAV;
		static float stamAtMaxGrad;
		
        public:
			static void UpdateStaminaAV();
			static float CalculateGradient();
			static void GetStamAtMaxGrad();
            static float CalculateStaminaBonus();
    };

    class Level
    {
		static float playerLevel;
		static float levelAtMaxGrad;

        public:
			static float CalculateGradient();
			static void GetLevelAtMaxGrad();
            static float CalculateLevelBonus();
    };

	void UpdateFromSettings();

    float CalculateWeightLimit();

    void UpdateWeightLimit();
}