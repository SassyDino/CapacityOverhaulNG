#include "WeightHandler.h"
#include <math.h>

namespace WeightHandler
{
	float Stamina::playerBaseStamina = 100;	// NOTE: Could do with finding a way of obtaining the DEFAULT stamina value (not base AV), in case any mods make the player's default stamina something other than 100
	float Stamina::playerStamAV;
	float Stamina::stamAtMaxGrad;

	float Level::playerLevel;
	float Level::levelAtMaxGrad;

	void Stamina::UpdateStaminaAV()
	{
		auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();

		if (Settings::bTempStaminaAddsWeight) {
            // NOTE: PermanentActorValue doesn't include all buffs (such as some ingredients and blessings), but ActorValue and ClampedActorValue (which do) also consider current stamina (i.e. max stamina - consumed stamina)
            playerStamAV = player->GetPermanentActorValue(RE::ActorValue::kStamina);
        } else {
            playerStamAV = player->GetBaseActorValue(RE::ActorValue::kStamina);
        }
	}

	float Stamina::CalculateGradient()
	{
		float eqGrad1 = (Settings::fStaminaWeightRate - 1) * (Settings::fStaminaWeightRate + Settings::uStaminaWeightPivot);
        float eqGrad2 = Settings::fStaminaWeightRate + Settings::uStaminaWeightPivot - (Settings::fStaminaWeightRate * (playerStamAV - playerBaseStamina));
        float eqGrad = eqGrad1 / float(pow(eqGrad2, 2));

		return eqGrad;
	}

	void Stamina::GetStamAtMaxGrad()
	{
		static float sqrt2 = float(sqrt(2));
		float peakStam1 = - ((Settings::fStaminaWeightRate - 1) * (Settings::fStaminaWeightRate + Settings::uStaminaWeightPivot));
		float peakStam2 = (20 * sqrt2 * Settings::fStaminaWeightRate) + (20 * sqrt2 * Settings::uStaminaWeightPivot);
		float peakStam3 = 20 * sqrt2 * Settings::fStaminaWeightRate;
		stamAtMaxGrad = floor(((sqrt(Settings::uBaseWeightLimit * peakStam1) - peakStam2) / peakStam3) + playerBaseStamina);
	}

    float Stamina::CalculateStaminaBonus()
    {
        float staminaBonus;

		Stamina::UpdateStaminaAV();

        if (Settings::bLevelWeightSimple) {
            staminaBonus = (playerStamAV - playerBaseStamina) * Settings::fWeightPerStamina;
        } else {
            float eq1;
            float eq2 = Settings::fStaminaWeightRate - (Settings::fStaminaWeightRate * (playerStamAV - playerBaseStamina)) + Settings::uStaminaWeightPivot;
            float eq3 = float(Settings::uBaseWeightLimit) / 4;
            float eq4;

			float eqGrad = Stamina::CalculateGradient();

            if (eq2 <= 0 || eqGrad >= 200) {
                eq1 = stamAtMaxGrad - (Settings::fStaminaWeightRate * stamAtMaxGrad);
                eq2 = Settings::fStaminaWeightRate - (Settings::fStaminaWeightRate * stamAtMaxGrad) + Settings::uStaminaWeightPivot;
                eq4 = (playerStamAV - stamAtMaxGrad) * 200;
            } else {
                eq1 = (playerStamAV - playerBaseStamina) - (Settings::fLevelWeightRate * (playerStamAV - playerBaseStamina));
				eq4 = 0;
            }

            staminaBonus = ((eq1/eq2) * eq3) + eq4;
        }

        return staminaBonus;
    }

	float Level::CalculateGradient()
	{
		float eqGrad1 = (Settings::fLevelWeightRate - 1) * (Settings::fLevelWeightRate + Settings::uLevelWeightPivot);
		float eqGrad2 = Settings::fLevelWeightRate + Settings::uLevelWeightPivot - (Settings::fLevelWeightRate * playerLevel);
		float eqGrad = eqGrad1 / float(pow(eqGrad2, 2));

		return eqGrad;
	}

