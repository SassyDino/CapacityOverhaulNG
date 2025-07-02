#include "WeightHandler.h"
#include <math.h>

namespace WeightHandler
{
    float Stamina::CalculateStaminaBonus()
    {
        float staminaBonus;
        float playerStaminaAV;
        auto playerBaseStamina = 100; // NOTE: Could do with finding a way of obtaining the DEFAULT stamina value (not base AV), in case any mods make the player's default stamina something other than 100

        auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();

        if (Settings::bTempStaminaAddsWeight) {
            // NOTE: PermanentActorValue doesn't include all buffs (such as some ingredients and blessings), but ActorValue and ClampedActorValue (which do) also consider current stamina (i.e. max stamina - consumed stamina)
            playerStaminaAV = player->GetPermanentActorValue(RE::ActorValue::kStamina);
        } else {
            playerStaminaAV = player->GetBaseActorValue(RE::ActorValue::kStamina);
        }

        if (Settings::bLevelWeightSimple) {
            staminaBonus = (playerStaminaAV - playerBaseStamina) * Settings::fWeightPerStamina;
        } else {
            float eqGrad1 = (Settings::fStaminaWeightRate - 1) * (Settings::fStaminaWeightRate + Settings::uStaminaWeightPivot);
            float eqGrad2 = Settings::fStaminaWeightRate + Settings::uStaminaWeightPivot - (Settings::fStaminaWeightRate * (playerStaminaAV - playerBaseStamina));
            float eqGrad = eqGrad1 / float(pow(eqGrad2, 2));

            float eq1;
            float eq2 = Settings::fStaminaWeightRate - (Settings::fStaminaWeightRate * (playerStaminaAV - playerBaseStamina)) + Settings::uStaminaWeightPivot;
            float eq3 = float(Settings::uBaseWeightLimit) / 4;
            float eq4 = 0;

            if (eq2 <= 0 || eqGrad >= 200) {
                static float sqrt2 = float(sqrt(2));
                float peakStam1 = - ((Settings::fStaminaWeightRate - 1) * (Settings::fStaminaWeightRate + Settings::uStaminaWeightPivot));
                float peakStam2 = (20 * sqrt2 * Settings::fStaminaWeightRate) + (20 * sqrt2 * Settings::uStaminaWeightPivot);
                float peakStam3 = 20 * sqrt2 * Settings::fStaminaWeightRate;
                float peakStam = floor(((sqrt(Settings::uBaseWeightLimit * peakStam1) - peakStam2) / peakStam3) + playerBaseStamina);

                eq1 = peakStam - (Settings::fStaminaWeightRate * peakStam);
                eq2 = Settings::fStaminaWeightRate - (Settings::fStaminaWeightRate * peakStam) + Settings::uStaminaWeightPivot;
                eq4 = (playerStaminaAV - peakStam) * 200;
            } else {
                eq1 = (playerStaminaAV - playerBaseStamina) - (Settings::fLevelWeightRate * (playerStaminaAV - playerBaseStamina));
            }

            staminaBonus = ((eq1/eq2) * eq3) + eq4;
        }

        return staminaBonus;
    }

    float Level::CalculateLevelBonus() 
    {
        float levelBonus;

        auto player = RE::PlayerCharacter::GetSingleton();
        auto playerLevel = player->GetLevel();

        if (Settings::bStaminaWeightSimple) {
            levelBonus = playerLevel * Settings::fWeightPerLevel;
        } else {
            float eqGrad1 = (Settings::fLevelWeightRate - 1) * (Settings::fLevelWeightRate + Settings::uLevelWeightPivot);
            float eqGrad2 = Settings::fLevelWeightRate + Settings::uLevelWeightPivot - (Settings::fLevelWeightRate * playerLevel);
            float eqGrad = eqGrad1 / float(pow(eqGrad2, 2));

            float eq1;
            float eq2 = Settings::fLevelWeightRate - (Settings::fLevelWeightRate * playerLevel) + Settings::uLevelWeightPivot;
            float eq3 = float(Settings::uBaseWeightLimit) / 2;
            float eq4 = 0;

            // With +ve values of fLevelWeightRate, as level increases, eq2 gets smaller - as it approaches 0, the value of levelBonus grows to infinity
            // At -ve eq2 values, the graph flips and levelBonus remains at a value lower than at a player level of 0
            // Here we just need to check that this hasn't happened (as it entirely depends on the user-set values of fLevelWeightRate and uLevelWeightPivot)
            // And if it has happened, swap to a placeholder growth rate (200 weight per level) to stop the whole calculation breaking
            // Probably a tad overcomplicated and excessive, but then again so is this whole mod frankly
            // NOTE: Could probably at some point do with seeing if I can optimise or simplify it, unless it magically works with zero noticeable performance impact
            if (eq2 <= 0 || eqGrad > 200) {
                float peakLvl1 = Settings::uBaseWeightLimit * (1 - Settings::fLevelWeightRate) * (Settings::fLevelWeightRate + Settings::uLevelWeightPivot);
                float peakLvl2 = (20 * Settings::fLevelWeightRate) + (20 * Settings::uLevelWeightPivot);
                float peakLvl = floor(-((sqrt(peakLvl1) - peakLvl2) / (20 * Settings::fLevelWeightRate)));

                eq1 = peakLvl - (Settings::fLevelWeightRate * peakLvl);
                eq2 = Settings::fLevelWeightRate - (Settings::fLevelWeightRate * peakLvl) + Settings::uLevelWeightPivot;
                eq4 = (playerLevel - peakLvl) * 200;
            } else {
                eq1 = playerLevel - (Settings::fLevelWeightRate * playerLevel);
            }

            levelBonus = ((eq1/eq2) * eq3) + eq4;
        }

        return levelBonus;
    }

    float CalculateWeightLimit()
    {
        RE::FormID playerRace = RE::PlayerCharacter::GetSingleton()->GetRace()->GetFormID();
        float raceModifier = Utils::GetRaceWeightMod(playerRace);
        float weightLimit = static_cast<float>(Settings::uBaseWeightLimit);
        float weightLimitInt;

        if (Settings::bStaminaAffectsWeight) {weightLimit += (Stamina::CalculateStaminaBonus() * Settings::fStaminaWeightMod);}
        if (Settings::bLevelAffectsWeight) {weightLimit += (Level::CalculateLevelBonus() * Settings::fLevelWeightMod);}
        if (Settings::bRaceAffectsWeight) {weightLimit *= raceModifier;}

        weightLimitInt = ceil(weightLimit);
        logger::info("Player's carry weight limit calculated as {}, rounded up to {}", weightLimit, weightLimitInt);
        return weightLimitInt;
    }

    void UpdateWeightLimit(std::string a_event)
    {
        logger::info("Recalculating player's carry weight due to {}...", a_event);

        auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
        float playerWeightLimit = CalculateWeightLimit();

        player->SetBaseActorValue(RE::ActorValue::kCarryWeight, playerWeightLimit);
        logger::info("Player's carry weight set to {}", playerWeightLimit);
    }
}