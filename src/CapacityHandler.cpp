#include "CapacityHandler.h"
#undef GetObject

namespace CapacityHandler
{
    enum ItemCategories {
        kLarge,
        kMedium,
        kSmall,
        kTiny,
        kAlchemy,
        kAmmo,
        kCoin,
        kWeightless
    };
	
	int Limits::largeBaseCapacity;
	int Limits::mediumBaseCapacity;
	int Limits::smallBaseCapacity;
	int Limits::tinyBaseCapacity;
	int Limits::alchemyBaseCapacity;
	int Limits::ammoBaseCapacity;
	int Limits::coinBaseCapacity;

	int Limits::largeCapacity;
	int Limits::mediumCapacity;
	int Limits::smallCapacity;
	int Limits::tinyCapacity;
	int Limits::alchemyCapacity;
	int Limits::ammoCapacity;
	int Limits::coinCapacity;

	int Player::largeCount;
	int Player::mediumCount;
	int Player::smallCount;
	int Player::tinyCount;
	int Player::totalCount;
	int Player::alchemyCount;
	int Player::ammoCount;
	int Player::coinCount;

	void Limits::UpdateBaseValues()
	{   
		logger::info("Recalculating base capacity limits...");
		largeBaseCapacity = Settings::uLargeCapacity;
		mediumBaseCapacity = int(ceil(largeBaseCapacity * Settings::fMediumPerLarge));
		smallBaseCapacity = int(ceil(mediumBaseCapacity * Settings::fSmallPerMedium));
		tinyBaseCapacity = int(ceil(smallBaseCapacity * Settings::fTinyPerSmall));
		alchemyBaseCapacity = Settings::uAlchemyCapacity;
		ammoBaseCapacity = Settings::uAmmoCapacity;
		coinBaseCapacity = Settings::uCoinCapacity;
		logger::info("...done");
	}

	void Limits::CalculatePlayValues()
	{
		//TODO: Remember to complete this function to add capacity buffs from skills and backpacks etc.
		logger::info("Recalculating adjusted capacity limits...");
		largeCapacity = largeBaseCapacity;
		mediumCapacity = mediumBaseCapacity;
		smallCapacity = smallBaseCapacity;
		tinyCapacity = tinyBaseCapacity;
		alchemyCapacity = alchemyBaseCapacity;
		ammoCapacity = ammoBaseCapacity;
		coinCapacity = coinBaseCapacity;
		logger::info("...done");
	}

	int Player::GetCapacityValue(int a_cat)
	{
		int capacityValue;

		std::map<int, int> capacityValueIndex {
			{kLarge, largeCount}, {kMedium, mediumCount}, {kSmall, smallCount}, {kTiny, tinyCount},
			{kAlchemy, alchemyCount}, {kAmmo, ammoCount}, {kCoin, coinCount}
		};

		if (capacityValueIndex.at(a_cat)) return capacityValue = capacityValueIndex[a_cat];

		return 0;
	}

	void Player::UpdateAllCategories()
	{
		//? Once again, need to check if this actually works at all
		std::map<int, int> categoryCountIndex {
			{kLarge, largeCount}, {kMedium, mediumCount}, {kSmall, smallCount}, {kTiny, tinyCount},
			{kAlchemy, alchemyCount}, {kAmmo, ammoCount}, {kCoin, coinCount}
		};

		// Reset all storage category counts to zero before doing a complete update of the player's inventory
		for (auto const& [key, val] : categoryCountIndex) {
			categoryCountIndex[val] = 0;	//TODO: Should this be [key]? Need to check
		}
		
		auto player = RE::PlayerCharacter::GetSingleton();
		auto playerInv = player->GetInventory();
		int itemCategory;

		int invCount = 0;
		std::vector<std::string> invContents;
		
		for (auto const& [key, val] : playerInv) {
			invCount += 1;
			auto valChar = val.second->GetDisplayName();
			std::string valStr = valChar;
			invContents.push_back(valStr);

			//NOTE: Hopefully AsReference should work, but if not I might need to use GetFormID instead
			auto item = val.second->GetObject()->AsReference();
			itemCategory = Player::GetItemCategory(item);
			categoryCountIndex[itemCategory] =+ val.first;
		}

		UpdateTotalCount();
		//std::string invStr(invContents.begin(), invContents.end());
		//logger::debug("{} Inventory Contents: {}", invCount, invStr);
		logger::debug("Capacity Category Counts: Large = {}, Medium = {}, Small = {}, Tiny = {}, Total = {}", 
			largeCount, mediumCount, smallCount, tinyCount, totalCount);
	}

	void Player::UpdateTotalCount()
	{
		//TODO: Need to add alchemy/ammo/coin overflow logic
		totalCount = (largeCount * Settings::largeToTiny) + (mediumCount * Settings::mediumToTiny) + (smallCount * Settings::smallToTiny) + tinyCount;
	}

