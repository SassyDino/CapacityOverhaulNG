#pragma once

struct Settings
{
    // Toggle features
    static inline bool    bNoHandsOverCap{true};
    static inline bool    bPreventPickupOverCap{true};
    static inline bool    bSkillsAffectCapacity{true};
    static inline bool    bVanillaWeightLimit{false};
    static inline bool    bStaminaAffectsWeight{true};
    static inline bool    bLevelAffectsWeight{true};
    static inline bool    bRaceAffectsWeight{true};
    
    // Base value modifiers
    static inline uint32_t   uLargeCapacity{5};
    static inline uint32_t   uMediumCapacity{10};
    static inline uint32_t   uSmallCapacity{20};
    static inline uint32_t   uAlchemyCapacity{25};
    static inline uint32_t   uAmmoCapacity{50};
    static inline uint32_t   uCoinCapacity{500};
    static inline uint32_t   uBaseWeightLimit{100};

    // Buff & debuff settings
    static inline bool    bWeightAffectsSpeed{true};
    static inline float   fWeightSpeedModPos{1};
    static inline float   fWeightSpeedModNeg{1};
    static inline bool    bWeightAffectsCombat{true};
    static inline float   fWeightCombatModPos{1};
    static inline float   fWeightCombatModNeg{1};
    static inline bool    bWeightAffectsStealth{true};
    static inline float   fWeightStealthModPos{1};
    static inline float   fWeightStealthModNeg{1};
    static inline bool    bWeightAffectsStamDrain{true};
    static inline float   fWeightStamDrainModPos{1};
    static inline float   fWeightStamDrainModNeg{1};
    static inline bool    bWeightAffectsStamRegen{true};
    static inline float   fWeightStamRegenModPos{1};
    static inline float   fWeightStamRegenModNeg{1};

    // Debug settings
    static inline bool    bModEnabled{true};
    static inline bool    bLogContainerEvents{true};
    static inline bool    bLogPlayerContainerEvents{false};

    static inline bool globalLog;
    static inline bool playerLogOnly;

    static void ReadBool(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, bool& a_setting);
	static void ReadFloat(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, float& a_setting);
	static void ReadUInt32(CSimpleIniA& a_ini, const char* a_sectionName, const char* a_settingName, uint32_t& a_setting);

    static void Init();
    static void Load(std::filesystem::path path);
};