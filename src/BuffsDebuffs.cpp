#include "BuffsDebuffs.h"
#include "CapacityHandler.h"
#include "Player.h"
#include "FormHandler.h"

bool Debuffs::Current::overDebuffFloor= false;
float Debuffs::Current::speedMod = 0;

bool Debuffs::Current::enableHands = true;
bool Debuffs::Current::handsEnabled = true;
bool Debuffs::Current::enablePickup = true;
bool Debuffs::Current::pickupEnabled = true;
/* 
std::string_view Debuffs::pluginName = "CapacityOverhaulNG.esp";
RE::TESDataHandler* Debuffs::dataHandler = RE::TESDataHandler::GetSingleton();

RE::TESGlobal* Debuffs::speedGlobal = dataHandler->LookupForm(RE::FormID(0x804), pluginName)->As<RE::TESGlobal>();

RE::SpellItem* Debuffs::weightSpell = dataHandler->LookupForm(RE::FormID(0x802), pluginName)->As<RE::SpellItem>();

RE::Effect* Debuffs::speedEffect = weightSpell->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x803), pluginName)->As<RE::EffectSetting>(), Current::speedMod, 0, 0, 0);
 */

//NOTE: Best first guess, inv weight isn't changing until after the containerchanged event has ran, so its calculating the debuff amount based on old weight?
void Debuffs::CheckWeight()
{
	auto weightLimit = PlayerStatus::AsAV->GetActorValue(RE::ActorValue::kCarryWeight);
	auto currentWeight = PlayerStatus::UpdateAndGetBurden();
	float weightFloor = (float)Settings::Get<uint32_t>("uWeightDebuffFloor");

	if (Settings::Get<bool>("bWeightDebuffFloorIsPercentage")) { weightFloor = weightLimit * (weightFloor / 100); }

	logger::debug("Inv Weight: {} | Weight Limit: {} | Debuff floor: {}", currentWeight, weightLimit, weightFloor);

	// If current inv weight is below the debuff threshold, disable any debuffs (if applied)
	if (currentWeight <= weightFloor) {
		if (Current::overDebuffFloor == true) {
			Current::overDebuffFloor = false;
			DisableWeightEffects();
		}
		return;
	}

	// If over the debuff threshold, and debuffs are not already applied, activate them
	if (Current::overDebuffFloor == false) {
		Current::overDebuffFloor = true;
		EnableWeightEffects();
	}

	// Determine how strong the debuff to be applied will be, then adjust the appropriate effects
	auto debuffRange = weightLimit - weightFloor;
	auto weightOverFloor = currentWeight - weightFloor;
	auto debuffPercentage = (weightOverFloor / debuffRange) * 100;

	ModifyWeightEffects(debuffPercentage);
}

void Debuffs::AddDebuffSpellToPlayer()
{
	PlayerStatus::Char->AddSpell(Forms::Spell::weightDebuff);
	logger::debug("Added weight debuff spell to player.");
}

void Debuffs::RefreshDebuffSpell()
{
	PlayerStatus::Char->RemoveSpell(Forms::Spell::weightDebuff);
	PlayerStatus::Char->AddSpell(Forms::Spell::weightDebuff);
}

void Debuffs::EnableWeightEffects()
{
	logger::trace("Enabling Weight Effects");
	RE::DebugNotification("The weight of your inventory is beginning to hinder you.");

	if (Settings::Get<bool>("bWeightAffectsSpeed")) { EnableSpellMGEF(Forms::Global::debuffSpeed); }
	if (Settings::Get<bool>("bWeightAffectsStealth")) { EnableSpellMGEF(Forms::Global::debuffStealth); }
	if (Settings::Get<bool>("bWeightAffectsStamDrain")) { EnableSpellMGEF(Forms::Global::debuffStamDrain); }
	if (Settings::Get<bool>("bWeightAffectsStamRegen")) { EnableSpellMGEF(Forms::Global::debuffStamRegen); }
	//if (Settings::Get<bool>("bWeightAffectsWeapSpeed")) { EnableSpellMGEF(Forms::Global::debuffWeapSpeed); }
	if (Settings::Get<bool>("bWeightAffectsAttackDmg")) { EnableSpellMGEF(Forms::Global::debuffAttackDmg); }

	RefreshDebuffSpell();
}

void Debuffs::DisableWeightEffects()
{
	logger::trace("Disabling Weight Effects");
	RE::DebugNotification("The weight of your inventory is no longer a burden.");

	if (Settings::Get<bool>("bWeightAffectsSpeed")) { DisableSpellMGEF(Forms::Global::debuffSpeed); }
	if (Settings::Get<bool>("bWeightAffectsStealth")) { DisableSpellMGEF(Forms::Global::debuffStealth); }
	if (Settings::Get<bool>("bWeightAffectsStamDrain")) { DisableSpellMGEF(Forms::Global::debuffStamDrain); }
	if (Settings::Get<bool>("bWeightAffectsStamRegen")) { DisableSpellMGEF(Forms::Global::debuffStamRegen); }
	//if (Settings::Get<bool>("bWeightAffectsWeapSpeed")) { DisableSpellMGEF(Forms::Global::debuffWeapSpeed); }
	if (Settings::Get<bool>("bWeightAffectsAttackDmg")) { DisableSpellMGEF(Forms::Global::debuffAttackDmg); }

	RefreshDebuffSpell();
}

