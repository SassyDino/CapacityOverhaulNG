#include "Player.h"

float Player::StamBaseAV;
float Player::StamPermAV;
float Player::StamAV;
int Player::Level;

RE::PlayerCharacter* Player::Char = RE::PlayerCharacter::GetSingleton();
RE::ActorValueOwner* Player::AsAV = Player::Char->AsActorValueOwner();

int Player::ID = 0x14;
RE::TESRace* Player::Race = Player::Char->GetRace();
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

void Player::UpdateStamAtMaxGrad(float a_rate, uint32_t a_pivot)
{
	static float sqrt2 = float(sqrt(2));

	float peakStam1 = - ((a_rate - 1) * (a_rate + a_pivot));
	float peakStam2 = (20 * sqrt2 * a_rate) + (20 * sqrt2 * a_pivot);
	float peakStam3 = 20 * sqrt2 * a_rate;

	StamAtMaxGrad = floor(((sqrt(*Settings::Get<uint32_t*>("uBaseCarryWeight") * peakStam1) - peakStam2) / peakStam3) + Player::BaseStam);
}

void Player::UpdateStamAtMaxGrad()
{
	float rate = *Settings::Get<float*>("fStaminaWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uStaminaWeightPivot");

	UpdateStamAtMaxGrad(rate, pivot);
}

float Player::UpdateAndGetStamAtMaxGrad(float a_rate, uint32_t a_pivot)
{
	UpdateStamAtMaxGrad(a_rate, a_pivot);
	return { StamAtMaxGrad };
}

void Player::UpdateLevelAtMaxGrad(float a_rate, uint32_t a_pivot)
{
	float peakLvl1 = *Settings::Get<uint32_t*>("uBaseCarryWeight") * (1 - a_rate) * (a_rate + a_pivot);
	float peakLvl2 = (20 * a_rate) + (20 * a_pivot);

	LevelAtMaxGrad = floor(-((sqrt(peakLvl1) - peakLvl2) / (20 * a_rate)));
}

void Player::UpdateLevelAtMaxGrad()
{
	float rate = *Settings::Get<float*>("flevelWeightRate");
	uint32_t pivot = *Settings::Get<uint32_t*>("uLevelWeightPivot");

	UpdateLevelAtMaxGrad(rate, pivot);
}