#include <map>

void Settings::Init()
{   
    constexpr auto defaultPath = L"Data/MCM/Config/CapacityOverhaulNG/settings.ini";
    constexpr auto mcmPath = L"Data/MCM/Settings/CapacityOverhaulNG.ini";

    logger::info("Initialising settings...");

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

    // System-defined values
    globalContainerLog = bLogContainerEvents && !bLogOnlyPlayerContainerEvents;
    playerContainerLogOnly = bLogContainerEvents && bLogOnlyPlayerContainerEvents;
    globalMenuLog = bLogMenuEvents && !bLogOnlyRelevantMenuEvents;
    relevantMenuLogOnly = bLogMenuEvents && bLogOnlyRelevantMenuEvents;
    globalEquipLog = bLogEquipEvents && !bLogOnlyPlayerEquipEvents;
    playerEquipLogOnly = bLogEquipEvents && bLogOnlyPlayerEquipEvents;
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

float Settings::GetRaceWeightMod(uint32_t formID) 
{
    using namespace std;

        map<RE::FormID, float> raceWeightIndex {
            {0x13743, fAltmerRaceMod}, {0x13740, fArgonianRaceMod}, {0x13749, fBosmerRaceMod},
            {0x13741, fBretonRaceMod}, {0x13742, fDunmerRaceMod}, {0x13744, fImperialRaceMod},
            {0x13745, fKhajiitRaceMod}, {0x13746, fNordRaceMod}, {0x13747, fOrcRaceMod},
            {0x13748, fRedguardRaceMod}
        };

        // Fallback to default race modifier (x1) if player's race does not exist in index (to prevent custom races breaking this mod)
        float raceWeightMod = (raceWeightIndex.at(formID)) ? raceWeightIndex[formID] : fDefaultRaceMod;

        auto raceName = RE::TESRace::LookupByID(formID)->GetName();
        logger::info("Player race identified as {}. Carry weight limit modifier = x{}", raceName, raceWeightMod);
        return raceWeightMod;
        // if i need more values stored per race, maybe try something like
        // Settings::raceIndex[formID]->WeightMod()
        // with the index's values being classes/structs or something
}