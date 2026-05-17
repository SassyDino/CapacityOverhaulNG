#pragma once

namespace CapacityHandler
{
	enum CategoryID : short {
		kHuge,
        kLarge,
        kMedium,
        kSmall,
        kTiny,
        kAlchemy,
        kAmmo,
        kCoin,
		kGemstone,
		kWeaponLarge,
        kWeaponMedium,
        kWeaponSmall,
        kWeaponRanged,
		kShield,
		kWeightless
    };

	inline uint32_t hugeToTiny = 0;
	inline uint32_t largeToTiny = 0;
	inline uint32_t mediumToTiny = 0;
	inline uint32_t smallToTiny = 0;
	inline uint32_t tinyToTiny = 1;

	struct ItemCat
	{
		CategoryID id;
		std::string idStr;

		int baseCap = 0;
		int capacity = 0;
		int count = 0;
		int countNorm = 0;

		bool isWeaponCat = false;

		uint32_t* parentModifier;

		std::string name;

		ImU32 visualiserColour = HEX_COL32(0x00000000);

		// NOTE: I know I'm implementing this class to try and avoid using a load of maps, but I could possibly consider keeping a couple of maps to associate the id enums with the category names/tooltips etc.
		ItemCat(CategoryID a_id = kWeightless, std::string a_idStr = "kCategory", std::string a_name = "cCategory", std::string a_tooltipKey = "cCategoryTT", ImU32 a_visualiserColour = HEX_COL32(0x00000000));

		const char* GetTooltipText();

		float GetCapacityForGUI();
		float GetCountForGUI();

		void IncreaseCount(int a_qty);
		void DecreaseCount(int a_qty);

		int GetNormCapacity();
		int GetNormCount();

		bool IsOverflowing();
		int GetOverflow();

		float GetMCPPercent();

		std::string FractionStr();

		private:
			std::string tooltipKey;
	};

	extern ItemCat cHuge;
	extern ItemCat cLarge;
	extern ItemCat cMedium;
	extern ItemCat cSmall;
	extern ItemCat cTiny;
	extern ItemCat cAlchemy;
	extern ItemCat cAmmo;
	extern ItemCat cCoin;
	extern ItemCat cGemstone;
	extern ItemCat cWeaponLarge;
	extern ItemCat cWeaponMedium;
	extern ItemCat cWeaponSmall;
	extern ItemCat cWeaponRanged;
	extern ItemCat cShield;
	extern ItemCat cWeightless;

	inline int totalCount = 0;

	extern const std::array<ItemCat*, 15> categoryArr;
	inline const std::array<ItemCat*, 5> weaponCategories = {&cWeaponLarge, &cWeaponMedium, &cWeaponSmall, &cWeaponRanged, &cShield};

	//TODO: Could possibly make this configurable (saving changes in config might be tricky though). Also check that crossbows are considered WeapTypeBow
	inline const std::unordered_map<std::string_view, ItemCat*> weaponKeywords = {
		{"WeapTypeGreatsword", &cWeaponLarge}, {"WeapTypeBattleaxe", &cWeaponLarge}, {"WeapTypeWarhammer", &cWeaponLarge}, {"WeapTypeStaff", &cWeaponLarge},
		{"WeapTypeSword", &cWeaponMedium}, {"WeapTypeWarAxe", &cWeaponMedium}, {"WeapTypeMace", &cWeaponMedium},
		{"WeapTypeDagger", &cWeaponSmall},
		{"WeapTypeBow", &cWeaponRanged}
	};

	ItemCat* GetCategory(CategoryID a_categoryID);

	void UpdateBaseCapacities();
	void CalculateActualCapacities();

	void ZeroAllCategories(bool suppressLog);
	void UpdateAllCategories(bool suppressLog);

	void UpdateCategoryRatios();
	void UpdateTotalCount();

	void AdjustSingleCategory(const RE::TESContainerChangedEvent *a_event);

	ItemCat* GetItemCategory(RE::TESForm *a_item, bool a_quest, int a_qty, bool suppressLog);
	ItemCat* GetCategoryForEquip(RE::TESForm *a_item);
	ItemCat* GetBasicCategory(RE::TESForm *a_item);
	ItemCat* GetWeaponCategory(RE::TESForm *a_item);

	void CheckIfOverCapacity();

	void LogAllCategories();
}