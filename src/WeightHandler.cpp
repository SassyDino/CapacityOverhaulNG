#include "WeightHandler.h"
#include "Player.h"
#include "Calc.h"
#include <math.h>
#include "BuffsDebuffs.h"
#include <ClibUtil/timer.hpp>

namespace WeightHandler
{
	void GetRaceWeightMod()
    {
        std::map<RE::FormID, const char*> raceWeightIndex {
            {0x13743, "fAltmerRaceMod"}, {0x13740, "fArgonianRaceMod"}, {0x13749, "fBosmerRaceMod"},
            {0x13741, "fBretonRaceMod"}, {0x13742, "fDunmerRaceMod"}, {0x13744, "fImperialRaceMod"},
            {0x13745, "fKhajiitRaceMod"}, {0x13746, "fNordRaceMod"}, {0x13747, "fOrcRaceMod"},
            {0x13748, "fRedguardRaceMod"}
        };

		// Use default race modifier as a fallback if player's race does not exist in index (to prevent custom races breaking this mod)
        PlayerStatus::raceWeightMod = *Settings::Get<float*>("fDefaultRaceMod");
		auto raceID = PlayerStatus::Race->GetFormID();
        
        if (raceWeightIndex.at(raceID)) {
            PlayerStatus::raceWeightMod = *Settings::Get<float*>(raceWeightIndex[raceID]);
            auto raceName = PlayerStatus::Race->GetName();
            logger::info("Player race identified as {}. Carry weight limit modifier = x{}", raceName, PlayerStatus::raceWeightMod);
        } else {
			//TODO: If/when I fix whatever's broken here, move the "raceWeightMod = fDefaultRaceMod" definiton into here
			logger::warn("Unable to identify player race: reverting to Default race modifier (x{}). A custom player race is the most likely cause for this warning, and if this applies to you then you may ignore this message. Otherwise, this warning may indicate an issue with the mod which you are advised to report.", PlayerStatus::raceWeightMod);
		}

        // if i need more values stored per race, maybe try something like
        // Settings::raceIndex[formID]->WeightMod()
        // with the index's values being classes/structs or something
    }

	void UpdateFromSettings()
	{
		GetRaceWeightMod(); //NOTE: Check out TESSwitchRaceCompleteEvent, see whether it fires even from racemenu, console etc.

		PlayerStatus::UpdateStamAtMaxGrad();
		PlayerStatus::UpdateLevelAtMaxGrad();
	}

    float CalculateWeightLimit()
    {
		clib_util::Timer timer;
		timer.start();
        float weightLimit = static_cast<float>(*Settings::Get<uint32_t*>("uBaseCarryWeight"));

        if (*Settings::Get<bool*>("bStaminaAffectsWeight")) {
			weightLimit += (Calc::StaminaWeightBonusCurrent() * *Settings::Get<float*>("fStaminaWeightMod"));
		}
        if (*Settings::Get<bool*>("bLevelAffectsWeight")) {
			weightLimit += (Calc::LevelWeightBonusCurrent() * *Settings::Get<float*>("fLevelWeightMod"));
		}
        if (*Settings::Get<bool*>("bRaceAffectsWeight")) {
			weightLimit *= PlayerStatus::raceWeightMod;
		}

        float weightLimitRound = ceil(weightLimit);
		timer.stop();

        logger::debug("Player's carry weight limit calculated as {}, rounded up to {}. Time taken: {}μs / {}ms", weightLimit, weightLimitRound, timer.duration_μs(), timer.duration_ms());
        return { weightLimitRound };
    }

    void UpdateWeightLimit()
    {
        float playerWeightLimit = CalculateWeightLimit();

        PlayerStatus::AsAV->SetBaseActorValue(RE::ActorValue::kCarryWeight, playerWeightLimit);

		Debuffs::CheckWeight();
    }
}