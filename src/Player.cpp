#include "Player.h"

float PlayerStatus::StamBaseAV;
float PlayerStatus::StamPermAV;
float PlayerStatus::StamAV;
int PlayerStatus::Level;
float PlayerStatus::CurrentWeight;
bool PlayerStatus::isOverCapacity = false;

RE::PlayerCharacter* PlayerStatus::Char;
RE::ActorValueOwner* PlayerStatus::AsAV;

int PlayerStatus::ID = 0x14;
RE::TESRace* PlayerStatus::Race;
float PlayerStatus::raceWeightMod;
float PlayerStatus::BaseStam = 100; // NOTE: Could do with finding a way of obtaining the actual DEFAULT stamina value, in case any mods make the player's default stamina something other than 100

float PlayerStatus::StamAtMaxGrad;
float PlayerStatus::LevelAtMaxGrad;

void PlayerStatus::UpdateCapacityStatus(bool overCapacityStatus) { isOverCapacity = overCapacityStatus; }

bool PlayerStatus::GetOverCapacityStatus() { return isOverCapacity; }

void PlayerStatus::UpdateBurden() { CurrentWeight = AsAV->GetActorValue(RE::ActorValue::kInventoryWeight); }

int PlayerStatus::GetBurden() { return CurrentWeight; }

int PlayerStatus::UpdateAndGetBurden()
{
	CurrentWeight = AsAV->GetActorValue(RE::ActorValue::kInventoryWeight);
	return CurrentWeight;
}

void PlayerStatus::UpdateLevel() { Level = Char->GetLevel(); }

int PlayerStatus::GetLevel() { return { Level }; }

int PlayerStatus::UpdateAndGetLevel()
{
	Level = Char->GetLevel();
	return Level;
}

void PlayerStatus::UpdateStamina()
{
	StamPermAV = AsAV->GetPermanentActorValue(RE::ActorValue::kStamina); // NOTE: PermanentActorValue doesn't include all buffs (such as some ingredients and blessings), but ActorValue and ClampedActorValue (which do) also consider current stamina (i.e. max stamina - consumed stamina)
	StamBaseAV = AsAV->GetBaseActorValue(RE::ActorValue::kStamina);

	if (*Settings::Get<bool*>("bTempStaminaAddsWeight")) {
		StamAV = StamPermAV;
	} else {
		StamAV = StamBaseAV;
	}
}

float PlayerStatus::GetStamAV() { return { StamAV }; }

float PlayerStatus::UpdateAndGetStamAV()
{
	PlayerStatus::UpdateStamina();
	return { StamAV };
}

float PlayerStatus::CalcStamAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry)
{
	static float sqrt2 = float(sqrt(2));

	float peakStam1 = - ((a_rate - 1) * (a_rate + a_pivot));
	float peakStam2 = (20 * sqrt2 * a_rate) + (20 * sqrt2 * a_pivot);
	float peakStam3 = 20 * sqrt2 * a_rate;

	float peakStam = floor(((sqrt(baseCarry * peakStam1) - peakStam2) / peakStam3) + PlayerStatus::BaseStam);
	return { peakStam };
}

void PlayerStatus::UpdateStamAtMaxGrad()
{
	float rate = *Settings::Get<float*>("fStaminaWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uStaminaWeightPivot");
	uint32_t baseCarry = *Settings::Get<uint32_t*>("uBaseCarryWeight");

	StamAtMaxGrad = CalcStamAtMaxGrad(rate, pivot, baseCarry);
}

float PlayerStatus::CalcLevelAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry)
{
	float peakLvl1 = baseCarry * (1 - a_rate) * (a_rate + a_pivot);
	float peakLvl2 = (20 * a_rate) + (20 * a_pivot);

	float peakLvl = floor(-((sqrt(peakLvl1) - peakLvl2) / (20 * a_rate)));
	return { peakLvl };
}

void PlayerStatus::UpdateLevelAtMaxGrad()
{
	float rate = *Settings::Get<float*>("fLevelWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uLevelWeightPivot");
	uint32_t baseCarry = *Settings::Get<uint32_t*>("uBaseCarryWeight");

	LevelAtMaxGrad = CalcLevelAtMaxGrad(rate, pivot, baseCarry);
}