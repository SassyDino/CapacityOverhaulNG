#include "FormHandler.h"

const std::string_view Forms::pluginName = "CapacityOverhaulNG.esp";
RE::TESDataHandler* Forms::dataHandler;
std::optional<std::uint8_t> Forms::rawModIndex;
std::pair<uint32_t, uint32_t> Forms::modIndex;

RE::SpellItem* Forms::Spell::weightDebuff;

RE::TESGlobal* Forms::Global::debuffSpeed;
RE::TESGlobal* Forms::Global::debuffStealth;
RE::TESGlobal* Forms::Global::debuffStamDrain;
RE::TESGlobal* Forms::Global::debuffStamRegen;
RE::TESGlobal* Forms::Global::debuffWeapSpeed;
RE::TESGlobal* Forms::Global::debuffAttackDmg;
			
RE::TESGlobal* Forms::Global::debuffStealthDisplay;
RE::TESGlobal* Forms::Global::debuffStamDrainDisplay;
RE::TESGlobal* Forms::Global::debuffWeapSpeedDisplay;
RE::TESGlobal* Forms::Global::debuffAttackDmgDisplay;

RE::Effect* Forms::Effect::debuffSpeed;
RE::Effect* Forms::Effect::debuffStealth;
RE::Effect* Forms::Effect::debuffStamDrain;
RE::Effect* Forms::Effect::debuffStamRegen;
RE::Effect* Forms::Effect::debuffWeapSpeed;
RE::Effect* Forms::Effect::debuffAttackDmg;

RE::FormID Forms::KYWD::CONG_CoinItem;

RE::FormID Forms::KYWD::VendorItemPotion = 0x8CDEC;
RE::FormID Forms::KYWD::VendorItemPoison = 0x8CDED;
RE::FormID Forms::KYWD::VendorItemGem = 0x914ED;
RE::FormID Forms::KYWD::ArmorShield = 0x965B2;

RE::FormID Forms::MISC::BYOHMaterialLog = 0x300300E;

void Forms::LoadFromGame()
{
	dataHandler = RE::TESDataHandler::GetSingleton();
	rawModIndex = dataHandler->GetLoadedLightModIndex(pluginName);
	uint32_t tempIndex = (rawModIndex.value() + 0xFE000) * 0x1000;
	modIndex = {tempIndex, tempIndex + 0xFFF};
	logger::debug("modIndex Range = 0x{:X} - 0x{:X}", modIndex.first, modIndex.second);
	logger::debug("TESDataHandler found ---> Locating forms...");

	Spell::weightDebuff = dataHandler->LookupForm(RE::FormID(0x802), pluginName)->As<RE::SpellItem>();

	Global::debuffSpeed = dataHandler->LookupForm(RE::FormID(0x804), pluginName)->As<RE::TESGlobal>();
	Global::debuffStealth = dataHandler->LookupForm(RE::FormID(0x805), pluginName)->As<RE::TESGlobal>();
	Global::debuffStamDrain = dataHandler->LookupForm(RE::FormID(0x806), pluginName)->As<RE::TESGlobal>();
	Global::debuffStamRegen = dataHandler->LookupForm(RE::FormID(0x807), pluginName)->As<RE::TESGlobal>();
	Global::debuffWeapSpeed = dataHandler->LookupForm(RE::FormID(0x80F), pluginName)->As<RE::TESGlobal>();
	Global::debuffAttackDmg = dataHandler->LookupForm(RE::FormID(0x811), pluginName)->As<RE::TESGlobal>();
			
	Global::debuffStealthDisplay = dataHandler->LookupForm(RE::FormID(0x809), pluginName)->As<RE::TESGlobal>();
	Global::debuffStamDrainDisplay = dataHandler->LookupForm(RE::FormID(0x80C), pluginName)->As<RE::TESGlobal>();
	Global::debuffWeapSpeedDisplay = dataHandler->LookupForm(RE::FormID(0x810), pluginName)->As<RE::TESGlobal>();
	Global::debuffAttackDmgDisplay = dataHandler->LookupForm(RE::FormID(0x813), pluginName)->As<RE::TESGlobal>();

	Effect::debuffSpeed = Spell::weightDebuff->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x803), pluginName)->As<RE::EffectSetting>(), 0, 0, 0, 0);
	Effect::debuffStealth = Spell::weightDebuff->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x808), pluginName)->As<RE::EffectSetting>(), 0, 0, 0, 0);
	Effect::debuffStamDrain = Spell::weightDebuff->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x80A), pluginName)->As<RE::EffectSetting>(), 0, 0, 0, 0);
	Effect::debuffStamRegen = Spell::weightDebuff->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x80B), pluginName)->As<RE::EffectSetting>(), 0, 0, 0, 0);
	Effect::debuffWeapSpeed = Spell::weightDebuff->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x80E), pluginName)->As<RE::EffectSetting>(), 0, 0, 0, 0);
	Effect::debuffAttackDmg = Spell::weightDebuff->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x812), pluginName)->As<RE::EffectSetting>(), 0, 0, 0, 0);

	KYWD::CONG_CoinItem = dataHandler->LookupFormID(0x801, pluginName);

	logger::debug("All forms located.\n{}", std::string(100, '='));
}

bool Forms::IsCONGForm(uint32_t a_formID)
{
	if ((a_formID < modIndex.first) || (a_formID > modIndex.second)) {
		return false;
	} else {
		return true;
	}
}