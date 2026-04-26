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
	
	const extern std::array<CategoryID, 5> mainCategories;
	const extern std::array<CategoryID, 3> miscCategories;
	const extern std::array<CategoryID, 5> weaponCategories;
	const extern std::unordered_map<std::string_view, CategoryID> weaponKeywords;

	extern uint32_t hugeToTiny;
	extern uint32_t largeToTiny;
	extern uint32_t mediumToTiny;
	extern uint32_t smallToTiny;
	extern uint32_t tinyToTiny;

	struct ItemCat
	{
		CategoryID id;
		std::string idStr;

		static const std::unordered_map<CategoryID, std::string> idStrMap;

		int baseCap = 0;
		int capacity = 0;
		int count = 0;
		int countNorm = 0;

		bool isWeaponCat = false;

		uint32_t* parentModifier;

		std::string name;

		unsigned int visualiserColour;

		// NOTE: I know I'm implementing this class to try and avoid using a load of maps, but I could possibly consider keeping a couple of maps to associate the id enums with the category names/tooltips etc.
		ItemCat(CategoryID a_id = kWeightless, std::string a_idStr = "kCategory", std::string a_name = "cCategory", std::string a_tooltipName = "cCategoryTT");

		const char* GetTooltipName();

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
			std::string tooltipName;
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