	void Player::AdjustSingleCategory(const RE::TESContainerChangedEvent *a_event)
	{
		auto item = RE::TESForm::LookupByID<RE::TESObjectREFR>(a_event->baseObj);
		auto oldContainer = a_event->oldContainer;
		auto newContainer = a_event->newContainer;
		auto itemCount = a_event->itemCount;

		//NOTE: Need to check that modifying map values actually changes the static variable itself, and not just a copy stored in the map or something
		//? And maybe see if i can make this map a static class member or something, especially if i have to reuse it in any other methods
		std::map<int, int> categoryCountIndex {
			{kLarge, largeCount}, {kMedium, mediumCount}, {kSmall, smallCount}, {kTiny, tinyCount},
			{kAlchemy, alchemyCount}, {kAmmo, ammoCount}, {kCoin, coinCount}
		};

		int itemCategory = Player::GetItemCategory(item);

		if (oldContainer == 0x14) { // If moving item out of inventory
			categoryCountIndex[itemCategory] =- itemCount;
		} else { // If moving item into inventory
			categoryCountIndex[itemCategory] =+ itemCount;
		}

		UpdateTotalCount();
	}

	int Player::GetItemCategory(RE::TESObjectREFR *a_item)
	{
		int itemCategory;
		auto item = a_item;
		float itemWeight = item->GetWeight();
		auto itemType = item->GetFormType();

		//NOTE: Could possibly move this to its own header or something? Cause these doesn't really need redeclaring/redefining every time this function runs.
		auto potionKW = RE::TESForm::LookupByID(0x3989B)->As<RE::BGSKeyword>();	// VendorItemPotion
		auto poisonKW = RE::TESForm::LookupByID(0x74A39)->As<RE::BGSKeyword>();	// VendorItemPoison
		//auto ammoKW = RE::TESForm::LookupByID(0x917E7)->As<RE::BGSKeyword>();	// VendorItemArrow (not currently used in this code)
		auto coinKWID = RE::TESDataHandler::GetSingleton()->LookupFormID(0x801, "CapacityOverhaulNG.esp"); // CONG_CoinItem
		auto coinKW = RE::TESForm::LookupByID(coinKWID)->As<RE::BGSKeyword>();

		auto isQuestItem = false;	//TODO: Need to figure out a way to determine if the item is a quest item. InventoryEntryData has IsQuestObject() - could be of use?

		if (item->HasKeyword(potionKW) || item->HasKeyword(poisonKW)) {	//TODO: Double check what HasKeywordHelper etc. do
			return itemCategory = kAlchemy;
		} else if (itemType == RE::FormType::Ammo) {
			return itemCategory = kAmmo;
		} else if ((item->GetFormID() == 0xF) || item->HasKeyword(coinKW)) {
			return itemCategory = kCoin;
		}

		//TODO: Need to add lots of additional checks here:
		// Check if quest item (probably shouldn't count towards capacity, maybe make it a setting though?)
		// Consider adding extra category for huge items
		// Need to consider invisible items, such as SOS addons, or any other potential vanilla or modded non-player inventory items
		// Might also need to account for quantity of duplicate items
		if (itemWeight == 0 || (!Settings::bQuestItemsAffectCapacity && isQuestItem)) {
			itemCategory = kWeightless;
		} else {
			if (itemWeight >= Settings::fLargeItemWeight) {
				itemCategory = kLarge;
			} else if (itemWeight >= Settings::fMediumItemWeight) {
				itemCategory = kMedium;
			} else if (itemWeight >= Settings::fSmallItemWeight) {
				itemCategory = kSmall;
			} else {
				itemCategory = kTiny;
			}
		}

		return itemCategory;
	}

	bool Player::IsOverCapacity()
	{
		bool totalOver = totalCount > Limits::tinyCapacity;
		bool alchemyOver = alchemyCount > Limits::alchemyCapacity;
		bool ammoOver = ammoCount > Limits::ammoCapacity;
		bool coinOver = coinCount > Limits::coinCapacity;

		bool isOver;

		// If any or all the large, medium, small, or tiny categories are over-capacity, then the total category will also be over-capacity
		if (totalOver) {
			return isOver = true;
		} else if (alchemyOver || ammoOver || coinOver) {
			int alchemyOverflow = 0;
			int ammoOverflow = 0;
			float coinOverflow = 0;

			if (alchemyOver) alchemyOverflow = alchemyCount - Limits::alchemyCapacity;
			if (ammoOver) ammoOverflow = ammoCount - Limits::ammoCapacity;
			if (coinOver) coinOverflow = (coinCount - Limits::coinCapacity)/Settings::uCoinsPerTiny;
			
			// Will only evaluate alchemy/ammo/coin overflow as "over-capacity" if the overflow into tiny storage exceeds standard capacity limits
			if ((totalCount + alchemyOverflow + ammoOverflow + coinOverflow) > Limits::tinyCapacity) return isOver = true;
		}

		return isOver = false;
	};
}