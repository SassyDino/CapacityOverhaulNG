#include "FormHandler.h"

const std::string_view Forms::pluginName = "CapacityOverhaulNG.esp";
RE::TESDataHandler* Forms::dataHandler;
std::optional<std::uint8_t> Forms::rawModIndex;
std::pair<uint32_t, uint32_t> Forms::modIndex;

RE::TESGlobal* Forms::Global::debuffSpeed;

RE::SpellItem* Forms::Spell::weightDebuff;

RE::Effect* Forms::Effect::debuffSpeed;

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

	Effect::debuffSpeed = Spell::weightDebuff->GetEffectIsMatch(dataHandler->LookupForm(RE::FormID(0x803), pluginName)->As<RE::EffectSetting>(), 0, 0, 0, 0);

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