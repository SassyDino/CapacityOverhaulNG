#pragma once

namespace CapacityHandler
{
	enum ItemCategories : short;
	
	/*
	struct Keywords
	{
		static RE::BGSKeyword *potion;
		static RE::BGSKeyword *poison;
		static RE::BGSKeyword *ammo;
		static RE::BGSKeyword *coin;
	};
	*/


    struct Limits
    {
		static int hugeBaseCapacity;
		static int largeBaseCapacity;
        static int mediumBaseCapacity;
        static int smallBaseCapacity;
        static int tinyBaseCapacity;
        static int alchemyBaseCapacity;
        static int ammoBaseCapacity;
        static int coinBaseCapacity;

        static int hugeCapacity;
        static int largeCapacity;
        static int mediumCapacity;
        static int smallCapacity;
        static int tinyCapacity;
        static int alchemyCapacity;
        static int ammoCapacity;
        static int coinCapacity;

        /** 
         * Updates the per-category values defining base capacity limits, from settings.
		 * 
		 * @note Updated item categories are: large, medium, small, tiny, alchemy, ammo, coin.
        */
        static void UpdateBaseValues();

		/** 
         * Calculates per-category capacity values used in all gameplay situations. 
		 * Initial values are based on the settings-derived base values, 
		 * with adjustments then made where applicable by modifiers such as equipment and skills.
		 * 
		 * @note Updated item categories are: large, medium, small, tiny, alchemy, ammo, coin.
        */
        static void CalculatePlayValues();
    };

    class Player
    {
		static int hugeCount;
		static int largeCount;
        static int mediumCount;
        static int smallCount;
        static int tinyCount;
        static int totalCount;
        static int alchemyCount;
        static int ammoCount;
        static int coinCount;
		static int weightlessCount;

		//static std::map<int, int> categoryCountIndex;

        public:
			/** 
			 * Returns the current item count for a given item category.
			 * 
			 * @param a_cat An `ItemCategories` Enum identifying the desired item category to inspect.
			 * 
			 * @returns the item count `capacityValue` for given item category `a_cat`.
			*/
            static int GetCategoryCount(int a_cat);

			//TODO: Need to see how else I can do this, as all the switch statements is probably not the best way. Maps seem promising, but don't seem to want to behave
			static void AllCategoriesLog();
			static void IncreaseCategory(int a_cat, int a_count);
			static void DecreaseCategory(int a_cat, int a_count);
			static void ResetAllCategories();

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
			 * Determines the appropriate category for a given `a_item`.
			 * 
			 * @param a_item A game object containing the relevant information to identify if it's an alchemy item, ammo, or a coin. If it is none of those, then category will be determined based on the item's weight.
			 * 
			 * @returns `itemCategory`, the representative `itemCategories` Enum appropriate for the given `a_item`.
			*/
			static int GetItemCategory(RE::TESForm *a_item, bool a_quest, int a_count);

			static int GetEquipCategory(RE::TESForm *a_item);

			/** 
			 * Determines whether the player is considered over-capacity, based on the currently stored values for each individual item category.
			 * 
			 * @returns `isOver`, where a result of `true` equates to the player being considered over-capacity.
			*/
            static bool IsOverCapacity();

			static void DebuffIfOverCapacity();
    };
}