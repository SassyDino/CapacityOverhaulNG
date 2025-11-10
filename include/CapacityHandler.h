#pragma once

namespace CapacityHandler
{
	enum ItemCategories : short {
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

	const extern std::unordered_map<int, std::string> categoryNames;

	const extern std::array<ItemCategories, 5> mainCategories;
	const extern std::array<ItemCategories, 3> miscCategories;
	const extern std::array<ItemCategories, 5> weaponCategories;
	const extern std::unordered_map<std::string_view, int> weaponKeywords;

	float GetCapacityForGUI(ItemCategories a_category);
	float GetCountForGUI(ItemCategories a_category);
//	float CategoryPercentOfTotal(ItemCategories a_category);

    struct Base
    {
		
		static int hugeBaseCapacity;
		static int largeBaseCapacity;
        static int mediumBaseCapacity;
        static int smallBaseCapacity;
        static int tinyBaseCapacity;
        static int alchemyBaseCapacity;
        static int ammoBaseCapacity;
        static int coinBaseCapacity;
		static int weaponLargeBaseCapacity;
		static int weaponMediumBaseCapacity;
		static int weaponSmallBaseCapacity;
		static int weaponRangedBaseCapacity;
		static int shieldBaseCapacity;

        /** 
         * Updates the per-category values defining base capacity limits, from settings.
		 * 
		 * @note Updated item categories are: large, medium, small, tiny, alchemy, ammo, coin.
        */
        static void UpdateBaseCapacities();

		public:
			static const std::unordered_map<int, int*> baseCapacityMap;
    };

    class Player
    {
		static int hugeCapacity;
        static int largeCapacity;
        static int mediumCapacity;
        static int smallCapacity;
        static int tinyCapacity;
        static int alchemyCapacity;
        static int ammoCapacity;
        static int coinCapacity;
		static int weaponLargeCapacity;
		static int weaponMediumCapacity;
		static int weaponSmallCapacity;
		static int weaponRangedCapacity;
		static int shieldCapacity;

		static int hugeCount;
		static int largeCount;
        static int mediumCount;
        static int smallCount;
        static int tinyCount;
        static int totalCount;
        static int alchemyCount;
        static int ammoCount;
        static int coinCount;
		static int weaponLargeCount;
		static int weaponMediumCount;
		static int weaponSmallCount;
		static int weaponRangedCount;
		static int shieldCount;
		static int weightlessCount;

        public:
			static const std::unordered_map<int, int*> capacityMap;
			static const std::unordered_map<int, int*> countMap;

			/** 
			 * Calculates per-category capacity values used in all gameplay situations. 
			 * Initial values are based on the settings-derived base values, 
			 * with adjustments then made where applicable by modifiers such as equipment and skills.
			 * 
			 * @note Updated item categories are: large, medium, small, tiny, alchemy, ammo, coin.
			 */
			static void CalculateActualCapacities();

			static int GetCategoryCapacity(int a_cat);

			/** 
			 * Returns the current item count for a given item category.
			 * 
			 * @param a_cat An `ItemCategories` Enum identifying the desired item category to inspect.
			 * 
			 * @returns the item count `capacityValue` for given item category `a_cat`.
			 */
            static int GetCategoryCount(int a_cat);

			//TODO: Need to see how else I can do this, as all the switch statements is probably not the best way. Maps seem promising, but don't seem to want to behave
			static void IncreaseCategory(int a_cat, int a_count);
			static void DecreaseCategory(int a_cat, int a_count);
			static void ZeroAllCategories();

			/** 
			 * Updates all item category counts to reflect the player's current inventory.
			 */
            static void UpdateAllCategories();

			/** 
			 * Updates the item count for the `totalCount` category, based on the currently stored values for each individual item category.
			 */
            static void UpdateTotalCount();

			/** 
			 * Modifies the item count for a single item category.
			 * 
			 * @param *a_event A `TESContainerChangedEvent` which contains information (weight, quantity) about the item/stack being transferred, and whether it is being added or removed.
			 */
            static void AdjustSingleCategory(const RE::TESContainerChangedEvent *a_event);

			/** 
			 * @brief Determines the appropriate category for a given `a_item`.
			 * 
			 * If no specific category match is found (alchemy, ammo, coin, weapon), then the item's category will be determined based on the item's weight.
			 * 
			 * @param a_item An item form containing the relevant information to identify if it's an alchemy item, ammo, or a coin. 
			 * @param is_questItem Whether `a_item` is a quest item or not
			 * @param a_count The quantity of `a_item` <ONLY USED BY LOGGER>
			 * 
			 * @returns An enum category identifier from `itemCategories`.
			 */
			static int GetItemCategory(RE::TESForm *a_item, bool a_quest, int a_count);

			/**
			 * @brief Determines the appropriate category for a given `a_item`. Stripped down compared to `GetItemCategory()`, only accounting for items that may be "equipped" during a game event.
			 * 
			 * @param a_item An item form.
			 * 
			 * @returns An enum category identifier from `itemCategories`.
			 */
			static int GetCategoryForEquip(RE::TESForm *a_item);

			/**
			 * @brief Determines the basic category for a given item.
			 * 
			 * Does not take any specific characteristics or details of `a_item` into account, and instead determines the category solely based on the item's weight.
			 * 
			 * @param a_item An item form.
			 * 
			 * @return An enum category identifier, restricted to either: `kHuge`, `kLarge`, `kMedium`, `kSmall`, `kTiny`, `kWeightless`
			 */
			static int GetBasicCategory(RE::TESForm *a_item);

			static int GetWeaponCategory(RE::TESForm *a_item);

			/** 
			 * Determines whether the player is considered over-capacity, based on the currently stored values for each individual item category.
			 * 
			 * @returns `isOver`, where a result of `true` equates to the player being considered over-capacity.
			 */
            static void CheckIfOverCapacity();

			/**
			 * @brief 
			 */
			static void DebuffIfOverCapacity();

			static void LogAllCategories();
    };
}