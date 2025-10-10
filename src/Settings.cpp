const char *Settings::defaultPath = "Data/MCM/Config/CapacityOverhaulNG/settings.ini";
const char *Settings::userPath = "Data/MCM/Settings/CapacityOverhaulNG.ini";

bool Settings::settingsLoaded = false;

void Settings::Init()
{   
    //constexpr auto defaultPath = L"Data/MCM/Config/CapacityOverhaulNG/settings.ini";
    //constexpr auto mcmPath = L"Data/MCM/Settings/CapacityOverhaulNG.ini";
    bool validSettings;

    logger::info("Initialising settings");

    logger::info("Loading default settings...");
    Load(defaultPath);
    logger::info("...done");

    logger::info("Loading user settings...");
    Load(userPath);
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

	settingsLoaded = true;
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

	if (a_settingName[0] == "b"[0]) {
		bool *settingPtr = get<bool*>(settingMap[settingNameStr].first);
		*settingPtr = a_ini.GetBoolValue(a_sectionName, a_settingName);
	} else if (a_settingName[0] == "f"[0]) {
		float *settingPtr = get<float*>(settingMap[settingNameStr].first);
		*settingPtr = static_cast<float>(a_ini.GetDoubleValue(a_sectionName, a_settingName));
	} else if (a_settingName[0] == "u"[0]) {
		uint32_t *settingPtr = get<uint32_t*>(settingMap[settingNameStr].first);
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

void Settings::WriteIniSetting(CSimpleIniA& a_ini, std::pair<std::string, std::pair<std::variant<bool*, float*, uint32_t*>, std::string>> a_settingEntry)
{
	auto settingKey = a_settingEntry.first.c_str();
	auto settingType = settingKey[0];
	auto section = a_settingEntry.second.second.c_str();
	if (settingType == "b"[0]) {
		auto settingVal = *get<bool*>(a_settingEntry.second.first);
		a_ini.SetBoolValue(section, settingKey, settingVal);
	} else if (settingType == "f"[0]) {
		auto settingVal = *get<float*>(a_settingEntry.second.first);
		a_ini.SetDoubleValue(section, settingKey, settingVal);
	} else if (settingType == "u"[0]) {
		auto settingVal = *get<uint32_t*>(a_settingEntry.second.first);
		a_ini.SetLongValue(section, settingKey, settingVal);
	} else {
		logger::error("WriteIniSetting failed on setting [{}]: Invalid datatype identifier [{}] (setting name must begin with 'b', 'f', or 'u' )", settingKey, settingType);
	}
}

void Settings::SaveToFile(const char *path)
{
	logger::info("Saving settings to ini configuration file...");
	CSimpleIniA ini;
    ini.SetUnicode();
	ini.LoadFile(path);
	ini.Reset();

	for (auto it = settingMap.begin(); it != settingMap.end(); it++) {
		std::pair settingEntry = {it->first, it->second};
		WriteIniSetting(ini, settingEntry);
	}

	ini.SaveFile(path);
	logger::info("...Configuration file saved!");
}