#include "WeightHandler.h"

namespace WeightHandler
{
    int Stamina::GetStaminaBonusMethod()
    {
        if (Settings::bStaminaWeightSimple) {
            return kSimple;
        } else if (Settings::bStaminaWeightDiminish) {
            return kDiminishing;
        } else if (!Settings::bStaminaWeightDiminish) {
            return kIncreasing;
        } else {
            Utils::LogIniError("bStaminaWeightSimple, bStaminaWeightDiminish", 
                "Boolean values expected. Define bStaminaWeightSimple as 'true' or provide a valid bool value to bStaminaWeightDiminish.");
            return kDisabled;
        }
    }

    int Level::GetLevelBonusMethod()
    {
        if (Settings::bLevelWeightSimple) {
            return kSimple;
        } else if (Settings::bLevelWeightDiminish) {
            return kDiminishing;
        } else if (!Settings::bLevelWeightDiminish) {
            return kIncreasing;
        } else {
            Utils::LogIniError("bLevelWeightSimple, bLevelWeightDiminish", 
                "Boolean values expected. Define bLevelWeightSimple as 'true' or provide a valid bool value to bLevelWeightDiminish.");
            return kDisabled;
        }
    }

    float Stamina::CalculateStaminaBonus()
    {
        float staminaBonus;
        int calcMethod = GetStaminaBonusMethod();

        auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
        auto playerPermMaxStamina = player->GetBaseActorValue(RE::ActorValue::kStamina);
        // PermanentActorValue doesn't include all buffs (such as some ingredients and blessings), but ActorValue and ClampedActorValue (which do) also consider current stamina (i.e. max stamina - consumed stamina)
        auto playerTempMaxStamina = player->GetPermanentActorValue(RE::ActorValue::kStamina);
        auto playerBaseStamina = 100; // NOTE: Could do with finding a way of obtaining the DEFAULT stamina value (not base AV), in case any mods make the player's default stamina something other than 100
        float playerStaminaAV;

        if (Settings::bTempStaminaAddsWeight) {
            playerStaminaAV = playerTempMaxStamina;
        } else {
            playerStaminaAV = playerPermMaxStamina;
        }

        if (calcMethod == kSimple) {
            staminaBonus = (playerStaminaAV - playerBaseStamina) * Settings::fWeightPerStamina;
        } else if (calcMethod == kDiminishing) {
            staminaBonus = 0; // Placeholder
        } else if (calcMethod == kIncreasing) {
            staminaBonus = 0; // Placeholder
        } else {
            staminaBonus = 0; // Placeholder
        }

        return staminaBonus;
    }

    float Level::CalculateLevelBonus() 
    {
        float levelBonus;
        int calcMethod = GetLevelBonusMethod();

        auto player = RE::PlayerCharacter::GetSingleton();
        auto playerLevel = player->GetLevel();

        if (calcMethod == kSimple) {
            levelBonus = playerLevel * Settings::fWeightPerLevel;
        } else if (calcMethod == kDiminishing) {
            levelBonus = 0; // Placeholder
        } else if (calcMethod == kIncreasing) {
            levelBonus = 0; // Placeholder
        } else {
            levelBonus = 0; // Placeholder
        }

        return levelBonus;
    }

    float CalculateWeightLimit()
    {
        RE::FormID playerRace = RE::PlayerCharacter::GetSingleton()->GetRace()->GetFormID();
        float raceModifier = Utils::GetRaceWeightMod(playerRace);
        float weightLimit = static_cast<float>(Settings::uBaseWeightLimit);
        float weightLimitInt;

        if (Settings::bStaminaAffectsWeight) {weightLimit += Stamina::CalculateStaminaBonus();}
        if (Settings::bLevelAffectsWeight) {weightLimit += Level::CalculateLevelBonus();}
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