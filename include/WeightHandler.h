#pragma once

namespace WeightHandler
{
    class Stamina
    {
        enum CalcMethod {
            kDisabled,
            kSimple,
            kIncreasing,
            kDiminishing
        };

        public:
            static int GetStaminaBonusMethod();
    
            static float CalculateStaminaBonus();
    };

    class Level
    {
        enum CalcMethod {
            kDisabled,
            kSimple,
            kIncreasing,
            kDiminishing
        };

        public:
            static int GetLevelBonusMethod();
    
            static float CalculateLevelBonus();
    };

    float CalculateWeightLimit();

    void UpdateWeightLimit(std::string a_event);
}