void Debuffs::ModifyWeightEffects(float a_debuffAmount)
{
	logger::trace("Modifying Weight Effects -> Base Debuff = {}%", a_debuffAmount);
	if (Settings::Get<bool>("bWeightAffectsSpeed")) { ModifyMoveSpeedMGEF(a_debuffAmount); }
	if (Settings::Get<bool>("bWeightAffectsStealth")) { ModifyStealthMGEF(a_debuffAmount); }
	if (Settings::Get<bool>("bWeightAffectsStamDrain")) { ModifyStamDrainMGEF(a_debuffAmount); }
	if (Settings::Get<bool>("bWeightAffectsStamRegen")) { ModifyStamRegenMGEF(a_debuffAmount); }
	//if (Settings::Get<bool>("bWeightAffectsWeapSpeed")) { ModifyWeapSpeedMGEF(a_debuffAmount); }
	if (Settings::Get<bool>("bWeightAffectsAttackDmg")) { ModifyAttackDmgMGEF(a_debuffAmount); }

	RefreshDebuffSpell();
}

void Debuffs::EnableSpellMGEF(RE::TESGlobal* a_global)
{
	a_global->value = 1;
	//logger::trace("Enabling MGEF: '{}'", a_global->GetFormEditorID());
}

void Debuffs::DisableSpellMGEF(RE::TESGlobal* a_global)
{
	a_global->value = 0;
	//logger::trace("Disabling MGEF: '{}'", a_global->GetFormEditorID());
}

void Debuffs::ModifyMoveSpeedMGEF(float a_debuffAmount)
{
	auto maxDebuff = Settings::Get<uint32_t>("uSpeedDebuffMax");
	auto debuffMagnitude = (a_debuffAmount * ((float)maxDebuff)) / 100;

	logger::trace("Speed Debuff = -{}%% Speed", debuffMagnitude);

	if (debuffMagnitude >= 100) { debuffMagnitude = 99; }
	Forms::Effect::debuffSpeed->effectItem.magnitude = debuffMagnitude;
}

void Debuffs::ModifyStealthMGEF(float a_debuffAmount)
{
	auto maxDebuff = Settings::Get<uint32_t>("uStealthDebuffMax");
	auto debuffPercent = (a_debuffAmount * maxDebuff) / 100;
	auto debuffMagnitude = debuffPercent / 100;

	logger::trace("Stealth Debuff = +{}% Noise ({})", debuffPercent, debuffMagnitude);

	Forms::Global::debuffStealthDisplay->value = debuffPercent;
	Forms::Effect::debuffStealth->effectItem.magnitude = debuffMagnitude;
}

void Debuffs::ModifyStamDrainMGEF(float a_debuffAmount)
{
	auto maxDebuff = Settings::Get<uint32_t>("uStamDrainDebuffMax");
	auto debuffPercent = (a_debuffAmount * maxDebuff) / 100;
	auto baseStaminaDrain = 7; //NOTE: I'm pretty sure the actual in-game value will be different, depending on armour equipped. But, can't figure out how to find that value anywhere, so this will have to do for now.
	auto debuffMagnitude = baseStaminaDrain * (debuffPercent / 100);

	logger::trace("Stam Drain Debuff = +{}%% Cost (+{}SP/s)", debuffPercent, debuffMagnitude);

	Forms::Global::debuffStamDrainDisplay->value = debuffPercent;
	Forms::Effect::debuffStamDrain->effectItem.magnitude = debuffMagnitude;
}

void Debuffs::ModifyStamRegenMGEF(float a_debuffAmount)
{
	auto maxDebuff = Settings::Get<uint32_t>("uStamRegenDebuffMax");
	auto debuffMagnitude = (a_debuffAmount * (float)maxDebuff) / 100;

	logger::trace("Stam Regen Debuff = -{}%% SP/s", debuffMagnitude);

	Forms::Effect::debuffStamRegen->effectItem.magnitude = debuffMagnitude;
}

