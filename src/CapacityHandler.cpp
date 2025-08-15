#include "CapacityHandler.h"
#undef GetObject

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
        kWeightless
    };

	//TODO: Try and make this not break the whole plugin, otherwise delete
	//RE::BGSKeyword *Keywords::potion = RE::TESForm::LookupByID<RE::BGSKeyword>(0x3989B);
	//RE::BGSKeyword *Keywords::poison = RE::TESForm::LookupByID<RE::BGSKeyword>(0x74A39);
	//RE::BGSKeyword *Keywords::ammo = RE::TESForm::LookupByID<RE::BGSKeyword>(0x917E7);
	//RE::BGSKeyword *Keywords::coin = RE::TESForm::LookupByID<RE::BGSKeyword>(RE::TESDataHandler::GetSingleton()->LookupFormID(0x801, "CapacityOverhaulNG.esp"));
	
	int Limits::hugeBaseCapacity;
	int Limits::largeBaseCapacity;
	int Limits::mediumBaseCapacity;
	int Limits::smallBaseCapacity;
	int Limits::tinyBaseCapacity;
	int Limits::alchemyBaseCapacity;
	int Limits::ammoBaseCapacity;
	int Limits::coinBaseCapacity;

	int Limits::hugeCapacity;
	int Limits::largeCapacity;
	int Limits::mediumCapacity;
	int Limits::smallCapacity;
	int Limits::tinyCapacity;
	int Limits::alchemyCapacity;
	int Limits::ammoCapacity;
	int Limits::coinCapacity;

	int Player::hugeCount;
	int Player::largeCount;
	int Player::mediumCount;
	int Player::smallCount;
	int Player::tinyCount;
	int Player::totalCount;
	int Player::alchemyCount;
	int Player::ammoCount;
	int Player::coinCount;
	int Player::weightlessCount;

	//std::map<int, int> Player::categoryCountIndex {
	//	{kLarge, largeCount}, {kMedium, mediumCount}, {kSmall, smallCount}, {kTiny, tinyCount},
	//	{kAlchemy, alchemyCount}, {kAmmo, ammoCount}, {kCoin, coinCount}
	//};

	void Limits::UpdateBaseValues()
	{   
		logger::info("Recalculating base capacity limits...");
		hugeBaseCapacity = Settings::uHugeCapacity;
		if (Settings::bHugeCapacityNotShared) {
			largeBaseCapacity = Settings::uLargeCapacity;
		} else {
			largeBaseCapacity = int(ceil(hugeBaseCapacity * Settings::fLargePerHuge));
		}
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
		logger::info("Recalculating adjusted capacity limits...");

		//TODO: Remember to complete this function to add capacity buffs from backpacks etc.
		hugeCapacity = hugeBaseCapacity;
		largeCapacity = largeBaseCapacity;
		mediumCapacity = mediumBaseCapacity;
		smallCapacity = smallBaseCapacity;
		tinyCapacity = tinyBaseCapacity;
		alchemyCapacity = alchemyBaseCapacity;
		ammoCapacity = ammoBaseCapacity;
		coinCapacity = coinBaseCapacity;

		//TODO: Need to do some experimentation - GetActorValue vs GetBaseActorValue etc., and kAlchemy vs kAlchemyModifier etc.
		if (Settings::bSkillsAffectCapacity) {
			auto playerAVs = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
			auto alchemyLvl = playerAVs->GetActorValue(RE::ActorValue::kAlchemy);
			auto archeryLvl = playerAVs->GetActorValue(RE::ActorValue::kArchery);
			auto speechLvl = playerAVs->GetActorValue(RE::ActorValue::kSpeech);
			auto lockpickLvl = playerAVs->GetActorValue(RE::ActorValue::kLockpicking);
			auto pickpocketLvl = playerAVs->GetActorValue(RE::ActorValue::kPickpocket);

			logger::debug("Skills: Alchemy = {} | Archery = {} | Speech = {} | Lockpick = {} | Pickpocket = {}", alchemyLvl, archeryLvl, speechLvl, lockpickLvl, pickpocketLvl);

			//TODO: Consider making these configurable rather than hardcoded
			auto alchemySkillMod = 1 + (alchemyLvl/100);
			auto archerySkillMod = 1 + (archeryLvl/100);
			auto goldSkillMod = 1 + ((speechLvl + lockpickLvl + pickpocketLvl/3) / 100);

			alchemyCapacity = int(ceil(alchemyCapacity * alchemySkillMod));
			ammoCapacity = int(ceil(ammoCapacity * archerySkillMod));
			coinCapacity = int(ceil(coinCapacity * goldSkillMod));
		}
		

		logger::info("...done");
	}

	int Player::GetCategoryCount(int a_cat)
	{
		switch (a_cat) {
			case kHuge:
				return hugeCount;
				break;
			case kLarge:
				return largeCount;
				break;
			case kMedium:
				return mediumCount;
				break;
			case kSmall:
				return smallCount;
				break;
			case kTiny:
				return tinyCount;
				break;
			case kAlchemy:
				return alchemyCount;
				break;
			case kAmmo:
				return ammoCount;
				break;
			case kCoin:
				return coinCount;
				break;
			case kWeightless:
				return weightlessCount;
				break;
			default:
				logger::error("CapacityHandler::Player::GetCategoryCount -> Invalid category provided, returning 'weightlessCount'");
				return weightlessCount;
		}
	}

	void Player::AllCategoriesLog()
	{
		logger::debug("Capacity Category Counts: Huge = {}/{}, Large = {}/{}, Medium = {}/{}, Small = {}/{}, Tiny = {}/{}, Alchemy = {}/{}, Ammo = {}/{}, Coins = {}/{}, Total = {}/{}, Weightless = {}", 
			hugeCount, Limits::hugeCapacity, largeCount, Limits::largeCapacity, mediumCount, Limits::mediumCapacity, 
			smallCount, Limits::smallCapacity, tinyCount, Limits::tinyCapacity,
			alchemyCount, Limits::alchemyCapacity, ammoCount, Limits::ammoCapacity, 
			coinCount, Limits::coinCapacity, totalCount, Limits::tinyCapacity, weightlessCount);
	}

	void Player::IncreaseCategory(int a_cat, int a_count)
	{
		switch (a_cat) {
			case kHuge:
				hugeCount += a_count;
				break;
			case kLarge:
				largeCount += a_count;
				break;
			case kMedium:
				mediumCount += a_count;
				break;
			case kSmall:
				smallCount += a_count;
				break;
			case kTiny:
				tinyCount += a_count;
				break;
			case kAlchemy:
				alchemyCount += a_count;
				break;
			case kAmmo:
				ammoCount += a_count;
				break;
			case kCoin:
				coinCount += a_count;
				break;
			case kWeightless:
				weightlessCount += a_count;
				break;
			default:
				logger::error("CapacityHandler::Player::IncreaseCategory -> Invalid category provided");
		}
	}

	void Player::DecreaseCategory(int a_cat, int a_count)
	{
		switch (a_cat) {
			case kHuge:
				hugeCount -= a_count;
				break;
			case kLarge:
				largeCount -= a_count;
				break;
			case kMedium:
				mediumCount -= a_count;
				break;
			case kSmall:
				smallCount -= a_count;
				break;
			case kTiny:
				tinyCount -= a_count;
				break;
			case kAlchemy:
				alchemyCount -= a_count;
				break;
			case kAmmo:
				ammoCount -= a_count;
				break;
			case kCoin:
				coinCount -= a_count;
				break;
			case kWeightless:
				weightlessCount -= a_count;
				break;
			default:
				logger::error("CapacityHandler::Player::DecreaseCategory -> Invalid category provided");
		}
	}

	void Player::ResetAllCategories()
	{
		hugeCount = 0;
		largeCount = 0;
		mediumCount = 0;
		smallCount = 0;
		tinyCount = 0;
		totalCount = 0;
		alchemyCount = 0;
		ammoCount = 0;
		coinCount = 0;
		weightlessCount = 0;
		logger::debug("All categories reset to 0");
	}

	void Player::UpdateAllCategories()
	{
		ResetAllCategories();
		
		auto player = RE::PlayerCharacter::GetSingleton();
		int itemCategory;
		
		for (auto& [form, data] : player->GetInventory()) {
			auto itemID = form->GetFormID();
			auto item = RE::TESForm::LookupByID(itemID);
			auto isQuestItem = data.second.get()->IsQuestObject();

			// For some reason a load of random items with a count of 0 appear in the player's inventory when using GetInventory()
			if ((data.first > 0) && (itemID != 0x300300E)) {
				itemCategory = Player::GetItemCategory(item, isQuestItem, data.first);
				if (!data.second->IsWorn()) {
					IncreaseCategory(itemCategory, data.first);
				} else {
					logger::debug("{} is Worn", item->GetName());
				}
			}
		}

		UpdateTotalCount();
		AllCategoriesLog();
	}

	void Player::UpdateTotalCount()
	{
		if (Settings::bHugeCapacityNotShared) {
			totalCount = (largeCount * Settings::largeToTiny) + (mediumCount * Settings::mediumToTiny) + (smallCount * Settings::smallToTiny) + tinyCount;
		} else {
			totalCount = (hugeCount * Settings::hugeToTiny) + (largeCount * Settings::largeToTiny) + (mediumCount * Settings::mediumToTiny) + (smallCount * Settings::smallToTiny) + tinyCount;
		}

		if (alchemyCount > Limits::alchemyCapacity) totalCount += alchemyCount - Limits::alchemyCapacity;
		if (ammoCount > Limits::ammoCapacity) totalCount += ammoCount - Limits::ammoCapacity;
		if (coinCount > Limits::coinCapacity) totalCount += int(ceil((coinCount - Limits::coinCapacity)/Settings::uCoinsPerTiny));

	}

	void Player::AdjustSingleCategory(const RE::TESContainerChangedEvent *a_event)
	{
		auto item = RE::TESForm::LookupByID(a_event->baseObj);

		auto oldContainer = a_event->oldContainer;
		auto itemCount = a_event->itemCount;
		//TODO: Figure out how to identify quest items, if at all possible
		auto isQuestItem = false;

		if (a_event->baseObj != 0x300300E) {
			RE::FormID itemCategory = Player::GetItemCategory(item, isQuestItem, itemCount);

			if (oldContainer == 0x14) { // If moving item out of inventory
				DecreaseCategory(itemCategory, itemCount);
			} else { // If moving item into inventory
				IncreaseCategory(itemCategory, itemCount);
			}
		}

		//NOTE: (Un)Equipping items does not trigger ContainerChanged (as the items are staying inside the player's inv)
		//? This means that equipped gear still counts towards capacity
		//? (Un)Equipping to/from a container DOES trigger ContainerChanged as well as EquipEvent
		//? Storing equipped gear does EquipEvent first, then ContainerChanged
		//? Equipping stored gear does ContainerChanged first, then EquipEvent
		//TODO: So basically, I need to use EquipEvent to remove a given equipped item from it's respective category
		//TODO: And then add it back when item is unequipped
		//? I think this should mostly avoid any potentially nasty/messy issues with regards to direct transfer between storage and equipped

		UpdateTotalCount();
		AllCategoriesLog();
	}

	int Player::GetItemCategory(RE::TESForm *a_item, bool a_quest, int a_count)
	{
		int itemCategory;
		auto item = a_item;
		auto isQuestItem = a_quest;
		auto kwItem = item->As<RE::BGSKeywordForm>();
		auto itemType = item->GetFormType();
		auto itemWeight = item->GetWeight();

		logger::debug("{}x {} [{}:0x{:X}] - Weight: {} | Quest Item: {}", a_count, item->GetName(), itemType, item->GetFormID(), itemWeight, isQuestItem);

		auto coinKWID = RE::TESDataHandler::GetSingleton()->LookupFormID(0x801, "CapacityOverhaulNG.esp");

		if (kwItem) {
			if (kwItem->HasKeywordID(0x8CDEC) || kwItem->HasKeywordID(0x8CDED)) {
				logger::debug("Item Category: kAlchemy");
				return kAlchemy;
			} else if (item->IsAmmo()) {
				logger::debug("Item Category: kAmmo");
				return kAmmo;
			} else if ((item->IsGold()) || kwItem->HasKeywordID(coinKWID)) {
				logger::debug("Item Category: kCoin");
				return kCoin;
			}
		} else {
			if (item->Is(RE::FormType::Ammo)) {
				logger::debug("Item Category: kAmmo");
				return kAmmo;
			}
		}

		//NOTE: Lockpicks have an internal weight of 0.1 (due to Survival Mode), but are weightless (W = -1) when SM is disabled, so they behave a bit weirdly
		//? So until I find a better solution for whatever's happening there, this is what I'm going with
		if (item->IsLockpick()) {
			logger::debug("Item Category: kWeightless");
			return kWeightless;
		}

		if (itemWeight <= 0 || (!Settings::bQuestItemsAffectCapacity && isQuestItem)) {
			itemCategory = kWeightless;
			logger::debug("Item Category: kWeightless");
		} else {
			if (itemWeight >= Settings::fHugeItemWeight) {
				itemCategory = kHuge;
				logger::debug("Item Category: kHuge");
			} else if (itemWeight >= Settings::fLargeItemWeight) {
				itemCategory = kLarge;
				logger::debug("Item Category: kLarge");
			} else if (itemWeight >= Settings::fMediumItemWeight) {
				itemCategory = kMedium;
				logger::debug("Item Category: kMedium");
			} else if (itemWeight >= Settings::fSmallItemWeight) {
				itemCategory = kSmall;
				logger::debug("Item Category: kSmall");
			} else {
				itemCategory = kTiny;
				logger::debug("Item Category: kTiny");
			}
		}

		return itemCategory;
	}

	int Player::GetEquipCategory(RE::TESForm *a_item)
	{
		int itemCategory;
		auto item = a_item;
		auto itemWeight = item->GetWeight();
		auto kwItem = item->As<RE::BGSKeywordForm>();
		//TODO: Quest item again
		auto isQuestItem = false;

		if (kwItem) {
			if (kwItem->HasKeywordID(0x8CDEC) || kwItem->HasKeywordID(0x8CDED)) {
				logger::debug("Item Category: kAlchemy");
				return kAlchemy;
			}
		}

		if (itemWeight <= 0 || (!Settings::bQuestItemsAffectCapacity && isQuestItem)) {
			itemCategory = kWeightless;
			logger::debug("Item Category: kWeightless");
		} else {
			if (itemWeight >= Settings::fHugeItemWeight) {
				itemCategory = kHuge;
				logger::debug("Item Category: kHuge");
			} else if (itemWeight >= Settings::fLargeItemWeight) {
				itemCategory = kLarge;
				logger::debug("Item Category: kLarge");
			} else if (itemWeight >= Settings::fMediumItemWeight) {
				itemCategory = kMedium;
				logger::debug("Item Category: kMedium");
			} else if (itemWeight >= Settings::fSmallItemWeight) {
				itemCategory = kSmall;
				logger::debug("Item Category: kSmall");
			} else {
				itemCategory = kTiny;
				logger::debug("Item Category: kTiny");
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