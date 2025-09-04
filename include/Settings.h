#pragma once
#include "Utils.h"

class Settings final : public Utils::Singleton<Settings>
{
	// Toggle features
	static inline bool		bNoHandsOverCap{true};
	static inline bool		bPreventPickupOverCap{true};
	static inline bool		bSkillsAffectCapacity{true};
	static inline bool		bQuestItemsAffectCapacity{false};

	static inline bool		bVanillaWeightLimit{false};
	static inline bool		bStaminaAffectsWeight{true};
	static inline bool		bLevelAffectsWeight{true};
	static inline bool		bRaceAffectsWeight{true};
	
	// Base value modifiers
	static inline bool		bHugeCapacityShared{false};
	static inline uint32_t	uHugeCapacity{1};
	static inline uint32_t	uLargeCapacity{5};
	static inline float		fLargePerHuge{5};
	static inline float		fMediumPerLarge{2};
	static inline float		fSmallPerMedium{2.5};
	static inline float		fTinyPerSmall{5};
	static inline uint32_t	uAlchemyCapacity{25};
	static inline uint32_t	uAmmoCapacity{50};
	static inline uint32_t	uCoinCapacity{500};
	static inline uint32_t	uCoinsPerTiny{25};
	static inline float		fHugeItemWeight{20};
	static inline float		fLargeItemWeight{9};
	static inline float		fMediumItemWeight{5};
	static inline float		fSmallItemWeight{1};

	// Buff & debuff settings
	static inline bool    bWeightAffectsSpeed{true};
	static inline bool    bWeightAffectsCombat{true};
	static inline bool    bWeightAffectsStealth{true};
	static inline bool    bWeightAffectsStamDrain{true};
	static inline bool    bWeightAffectsStamRegen{true};

	// Debug settings
	static inline bool    bModEnabled{true};
	static inline bool    bLogContainerEvents{true};
	static inline bool    bLogOnlyPlayerContainerEvents{true};
	static inline bool    bLogLoadEvents{true};
	static inline bool    bLogMenuEvents{true};
	static inline bool    bLogOnlyRelevantMenuEvents{true};
	static inline bool    bLogEquipEvents{true};
	static inline bool    bLogOnlyPlayerEquipEvents{true};

	// ADVANCED SETTINGS
	// Per-race weight limit modifiers
	static inline uint32_t	uBaseCarryWeight{100};

	static inline float fDefaultRaceMod{1};
	static inline float fAltmerRaceMod{1.1};
	static inline float fArgonianRaceMod{1.1};
	static inline float fBosmerRaceMod{0.8};
	static inline float fBretonRaceMod{0.95};
	static inline float fDunmerRaceMod{0.9};
	static inline float fImperialRaceMod{1};
	static inline float fKhajiitRaceMod{0.9};
	static inline float fNordRaceMod{1.2};
	static inline float fOrcRaceMod{1.25};
	static inline float fRedguardRaceMod{1.2};

	// Stamina-based carry weight bonus settings/modifiers
	static inline float fStaminaWeightMod{1};
	static inline bool  bTempStaminaAddsWeight{true};
	static inline bool  bStaminaWeightSimple{true};
	static inline float fWeightPerStamina{1};
	static inline float fStaminaWeightRate{0.5};
	static inline uint32_t uStaminaWeightPivot{100};
	
	// Level-based carry weight bonus settings/modifiers
	static inline float fLevelWeightMod{1};
	static inline bool  bLevelWeightSimple{true};
	static inline float fWeightPerLevel{1};
	static inline float fLevelWeightRate{0.5};
	static inline uint32_t uLevelWeightPivot{100};

	// SYSTEM-DEFINED VALUES
	static inline bool globalContainerLog;
	static inline bool playerContainerLogOnly;
	static inline bool globalMenuLog;
	static inline bool relevantMenuLogOnly;
	static inline bool globalEquipLog;
	static inline bool playerEquipLogOnly;

	static inline uint32_t uTestSetting{69};

