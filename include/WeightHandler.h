#pragma once

namespace WeightHandler
{
    class Stamina
    {
        public:
            static float CalculateStaminaBonus();
    };

    class Level
    {
        public:
            static float CalculateLevelBonus();
    };

    float CalculateWeightLimit();

    void UpdateWeightLimit(std::string a_event);
}