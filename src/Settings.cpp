void Settings::Init()
{   
    constexpr auto defaultPath = L"Data/MCM/Config/CapacityOverhaulNG/settings.ini";
    constexpr auto mcmPath = L"Data/MCM/Settings/CapacityOverhaulNG.ini";
    bool validSettings;

    logger::info("Initialising settings");

    logger::info("Loading default settings...");
    Load(defaultPath);
    logger::info("...done");

    logger::info("Loading user settings...");
    Load(mcmPath);
    logger::info("...done");

    logger::info("Validating settings...");
    validSettings = Validate();

    if (validSettings == true) {
        logger::info("...settings validated.");
        logger::info("Initialisation success!");
    } else {
        logger::info("...failed to validate settings.");
        logger::info("Initialisation complete. Mod may not function as intended.");
        auto logErrorBox = std::format("Capacity Overhaul NG: An error occured while validating this mod's settings. This error suggests that an incorrect or invalid value has been provided in the mod's INI file or MCM. Continuing without rectifying this issue may result in unexpected mod behaviour. Refer to the log file for more information (My Games/Skyrim Special Edition/SKSE/CapacityOverhaulNG.log).");
        RE::DebugMessageBox(logErrorBox.c_str());
    }

	Utils::UpdateModules();
}

bool Settings::Validate()
{
    bool validSettings = true;

    if (fSmallItemWeight >= fMediumItemWeight || fSmallItemWeight >= fLargeItemWeight) {
        validSettings = false;
        logger::error("User setting 'fSmallItemWeight' (Small Item Minimum Weight) must be smaller than 'fMediumItemWeight' and 'fLargeItemWeight' (Medium/Large Item Minimum Weight).");
    }
    if (fMediumItemWeight >= fLargeItemWeight) {
        validSettings = false;
        logger::error("User setting 'fMediumItemWeight' (Medium Item Minimum Weight) must be smaller than 'fLargeItemWeight' (Large Item Minimum Weight).");
    }
    if (!bStaminaWeightSimple) {
        if (fStaminaWeightRate >= 1 || fStaminaWeightRate < -1) {
            validSettings = false;
            logger::error("User setting 'fStaminaWeightRate' must be a value between -1 and 1 (Note: A value of 1 will also return an error).");
        }
    }
    if (!bLevelWeightSimple) {
        if (fLevelWeightRate >= 1 || fLevelWeightRate < -1) {
            validSettings = false;
            logger::error("User setting 'fLevelWeightRate' must be a value between -1 and 1 (Note: A value of 1 will also return an error).");
        }
    }

    return validSettings;
}

