#include "Player.h"

float Player::StamBaseAV;
float Player::StamPermAV;
float Player::StamAV;
int Player::Level;

RE::PlayerCharacter* Player::Char;
RE::ActorValueOwner* Player::AsAV;

int Player::ID = 0x14;
RE::TESRace* Player::Race;
float Player::BaseStam = 100;
float Player::StamAtMaxGrad;
float Player::LevelAtMaxGrad;

void Player::UpdateLevel() { Level = Char->GetLevel(); }

int Player::GetLevel() { return { Level }; }

int Player::UpdateAndGetLevel()
{
	Level = Char->GetLevel();
	return { Level };
}

void Player::UpdateStamina()
{
	StamPermAV = AsAV->GetPermanentActorValue(RE::ActorValue::kStamina);
	StamBaseAV = AsAV->GetBaseActorValue(RE::ActorValue::kStamina);

	if (*Settings::Get<bool*>("bTempStaminaAddsWeight")) {
		StamAV = StamPermAV;
	} else {
		StamAV = StamBaseAV;
	}
}

float Player::GetStamAV() { return { StamAV }; }

float Player::UpdateAndGetStamAV()
{
	Player::UpdateStamina();
	return { StamAV };
}

float Player::CalcStamAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry)
{
	static float sqrt2 = float(sqrt(2));

	float peakStam1 = - ((a_rate - 1) * (a_rate + a_pivot));
	float peakStam2 = (20 * sqrt2 * a_rate) + (20 * sqrt2 * a_pivot);
	float peakStam3 = 20 * sqrt2 * a_rate;

	float peakStam = floor(((sqrt(baseCarry * peakStam1) - peakStam2) / peakStam3) + Player::BaseStam);
	return { peakStam };
}

void Player::UpdateStamAtMaxGrad()
{
	float rate = *Settings::Get<float*>("fStaminaWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uStaminaWeightPivot");
	uint32_t baseCarry = *Settings::Get<uint32_t*>("uBaseCarryWeight");

	StamAtMaxGrad = CalcStamAtMaxGrad(rate, pivot, baseCarry);
}

float Player::CalcLevelAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry)
{
	float peakLvl1 = baseCarry * (1 - a_rate) * (a_rate + a_pivot);
	float peakLvl2 = (20 * a_rate) + (20 * a_pivot);

	float peakLvl = floor(-((sqrt(peakLvl1) - peakLvl2) / (20 * a_rate)));
	return { peakLvl };
}

void Player::UpdateLevelAtMaxGrad()
{
	float rate = *Settings::Get<float*>("fLevelWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uLevelWeightPivot");
	uint32_t baseCarry = *Settings::Get<uint32_t*>("uBaseCarryWeight");

	LevelAtMaxGrad = CalcLevelAtMaxGrad(rate, pivot, baseCarry);
}