void Settings::Init()
{   
    constexpr auto defaultPath = L"Data/MCM/Config/CapacityOverhaulNG/settings.ini";
    constexpr auto mcmPath = L"Data/MCM/Settings/CapacityOverhaulNG.ini";

    logger::info("Initialising...");

    logger::info("Loading default settings...");
    Load(defaultPath);
    logger::info("...done");
    logger::info("Loading user settings...");
    Load(mcmPath);
    logger::info("...done");

    logger::info("...initialisation success!");
}

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
    ReadUInt32(ini, "BaseValueModifiers", "fMediumCapacity", uMediumCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "fSmallCapacity", uSmallCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "fAlchemyCapacity", uAlchemyCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "fAmmoCapacity", uAmmoCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "fCoinCapacity", uCoinCapacity);
    ReadUInt32(ini, "BaseValueModifiers", "fBaseWeightLimit", uBaseWeightLimit);

    // Buff & debuff settings
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsSpeed", bWeightAffectsSpeed);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightSpeedModPos", fWeightSpeedModPos);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightSpeedModNeg", fWeightSpeedModNeg);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsCombat", bWeightAffectsCombat);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightCombatModPos", fWeightCombatModPos);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightCombatModNeg", fWeightCombatModNeg);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsStealth", bWeightAffectsStealth);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightStealthModPos", fWeightStealthModPos);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightStealthModNeg", fWeightStealthModNeg);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsStamDrain", bWeightAffectsStamDrain);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightDrainModPos", fWeightStamDrainModPos);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightDrainModNeg", fWeightStamDrainModNeg);
    ReadBool(ini, "BuffsAndDebuffs", "bWeightAffectsStamRegen", bWeightAffectsStamRegen);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightRegenModPos", fWeightStamRegenModPos);
    ReadFloat(ini, "BuffsAndDebuffs", "fWeightRegenModNeg", fWeightStamRegenModNeg);

    //Debug settings
    ReadBool(ini, "Debug", "bModEnabled", bModEnabled);
    ReadBool(ini, "Debug", "bLogContainerEvents", bLogContainerEvents);
    ReadBool(ini, "Debug", "bLogPlayerContainerEvents", bLogPlayerContainerEvents);

    // Misc. (non-ini) variables
    globalLog = bLogContainerEvents && !bLogPlayerContainerEvents;
    playerLogOnly = bLogContainerEvents && bLogPlayerContainerEvents;
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