void Debuffs::ModifyWeapSpeedMGEF(float a_debuffAmount)
{
	//NOTE: Ok, so cause of how the game handles WeaponSpeedMult, this could potentially break at any point without anyway of really knowing beforehand.
	//? So I'm gonna try and build in a couple of weird workarounds, but I probably need to add a note on the mod page that this may break.
	//TODO: Yeah so I'm gonna sideline this for the time being, one or two playtests and it's already looking to be an absolute nightmare to get
	//TODO  this code working consistently. So will take effect off of debuff spell in ESP for now.
	auto maxDebuff = Settings::Get<uint32_t>("uWeapSpeedDebuffMax");
	auto debuffPercent = (a_debuffAmount * maxDebuff) / 100;

	auto debuffMagnitude = debuffPercent / 100;
	auto debuffResult = 1 - debuffMagnitude;

	auto currentMagnitude = Forms::Effect::debuffWeapSpeed->effectItem.magnitude;
	auto currentAV = PlayerStatus::AsAV->GetActorValue(RE::ActorValue::kWeaponSpeedMult);
	auto currNoDebuffAV = currentAV - currentMagnitude;
	auto newAV = currentAV - debuffMagnitude;

	logger::trace("Weapon Speed Debuff = -{}% | No Debuff AV = {} ({}-{})", debuffPercent, currNoDebuffAV, currentAV, currentMagnitude);

	if ((currentAV == 0) || (currNoDebuffAV == 0)) {
		logger::trace("(currentAV == 0) || (currNoDebuffAV == 0)");
		Forms::Effect::debuffWeapSpeed->effectItem.magnitude = -debuffResult;
	} else if ((currentAV >= 1) && (newAV >= 0)) {
		logger::trace("(currentAV >= 1) && (newAV >= 0)");
		Forms::Effect::debuffWeapSpeed->effectItem.magnitude = debuffMagnitude;
	} else if (currentAV < 1) {
		logger::trace("(currentAV < 1)");
		if ((currNoDebuffAV - debuffMagnitude) > 0) {
			logger::trace("((currNoDebuffAV - debuffMagnitude) > 0)");
			Forms::Effect::debuffWeapSpeed->effectItem.magnitude = debuffMagnitude;
		} else {
			logger::trace("((currNoDebuffAV - debuffMagnitude) <= 0)");
			Forms::Effect::debuffWeapSpeed->effectItem.magnitude = currNoDebuffAV - 0.01f;
		}
	} else {
		logger::trace("(currentAV >= 1) && (newAV < 0)");
		Forms::Effect::debuffWeapSpeed->effectItem.magnitude = 0;
		logger::error("Issue with calculating a valid debuff value to assign to MGEF 'CONG_WeightDebuffEffect_WeapSpeed', which modifies Actor Value 'WeaponSpeedMult'. Most likely cause is other effects altering WeaponSpeedMult in a way that this mod has not been set up to compensate for. It is recommended to disable the 'Weight debuffs weapon speed' setting in this mod's config.");
	}

	Forms::Global::debuffWeapSpeedDisplay->value = debuffPercent;
}

void Debuffs::ModifyAttackDmgMGEF(float a_debuffAmount)
{
	auto maxDebuff = Settings::Get<uint32_t>("uAttackDmgDebuffMax");
	auto debuffPercent = (a_debuffAmount * ((float)maxDebuff)) / 100;
	auto debuffMagnitude = debuffPercent / 100;

	logger::trace("Attack Damage Debuff = -{}%", debuffPercent);

	Forms::Global::debuffAttackDmgDisplay->value = debuffPercent;
	Forms::Effect::debuffAttackDmg->effectItem.magnitude = debuffMagnitude;
}

void Debuffs::CapacityEffects()
{
	CapacityHandler::Player::CheckIfOverCapacity();

	if (PlayerStatus::GetOverCapacityStatus()) {
		logger::debug("Exceeding total capacity.");

		Current::enableHands = (Settings::Get<bool>("bNoHandsOverCap")) ? false : true;
		Current::enablePickup = (Settings::Get<bool>("bPreventPickupOverCap")) ? false : true;
	} else {
		logger::debug("Total not exceeding capacity.");

		Current::enableHands = true;
		Current::enablePickup = true;
	}

	CheckCapacityDebuffStatus();
}

void Debuffs::CheckCapacityDebuffStatus()
{
	if (Current::enableHands && !Current::handsEnabled) {
		EnableHands();
	} else if (!Current::enableHands && Current::handsEnabled) {
		DisableHands();
	}

	if (Current::enablePickup && !Current::pickupEnabled) {
		EnablePickup();
	} else if (!Current::enablePickup && Current::pickupEnabled) {
		DisablePickup();
	}
}

void Debuffs::EnableHands()
{
	logger::trace("Enabling player's hands.");
	PlayerStatus::Controls->readyWeaponHandler->SetInputEventHandlingEnabled(true);
	Current::handsEnabled = true;
}

void Debuffs::DisableHands()
{
	logger::trace("Disabling player's hands.");

	switch (PlayerStatus::State->GetWeaponState())
	{
		case RE::WEAPON_STATE::kDrawing:
		case RE::WEAPON_STATE::kDrawn:
		case RE::WEAPON_STATE::kWantToDraw:
		case RE::WEAPON_STATE::kWantToSheathe:
			PlayerStatus::Char->AsActorState()->actorState2.weaponState = RE::WEAPON_STATE::kWantToSheathe;
			break;
		default:
			break;
	}

	PlayerStatus::Controls->readyWeaponHandler->SetInputEventHandlingEnabled(false);
	Current::handsEnabled = false;
}

void Debuffs::EnablePickup()
{
	logger::trace("Enabling pickup.");
}

void Debuffs::DisablePickup()
{
	logger::trace("Disabling pickup.");
}