/*
void Settings::Load(std::filesystem::path path)
{
    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path.string().c_str());

    // Toggle features
    ReadBool(ini, "ToggleFeatures", "bNoHandsOverCap", bNoHandsOverCap);
    ReadBool(ini, "ToggleFeatures", "bPreventPickupOverCap", bPreventPickupOverCap);
    ReadBool(ini, "ToggleFeatures", "bSkillsAffectCapacity", bSkillsAffectCapacity);
    ReadBool(ini, "ToggleFeatures", "bVanillaWeightLimit", bVanillaWeightLimit);
    ReadBool(ini, "ToggleFeatures", "bStaminaAffectsWeight", bStaminaAffectsWeight);
    ReadBool(ini, "ToggleFeatures", "bLevelAffectsWeight", bLevelAffectsWeight);
    ReadBool(ini, "ToggleFeatures", "bRaceAffectsWeight", bRaceAffectsWeight);

    // Base value modifiers
    ReadUInt32(ini, "BaseValueModifiers", "fLargeCapacity", uLargeCapacity);
    ReadFloat(ini, "BaseValueModifiers", "fMediumPerLarge", fMediumPerLarge);
    ReadFloat(ini, "BaseValueModifiers", "fSmallPerMedium", fSmallPerMedium);
    ReadFloat(ini, "BaseValueModifiers", "fTinyPerSmall", fTinyPerSmall);
    ReadUInt32(ini, "BaseValueModifiers", "fAlchemyCapacity", uAlchemyCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "fAmmoCapacity", uAmmoCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "fCoinCapacity", uCoinCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "uCoinsPerTiny", uCoinsPerTiny);
    ReadFloat(ini, "BaseValueModifiers", "fLargeItemWeight", fLargeItemWeight);
    ReadFloat(ini, "BaseValueModifiers", "fMediumItemWeight", fMediumItemWeight);
    ReadFloat(ini, "BaseValueModifiers", "fSmallItemWeight", fSmallItemWeight);
    ReadUInt32(ini, "BaseValueModifiers", "fBaseWeightLimit", uBaseWeightLimit);

    // Buff & debuff settings
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsSpeed", bWeightAffectsSpeed);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsCombat", bWeightAffectsCombat);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsStealth", bWeightAffectsStealth);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsStamDrain", bWeightAffectsStamDrain);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsStamRegen", bWeightAffectsStamRegen);

    // Debug settings
    ReadBool(ini, "Debug", "bModEnabled", bModEnabled);
    ReadBool(ini, "Debug", "bLogContainerEvents", bLogContainerEvents);
    ReadBool(ini, "Debug", "bLogPlayerContainerEvents", bLogOnlyPlayerContainerEvents);
    ReadBool(ini, "Debug", "bLogLoadEvents", bLogLoadEvents);
    ReadBool(ini, "Debug", "bLogMenuEvents", bLogMenuEvents);
    ReadBool(ini, "Debug", "bLogOnlyRelevantMenuEvents", bLogOnlyRelevantMenuEvents);
    ReadBool(ini, "Debug", "bLogEquipEvents", bLogEquipEvents);
    ReadBool(ini, "Debug", "bLogOnlyPlayerEquipEvents", bLogOnlyPlayerEquipEvents);

    // ADVANCED SETTINGS
    // Race weight modifiers
    ReadFloat(ini, "Advanced", "fDefaultRaceMod", fDefaultRaceMod);
    ReadFloat(ini, "Advanced", "fAltmerRaceMod", fAltmerRaceMod);
    ReadFloat(ini, "Advanced", "fArgonianRaceMod", fArgonianRaceMod);
    ReadFloat(ini, "Advanced", "fBosmerRaceMod", fBosmerRaceMod);
    ReadFloat(ini, "Advanced", "fBretonRaceMod", fBretonRaceMod);
    ReadFloat(ini, "Advanced", "fDunmerRaceMod", fDunmerRaceMod);
    ReadFloat(ini, "Advanced", "fImperialRaceMod", fImperialRaceMod);
    ReadFloat(ini, "Advanced", "fKhajiitRaceMod", fKhajiitRaceMod);
    ReadFloat(ini, "Advanced", "fNordRaceMod", fNordRaceMod);
    ReadFloat(ini, "Advanced", "fOrcRaceMod", fOrcRaceMod);
    ReadFloat(ini, "Advanced", "fRedguardRaceMod", fRedguardRaceMod);

    // Stamina weight bonus settings 
    ReadFloat(ini, "Advanced", "fStaminaWeightMod", fStaminaWeightMod);
    ReadBool(ini, "Advanced", "bTempStaminaAddsWeight", bTempStaminaAddsWeight);
    ReadBool(ini, "Advanced", "bStaminaWeightSimple", bStaminaWeightSimple);
    ReadFloat(ini, "Advanced", "fWeightPerStamina", fWeightPerStamina);
    ReadFloat(ini, "Advanced", "fStaminaWeightRate", fStaminaWeightRate);
    ReadUInt32(ini, "Advanced", "uStaminaWeightPivot", uStaminaWeightPivot);

    // Level weight bonus settings
    ReadFloat(ini, "Advanced", "fLevelWeightMod", fLevelWeightMod);
    ReadBool(ini, "Advanced", "bLevelWeightSimple", bLevelWeightSimple);
    ReadFloat(ini, "Advanced", "fWeightPerLevel", fWeightPerLevel);
    ReadFloat(ini, "Advanced", "fLevelWeightRate", fLevelWeightRate);
    ReadUInt32(ini, "Advanced", "uLevelWeightPivot", uLevelWeightPivot);

    // SYSTEM-DEFINED VALUES
    globalContainerLog = bLogContainerEvents && !bLogOnlyPlayerContainerEvents;
    playerContainerLogOnly = bLogContainerEvents && bLogOnlyPlayerContainerEvents;
    globalMenuLog = bLogMenuEvents && !bLogOnlyRelevantMenuEvents;
    relevantMenuLogOnly = bLogMenuEvents && bLogOnlyRelevantMenuEvents;
    globalEquipLog = bLogEquipEvents && !bLogOnlyPlayerEquipEvents;
    playerEquipLogOnly = bLogEquipEvents && bLogOnlyPlayerEquipEvents;

	hugeToTiny = Settings::fLargePerHuge * Settings::fMediumPerLarge * Settings::fSmallPerMedium * Settings::fTinyPerSmall;
    largeToTiny = Settings::fMediumPerLarge * Settings::fSmallPerMedium * Settings::fTinyPerSmall;
    mediumToTiny = Settings::fSmallPerMedium * Settings::fTinyPerSmall;
    smallToTiny = Settings::fTinyPerSmall;

}


void Settings::ReadBool(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound)
	{
		a_setting = a_ini.GetBoolValue(a_sectionName, a_settingName);
	}
}

void Settings::ReadFloat(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	}
}

void Settings::ReadUInt32(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (bFound) {
		a_setting = static_cast<uint32_t>(a_ini.GetLongValue(a_sectionName, a_settingName));
	}
}
*/

