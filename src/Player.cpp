#include "Player.h"
#include "Hooks.h"

float PlayerState::StamBaseAV;
float PlayerState::StamPermAV;
float PlayerState::StamAV;
int PlayerState::Level;
float PlayerState::CurrentWeight;
bool PlayerState::isOverCapacity = false;

RE::PlayerCharacter* PlayerState::Char;
RE::ActorValueOwner* PlayerState::AsAV;
RE::PlayerControls* PlayerState::Controls;
RE::ActorState* PlayerState::State;

int PlayerState::ID = 0x14;
RE::TESRace* PlayerState::Race;
float PlayerState::raceWeightMod = 1.0f;
float PlayerState::BaseStam = 100; // NOTE: Could do with finding a way of obtaining the actual DEFAULT stamina value, in case any mods make the player's default stamina something other than 100

float PlayerState::StamAtMaxGrad;
float PlayerState::LevelAtMaxGrad;

void PlayerState::UpdateCapacityStatus(bool overCapacityStatus) { isOverCapacity = overCapacityStatus; }

bool PlayerState::GetOverCapacityStatus() { return isOverCapacity; }

void PlayerState::UpdateBurden() { CurrentWeight = AsAV->GetActorValue(RE::ActorValue::kInventoryWeight); }

int PlayerState::GetBurden() { return (int)ceil(CurrentWeight); }

int PlayerState::UpdateAndGetBurden()
{
	CurrentWeight = AsAV->GetActorValue(RE::ActorValue::kInventoryWeight);
	return (int)ceil(CurrentWeight);
}

void PlayerState::UpdateLevel() { Level = Char->GetLevel(); }

int PlayerState::GetLevel() { return { Level }; }

int PlayerState::UpdateAndGetLevel()
{
	Level = Char->GetLevel();
	return Level;
}

void PlayerState::UpdateStamina()
{
	StamPermAV = AsAV->GetPermanentActorValue(RE::ActorValue::kStamina); // NOTE: PermanentActorValue doesn't include all buffs (such as some ingredients and blessings), but ActorValue and ClampedActorValue (which do) also consider current stamina (i.e. max stamina - consumed stamina)
	StamBaseAV = AsAV->GetBaseActorValue(RE::ActorValue::kStamina);

	if (*Settings::Get<bool*>("bTempStaminaAddsWeight")) {
		StamAV = StamPermAV;
	} else {
		StamAV = StamBaseAV;
	}
}

float PlayerState::GetStamAV() { return { StamAV }; }

float PlayerState::UpdateAndGetStamAV()
{
	PlayerState::UpdateStamina();
	return { StamAV };
}

float PlayerState::CalcStamAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry)
{
	static float sqrt2 = float(sqrt(2));

	float peakStam1 = - ((a_rate - 1) * (a_rate + a_pivot));
	float peakStam2 = (20 * sqrt2 * a_rate) + (20 * sqrt2 * a_pivot);
	float peakStam3 = 20 * sqrt2 * a_rate;

	float peakStam = floor(((sqrt(baseCarry * peakStam1) - peakStam2) / peakStam3) + PlayerState::BaseStam);
	return { peakStam };
}

void PlayerState::UpdateStamAtMaxGrad()
{
	float rate = Settings::Get<float>("fStaminaWeightRate");
	uint32_t pivot = Settings::Get<uint32_t>("uStaminaWeightPivot");
	uint32_t baseCarry = Settings::Get<uint32_t>("uBaseCarryWeight");

	StamAtMaxGrad = CalcStamAtMaxGrad(rate, pivot, baseCarry);
}

float PlayerState::CalcLevelAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry)
{
	float peakLvl1 = baseCarry * (1 - a_rate) * (a_rate + a_pivot);
	float peakLvl2 = (20 * a_rate) + (20 * a_pivot);

	float peakLvl = floor(-((sqrt(peakLvl1) - peakLvl2) / (20 * a_rate)));
	return { peakLvl };
}

void PlayerState::UpdateLevelAtMaxGrad()
{
	float rate = Settings::Get<float>("fLevelWeightRate");
	uint32_t pivot = Settings::Get<uint32_t>("uLevelWeightPivot");
	uint32_t baseCarry = Settings::Get<uint32_t>("uBaseCarryWeight");

	LevelAtMaxGrad = CalcLevelAtMaxGrad(rate, pivot, baseCarry);
}

bool PlayerState::AllowActivation()
{
	logger::debug("Calling <AllowActivation>");

	if (!Hooks::crosshair_ref) { return true; }

	if (const auto a_targetRef = Hooks::crosshair_ref->GetBaseObject()) {
		logger::trace("Crosshair object found. Checking whether activation should be allowed.");
		bool allow = true;

		switch (a_targetRef->GetFormType())
		{
			case RE::FormType::NPC: {
				logger::trace("NPC Activation");
				auto npc = Hooks::crosshair_ref->As<RE::Actor>();
				if (npc->IsDead() && Settings::Get<bool>("bNoContainerAccessOverCap")) {
					RE::DebugNotification("You cannot loot corpses while your inventory is overflowing.");
					allow = false;
				}
				break;
			}
			case RE::FormType::Container:
				logger::trace("Container Activation");
				if (Settings::Get<bool>("bNoContainerAccessOverCap")) {
					RE::DebugNotification("You cannot access containers while your inventory is overflowing.");
					allow = false;
				}
				break;
			default:
				//Todo: "Everything else" might include stuff like flora etc, need to make sure this only works when intended, and works everywhere I want it to
				logger::trace("Item Activation");
				RE::DebugNotification("Your storage is too full to hold any more items.");
				allow = false;
		}
		return allow;
	}
	return true;
}
/* 
RE::FormType PlayerState::CrosshairFormType()
{
	auto crosshairObj = GetCrosshairObj();

	if (const auto objRef = crosshairObj->GetBaseObject()) {
		return objRef->GetFormType();
	} else {
		return RE::FormType::None;
	}
}

bool PlayerState::LookingAtContainer()
{
	bool container = false;
	auto objType =  CrosshairFormType();

	if (objType == RE::FormType::None) { return container; }

	switch (objType)
	{
		//TODO: Might need to add another case for certain activators, I think some can add items/open an inventory upon activation?
		case RE::FormType::Container:
			container = true;
			break;
			//TODO: Check whether bookshelves count as a container
		case RE::FormType::NPC:
			if (GetCrosshairObj()->As<RE::Actor>()->IsDead()) {
				container = true;
			}
			break;
			//TODO: Check how followers, pickpocketing, and crafting stations work
		default:
			break;
	}

	return container;
}

bool PlayerState::LookingAtItem()
{
	bool item = false;
	auto objType = CrosshairFormType();

	//auto obj = CrosshairRef->As

	return false;
}
 */