	void Level::GetLevelAtMaxGrad()
	{
		float peakLvl1 = Settings::uBaseWeightLimit * (1 - Settings::fLevelWeightRate) * (Settings::fLevelWeightRate + Settings::uLevelWeightPivot);
		float peakLvl2 = (20 * Settings::fLevelWeightRate) + (20 * Settings::uLevelWeightPivot);
		levelAtMaxGrad = floor(-((sqrt(peakLvl1) - peakLvl2) / (20 * Settings::fLevelWeightRate)));
	}

    float Level::CalculateLevelBonus() 
    {
        float levelBonus;

        auto player = RE::PlayerCharacter::GetSingleton();
        playerLevel = player->GetLevel();

        if (Settings::bStaminaWeightSimple) {
            levelBonus = playerLevel * Settings::fWeightPerLevel;
        } else {
            float eq1;
            float eq2 = Settings::fLevelWeightRate - (Settings::fLevelWeightRate * playerLevel) + Settings::uLevelWeightPivot;
            float eq3 = float(Settings::uBaseWeightLimit) / 2;
            float eq4;

			float eqGrad = CalculateGradient();

            // With +ve values of fLevelWeightRate, as level increases, eq2 gets smaller - as it approaches 0, the value of levelBonus grows to infinity
            // At -ve eq2 values, the graph flips and levelBonus remains at a value lower than at a player level of 0
            // Here we just need to check that this hasn't happened (as it entirely depends on the user-set values of fLevelWeightRate and uLevelWeightPivot)
            // And if it has happened, swap to a placeholder growth rate (eq4, 200 weight per level) to stop the whole calculation breaking
            // Probably a tad overcomplicated and excessive, but then again so is this whole mod frankly
            //NOTE: Could probably at some point do with seeing if I can optimise or simplify it, unless it magically works with zero noticeable performance impact
            if (eq2 <= 0 || eqGrad > 200) {
                eq1 = levelAtMaxGrad - (Settings::fLevelWeightRate * levelAtMaxGrad);
                eq2 = Settings::fLevelWeightRate - (Settings::fLevelWeightRate * levelAtMaxGrad) + Settings::uLevelWeightPivot;
                eq4 = (playerLevel - levelAtMaxGrad) * 200;
            } else {
                eq1 = playerLevel - (Settings::fLevelWeightRate * playerLevel);
				eq4 = 0;
            }

            levelBonus = ((eq1/eq2) * eq3) + eq4;
        }

        return levelBonus;
    }

	void UpdateFromSettings()
	{
		Stamina::GetStamAtMaxGrad();
		Level::GetLevelAtMaxGrad();
	}

    float CalculateWeightLimit()
    {
        RE::FormID playerRace = RE::PlayerCharacter::GetSingleton()->GetRace()->GetFormID();
        float raceModifier = Utils::GetRaceWeightMod(playerRace);	//NOTE: Could be possible to only make this fire if/when the player changes race: check out TESSwitchRaceCompleteEvent, see whether it fires even from racemenu, console etc.
        float weightLimit = static_cast<float>(Settings::uBaseWeightLimit);

        if (Settings::bStaminaAffectsWeight) {weightLimit += (Stamina::CalculateStaminaBonus() * Settings::fStaminaWeightMod);}
        if (Settings::bLevelAffectsWeight) {weightLimit += (Level::CalculateLevelBonus() * Settings::fLevelWeightMod);}
        if (Settings::bRaceAffectsWeight) {weightLimit *= raceModifier;}

        float weightLimitInt = ceil(weightLimit);
        logger::info("Player's carry weight limit calculated as {}, rounded up to {}", weightLimit, weightLimitInt);
        return weightLimitInt;
    }

    void UpdateWeightLimit()
    {
        auto player = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
        float playerWeightLimit = CalculateWeightLimit();

        player->SetBaseActorValue(RE::ActorValue::kCarryWeight, playerWeightLimit);
    }
}