void Settings::ReadIniSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName)
{
	const char* bFound = nullptr;
	bFound = a_ini.GetValue(a_sectionName, a_settingName);
	if (!bFound) { return; }
	std::string settingNameStr = a_settingName;
	
	logger::debug("Section name: Char[{}], Str[{}]", a_settingName, settingNameStr);

	if (a_settingName[0] == "b"[0]) {
		logger::debug("Setting = bool");
		bool *settingPtr = get<bool*>(settingMap[settingNameStr]);
		*settingPtr = a_ini.GetBoolValue(a_sectionName, a_settingName);
	} else if (a_settingName[0] == "f"[0]) {
		logger::debug("Setting = float");
		float *settingPtr = get<float*>(settingMap[settingNameStr]);
		*settingPtr = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	} else if (a_settingName[0] == "u"[0]) {
		logger::debug("Setting = uint32_t");
		uint32_t *settingPtr = get<uint32_t*>(settingMap[settingNameStr]);
		*settingPtr = static_cast<uint32_t>(a_ini.GetLongValue(a_sectionName, a_settingName));
	}
}

void Settings::Load(std::filesystem::path path)
{
    CSimpleIniA ini;
    ini.SetUnicode();

    ini.LoadFile(path.string().c_str());

	std::list<CSimpleIniA::Entry> iniSections;
	ini.GetAllSections(iniSections);

	for (CSimpleIniA::Entry sectionName : iniSections) {
		auto section = *ini.GetSection(sectionName.pItem);
		for (std::multimap<CSimpleIniA::Entry, const char *, CSimpleIniA::Entry::KeyOrder>::iterator it = section.begin(); it != section.end(); ++it) {
			ReadIniSetting(ini, sectionName.pItem, (*it).first.pItem);
		}
	}

	globalContainerLog = bLogContainerEvents && !bLogOnlyPlayerContainerEvents;
    playerContainerLogOnly = bLogContainerEvents && bLogOnlyPlayerContainerEvents;
    globalMenuLog = bLogMenuEvents && !bLogOnlyRelevantMenuEvents;
    relevantMenuLogOnly = bLogMenuEvents && bLogOnlyRelevantMenuEvents;
    globalEquipLog = bLogEquipEvents && !bLogOnlyPlayerEquipEvents;
    playerEquipLogOnly = bLogEquipEvents && bLogOnlyPlayerEquipEvents;
}
/*
bool* Settings::GetBool(const char* a_setting)
{
	const auto cfg = Settings::GetSingleton();

	if (a_setting == "bNoHandsOverCap") { return {&cfg->bNoHandsOverCap}; }
	if (a_setting == "bPreventPickupOverCap") { return {&cfg->bPreventPickupOverCap}; }
	if (a_setting == "bSkillsAffectCapacity") { return {&cfg->bSkillsAffectCapacity}; }
	if (a_setting == "bQuestItemsAffectCapacity") { return {&cfg->bQuestItemsAffectCapacity}; }

	if (a_setting == "bVanillaWeightLimit") { return {&cfg->bVanillaWeightLimit}; }
	if (a_setting == "bStaminaAffectsWeight") { return {&cfg->bStaminaAffectsWeight}; }
	if (a_setting == "bLevelAffectsWeight") { return {&cfg->bLevelAffectsWeight}; }
	if (a_setting == "bRaceAffectsWeight") { return {&cfg->bRaceAffectsWeight}; }

	if (a_setting == "bHugeCapacityShared") { return {&cfg->bHugeCapacityShared}; }

	if (a_setting == "bModEnabled") { return {&cfg->bModEnabled}; }

	if (a_setting == "bTempStaminaAddsWeight") { return {&cfg->bTempStaminaAddsWeight}; }
	if (a_setting == "bStaminaWeightSimple") { return {&cfg->bStaminaWeightSimple}; }

	if (a_setting == "bLevelWeightSimple") { return {&cfg->bLevelWeightSimple}; }

	return { nullptr };
}

float* Settings::GetFloat(const char* a_setting)
{
	const auto cfg = Settings::GetSingleton();

	if (a_setting == "fLargePerHuge") { return {&cfg->fLargePerHuge}; }
	if (a_setting == "fMediumPerLarge") { return {&cfg->fMediumPerLarge}; }
	if (a_setting == "fSmallPerMedium") { return {&cfg->fSmallPerMedium}; }
	if (a_setting == "fTinyPerSmall") { return {&cfg->fTinyPerSmall}; }

	if (a_setting == "fHugeItemWeight") { return {&cfg->fHugeItemWeight}; }
	if (a_setting == "fLargeItemWeight") { return {&cfg->fLargeItemWeight}; }
	if (a_setting == "fMediumItemWeight") { return {&cfg->fMediumItemWeight}; }
	if (a_setting == "fSmallItemWeight") { return {&cfg->fSmallItemWeight}; }
	
	if (a_setting == "fAltmerRaceMod") { return {&cfg->fAltmerRaceMod}; }
	if (a_setting == "fArgonianRaceMod") { return {&cfg->fArgonianRaceMod}; }
	if (a_setting == "fBosmerRaceMod") { return {&cfg->fBosmerRaceMod}; }
	if (a_setting == "fBretonRaceMod") { return {&cfg->fBretonRaceMod}; }
	if (a_setting == "fDunmerRaceMod") { return {&cfg->fDunmerRaceMod}; }
	if (a_setting == "fImperialRaceMod") { return {&cfg->fImperialRaceMod}; }
	if (a_setting == "fKhajiitRaceMod") { return {&cfg->fKhajiitRaceMod}; }
	if (a_setting == "fNordRaceMod") { return {&cfg->fNordRaceMod}; }
	if (a_setting == "fOrcRaceMod") { return {&cfg->fOrcRaceMod}; }
	if (a_setting == "fRedguardRaceMod") { return {&cfg->fRedguardRaceMod}; }
	if (a_setting == "fDefaultRaceMod") { return {&cfg->fDefaultRaceMod}; }

	if (a_setting == "fStaminaWeightMod") { return {&cfg->fStaminaWeightMod}; }
	if (a_setting == "fWeightPerStamina") { return {&cfg->fWeightPerStamina}; }
	if (a_setting == "fStaminaWeightRate") { return {&cfg->fStaminaWeightRate}; }

	if (a_setting == "fLevelWeightMod") { return {&cfg->fLevelWeightMod}; }
	if (a_setting == "fWeightPerLevel") { return {&cfg->fWeightPerLevel}; }
	if (a_setting == "fLevelWeightRate") { return {&cfg->fLevelWeightRate}; }

	return { nullptr };
}

uint32_t* Settings::GetUInt(const char* a_setting)
{
	const auto cfg = Settings::GetSingleton();

	if (a_setting == "uHugeCapacity") { return {&cfg->uHugeCapacity}; }
	if (a_setting == "uLargeCapacity") { return {&cfg->uLargeCapacity}; }
	if (a_setting == "uAlchemyCapacity") { return {&cfg->uAlchemyCapacity}; }
	if (a_setting == "uAmmoCapacity") { return {&cfg->uAmmoCapacity}; }
	if (a_setting == "uCoinCapacity") { return {&cfg->uCoinCapacity}; }
	if (a_setting == "uCoinsPerTiny") { return {&cfg->uCoinsPerTiny}; }

	if (a_setting == "uBaseWeightLimit") { return {&cfg->uBaseWeightLimit}; }

	if (a_setting == "uStaminaWeightPivot") { return {&cfg->uStaminaWeightPivot}; }

	if (a_setting == "uLevelWeightPivot") { return {&cfg->uLevelWeightPivot}; }

	return { nullptr };
}
*/