	static inline std::unordered_map <std::string, std::variant<bool*, float*, uint32_t*>> settingMap = {
		{"bNoHandsOverCap", &bNoHandsOverCap}, 
		{"bPreventPickupOverCap", &bPreventPickupOverCap},
		{"bPreventPickupOverCap", &bPreventPickupOverCap},
		{"bSkillsAffectCapacity", &bSkillsAffectCapacity},
		{"bQuestItemsAffectCapacity", &bQuestItemsAffectCapacity},
		{"bVanillaWeightLimit", &bVanillaWeightLimit},
		{"bStaminaAffectsWeight", &bStaminaAffectsWeight},
		{"bLevelAffectsWeight", &bLevelAffectsWeight},
		{"bRaceAffectsWeight", &bRaceAffectsWeight},
		{"bHugeCapacityShared", &bHugeCapacityShared},
		{"uHugeCapacity", &uHugeCapacity},
		{"uLargeCapacity", &uLargeCapacity},
		{"fLargePerHuge", &fLargePerHuge},
		{"fMediumPerLarge", &fMediumPerLarge},
		{"fSmallPerMedium", &fSmallPerMedium},
		{"fTinyPerSmall", &fTinyPerSmall},
		{"uAlchemyCapacity", &uAlchemyCapacity},
		{"uAmmoCapacity", &uAmmoCapacity},
		{"uCoinCapacity", &uCoinCapacity},
		{"uCoinsPerTiny", &uCoinsPerTiny},
		{"fHugeItemWeight", &fHugeItemWeight},
		{"fLargeItemWeight", &fLargeItemWeight},
		{"fMediumItemWeight", &fMediumItemWeight},
		{"fSmallItemWeight", &fSmallItemWeight},
		{"bWeightAffectsSpeed", &bWeightAffectsSpeed},
		{"bWeightAffectsCombat", &bWeightAffectsCombat},
		{"bWeightAffectsStealth", &bWeightAffectsStealth},
		{"bWeightAffectsStamDrain", &bWeightAffectsStamDrain},
		{"bWeightAffectsStamRegen", &bWeightAffectsStamRegen},
		{"bModEnabled", &bModEnabled},
		{"bLogContainerEvents", &bLogContainerEvents},
		{"bLogOnlyPlayerContainerEvents", &bLogOnlyPlayerContainerEvents},
		{"bLogLoadEvents", &bLogLoadEvents},
		{"bLogMenuEvents", &bLogMenuEvents},
		{"bLogOnlyRelevantMenuEvents", &bLogOnlyRelevantMenuEvents},
		{"bLogEquipEvents", &bLogEquipEvents},
		{"bLogOnlyPlayerEquipEvents", &bLogOnlyPlayerEquipEvents},
		{"uBaseCarryWeight", &uBaseCarryWeight},
		{"fDefaultRaceMod", &fDefaultRaceMod},
		{"fAltmerRaceMod", &fAltmerRaceMod},
		{"fArgonianRaceMod", &fArgonianRaceMod},
		{"fBosmerRaceMod", &fBosmerRaceMod},
		{"fBretonRaceMod", &fBretonRaceMod},
		{"fDunmerRaceMod", &fDunmerRaceMod},
		{"fImperialRaceMod", &fImperialRaceMod},
		{"fKhajiitRaceMod", &fKhajiitRaceMod},
		{"fNordRaceMod", &fNordRaceMod},
		{"fOrcRaceMod", &fOrcRaceMod},
		{"fRedguardRaceMod", &fRedguardRaceMod},
		{"fStaminaWeightMod", &fStaminaWeightMod},
		{"bTempStaminaAddsWeight", &bTempStaminaAddsWeight},
		{"bStaminaWeightSimple", &bStaminaWeightSimple},
		{"fWeightPerStamina", &fWeightPerStamina},
		{"fStaminaWeightRate", &fStaminaWeightRate},
		{"uStaminaWeightPivot", &uStaminaWeightPivot},
		{"fLevelWeightMod", &fLevelWeightMod},
		{"bLevelWeightSimple", &bLevelWeightSimple},
		{"fWeightPerLevel", &fWeightPerLevel},
		{"fLevelWeightRate", &fLevelWeightRate},
		{"uLevelWeightPivot", &uLevelWeightPivot}
	};

	static void ReadBool(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
	static void ReadFloat(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
	static void ReadUInt32(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);

	static void ReadIniSetting(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName);

	public:
		static void Init();
		static void Load(std::filesystem::path path);
		static bool Validate();

		template <typename T> 
		static T Get(std::string a_settingName)
		{
			T setting;
			if constexpr (std::is_same_v<T, bool*> || std::is_same_v<T, float*> || std::is_same_v<T, uint32_t*>) {
				setting = get<T>(settingMap[a_settingName]);
			} else if constexpr (std::is_same_v<T, bool> || std::is_same_v<T, float> || std::is_same_v<T, uint32_t>) {
				setting = *get<T*>(settingMap[a_settingName]);
			} else {
				SKSE::log::error("Provided [Settings::<type>Get()] type did not match either valid condition set.");
			}
			return { setting };
		}
};