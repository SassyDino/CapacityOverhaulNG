#include "BuffsDebuffs.h"
#include "CapacityHandler.h"
#include "Player.h"
#include "FormHandler.h"

bool Debuffs::Current::overDebuffFloor= false;
float Debuffs::Current::speedMod = 0;
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

void Debuffs::EnableWeightEffects()
{
	logger::trace("Enabling Weight Effects");
	RE::DebugNotification("The weight of your inventory is beginning to hinder you.");

	if (Settings::Get<bool>("bWeightAffectsSpeed")) { EnableSpellMGEF(Forms::Global::debuffSpeed); }

	PlayerStatus::Char->RemoveSpell(Forms::Spell::weightDebuff);
	PlayerStatus::Char->AddSpell(Forms::Spell::weightDebuff);
}

void Debuffs::ModifyWeightEffects(float a_debuffAmount)
{
	logger::trace("Modifying Weight Effects -> Base Debuff = {}%", a_debuffAmount);
	if (Settings::Get<bool>("bWeightAffectsSpeed")) { ModifyMoveSpeedMGEF(a_debuffAmount); }

	PlayerStatus::Char->RemoveSpell(Forms::Spell::weightDebuff);
	PlayerStatus::Char->AddSpell(Forms::Spell::weightDebuff);
}

void Debuffs::DisableWeightEffects()
{
	logger::trace("Disabling Weight Effects");
	RE::DebugNotification("The weight of your inventory is no longer a burden.");

	if (Settings::Get<bool>("bWeightAffectsSpeed")) { DisableSpellMGEF(Forms::Global::debuffSpeed); }

	PlayerStatus::Char->RemoveSpell(Forms::Spell::weightDebuff);
	PlayerStatus::Char->AddSpell(Forms::Spell::weightDebuff);
}

void Debuffs::EnableSpellMGEF(RE::TESGlobal* a_global)
{
	//bool wasDisabled = (a_global->value == 0) ? true : false;
	a_global->value = 1;
	logger::trace("Enabling MGEF: '{}'", a_global->GetFormEditorID());
}

void Debuffs::DisableSpellMGEF(RE::TESGlobal* a_global)
{
	//bool wasEnabled = (a_global->value == 1) ? true : false;
	a_global->value = 0;
	
	logger::trace("Disabling MGEF: '{}'", a_global->GetFormEditorID());
}

void Debuffs::ModifyMoveSpeedMGEF(float a_debuffAmount)
{
	auto maxDebuff = Settings::Get<uint32_t>("uSpeedDebuffMax");
	auto debuffMagnitude = a_debuffAmount * (((float)maxDebuff) / 100);

	logger::trace("Base Debuff = {}% | Speed Max Debuff = {}%", a_debuffAmount, maxDebuff);
	logger::trace("Speed Debuff Mag = {}%", debuffMagnitude);

	if (debuffMagnitude >= 100) { debuffMagnitude = 99; }
	Forms::Effect::debuffSpeed->effectItem.magnitude = debuffMagnitude;
/* 
	//NOTE: Need to check that kSpeedMult is the correct value to adjust.
	PlayerStatus::AsAV->RestoreActorValue(RE::ActorValue::kSpeedMult, Current::speedMod);
	auto maxDebuff = Settings::Get<uint32_t>("uSpeedDebuffMax"); // Int between 0-100 (e.g. 80 = 20% movement speed (80% debuff))

	Current::speedMod = (maxDebuff * a_debuffAmount) / 100;
	PlayerStatus::AsAV->DamageActorValue(RE::ActorValue::kSpeedMult, Current::speedMod);
	speedEffect->effectItem.magnitude = Current::speedMod;
*/
}






void Debuffs::ReduceCombatSkill(float a_debuffAmount)
{

}

void Debuffs::ReduceStealth(float a_debuffAmount)
{

}

void Debuffs::IncreaseStamDrain(float a_debuffAmount)
{

}

void Debuffs::ReduceStamRegen(float a_debuffAmount)
{

}

void Debuffs::CapacityEffects()
{
	CapacityHandler::Player::CheckIfOverCapacity();
	if (PlayerStatus::GetOverCapacityStatus()) {
		//TODO: Debuff manager
		logger::debug("Exceeding total capacity.");
	} else {
		logger::debug("Total not exceeding capacity.");
	}
}