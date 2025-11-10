#include "CapacityHandler.h"
#include "ExtraStorage.h"
#include "Player.h"
#undef GetObject

namespace CapacityHandler
{
	//NOTE: With how many maps I now have just associating the category enums to different values (this, capacityMap, countMap, maybe more), I wonder if it might be worth possibly creating
	//? a custom storage class just for capacity/counts. Look into this at some point, I guess.
	const std::unordered_map<int, std::string> categoryNames = {
		{kHuge, "kHuge"}, {kLarge, "kLarge"}, {kMedium, "kMedium"}, {kSmall, "kSmall"}, {kTiny, "kTiny"},
		{kAlchemy, "kAlchemy"}, {kAmmo, "kAmmo"}, {kCoin, "kCoin"}, {kGemstone, "kGemstone"},
		{kWeaponLarge, "kWeaponLarge"}, {kWeaponMedium, "kWeaponMedium"}, {kWeaponSmall, "kWeaponSmall"}, {kWeaponRanged, "kWeaponRanged"}, {kShield, "kShield"},
		{kWeightless, "kWeightless"}
	};

	const std::array<ItemCategories, 5> mainCategories = {kHuge, kLarge, kMedium, kSmall, kTiny};
	const std::array<ItemCategories, 3> miscCategories = {kAlchemy, kAmmo, kCoin};
	const std::array<ItemCategories, 5> weaponCategories = {kWeaponLarge, kWeaponMedium, kWeaponSmall, kWeaponRanged, kShield};

	const std::unordered_map<std::string_view, int> weaponKeywords = {
		{"WeapTypeGreatsword", kWeaponLarge}, {"WeapTypeBattleaxe", kWeaponLarge}, {"WeapTypeWarhammer", kWeaponLarge}, {"WeapTypeStaff", kWeaponLarge},
		{"WeapTypeSword", kWeaponMedium}, {"WeapTypeWarAxe", kWeaponMedium}, {"WeapTypeMace", kWeaponMedium},
		{"WeapTypeDagger", kWeaponSmall},
		{"WeapTypeBow", kWeaponRanged}
	};

	
	int Base::hugeBaseCapacity;
	int Base::largeBaseCapacity;
	int Base::mediumBaseCapacity;
	int Base::smallBaseCapacity;
	int Base::tinyBaseCapacity;
	int Base::alchemyBaseCapacity;
	int Base::ammoBaseCapacity;
	int Base::coinBaseCapacity;
	int Base::weaponLargeBaseCapacity;
	int Base::weaponMediumBaseCapacity;
	int Base::weaponSmallBaseCapacity;
	int Base::weaponRangedBaseCapacity;
	int Base::shieldBaseCapacity;

	int Player::hugeCapacity;
	int Player::largeCapacity;
	int Player::mediumCapacity;
	int Player::smallCapacity;
	int Player::tinyCapacity;
	int Player::alchemyCapacity;
	int Player::ammoCapacity;
	int Player::coinCapacity;
	int Player::weaponLargeCapacity;
	int Player::weaponMediumCapacity;
	int Player::weaponSmallCapacity;
	int Player::weaponRangedCapacity;
	int Player::shieldCapacity;

	int Player::hugeCount;
	int Player::largeCount;
	int Player::mediumCount;
	int Player::smallCount;
	int Player::tinyCount;
	int Player::totalCount;
	int Player::alchemyCount;
	int Player::ammoCount;
	int Player::coinCount;
	int Player::weaponLargeCount;
	int Player::weaponMediumCount;
	int Player::weaponSmallCount;
	int Player::weaponRangedCount;
	int Player::shieldCount;
	int Player::weightlessCount;

	const std::unordered_map<int, int*> Base::baseCapacityMap = {
		{kHuge, &hugeBaseCapacity}, {kLarge, &largeBaseCapacity}, {kMedium, &mediumBaseCapacity}, {kSmall, &smallBaseCapacity}, {kTiny, &tinyBaseCapacity},
		{kAlchemy, &alchemyBaseCapacity}, {kAmmo, &ammoBaseCapacity}, {kCoin, &coinBaseCapacity},
		{kWeaponLarge, &weaponLargeBaseCapacity}, {kWeaponMedium, &weaponMediumBaseCapacity}, {kWeaponSmall, &weaponSmallBaseCapacity}, {kWeaponRanged, &weaponRangedBaseCapacity}, {kShield, &shieldBaseCapacity}
	};

	const std::unordered_map<int, int*> Player::capacityMap = {
		{kHuge, &hugeCapacity}, {kLarge, &largeCapacity}, {kMedium, &mediumCapacity}, {kSmall, &smallCapacity}, {kTiny, &tinyCapacity},
		{kAlchemy, &alchemyCapacity}, {kAmmo, &ammoCapacity}, {kCoin, &coinCapacity},
		{kWeaponLarge, &weaponLargeCapacity}, {kWeaponMedium, &weaponMediumCapacity}, {kWeaponSmall, &weaponSmallCapacity}, {kWeaponRanged, &weaponRangedCapacity}, {kShield, &shieldCapacity}
	};

	const std::unordered_map<int, int*> Player::countMap = {
		{kHuge, &hugeCount}, {kLarge, &largeCount}, {kMedium, &mediumCount}, {kSmall, &smallCount}, {kTiny, &tinyCount},
		{kAlchemy, &alchemyCount}, {kAmmo, &ammoCount}, {kCoin, &coinCount},
		{kWeaponLarge, &weaponLargeCount}, {kWeaponMedium, &weaponMediumCount}, {kWeaponSmall, &weaponSmallCount}, {kWeaponRanged, &weaponRangedCount}, {kShield, &shieldCount},
		{kWeightless, &weightlessCount}
	};

	float GetCapacityForGUI(ItemCategories a_category)
	{
		if (Settings::Get<bool>("bCapacityVisualiserBaseValues")) {
			return static_cast<float>(*Base::baseCapacityMap.at(a_category));
		} else {
			return static_cast<float>(*Player::capacityMap.at(a_category));
		}
	}

	float GetCountForGUI(ItemCategories a_category)
	{
		return static_cast<float>(*Player::countMap.at(a_category));
	}
/* 
	float CategoryPercentOfTotal(ItemCategories a_category)
	{
		if (std::ranges::contains(mainCategories, a_category)) {
			return (GetCountForGUI(a_category) / GetCapacityForGUI(a_category)); //NOTE: This will only stay true if total capacity is always equal to tiny capacity, and tiny equal to small * smallToTiny, etc.
		} else if (a_category == kCoin) {

		} else if (std::ranges::contains(miscCategories, a_category)) {

		} else if (std::ranges::contains(weaponCategories, a_category)) {

		} else {
			logger::error("fix yo shit");
		}
	}
 */
	void Base::UpdateBaseCapacities()
	{
		//logger::info("Recalculating base capacity limits...");
		hugeBaseCapacity = Settings::Get<uint32_t>("uHugeCapacity");
		if (!Settings::Get<bool>("bHugeCapacityShared")) {
			largeBaseCapacity = Settings::Get<uint32_t>("uLargeCapacity");
		} else {
			largeBaseCapacity = int(ceil(hugeBaseCapacity * Settings::Get<float>("fLargePerHuge")));
		}
		mediumBaseCapacity = int(ceil(largeBaseCapacity * Settings::Get<float>("fMediumPerLarge")));
		smallBaseCapacity = int(ceil(mediumBaseCapacity * Settings::Get<float>("fSmallPerMedium")));
		tinyBaseCapacity = int(ceil(smallBaseCapacity * Settings::Get<float>("fTinyPerSmall")));
		alchemyBaseCapacity = Settings::Get<uint32_t>("uAlchemyCapacity");
		ammoBaseCapacity = Settings::Get<uint32_t>("uAmmoCapacity");
		coinBaseCapacity = Settings::Get<uint32_t>("uCoinCapacity");
		weaponLargeBaseCapacity = Settings::Get<uint32_t>("uLargeWeaponCapacity");
		weaponMediumBaseCapacity = Settings::Get<uint32_t>("uMediumWeaponCapacity");
		weaponSmallBaseCapacity = Settings::Get<uint32_t>("uSmallWeaponCapacity");
		weaponRangedBaseCapacity = Settings::Get<uint32_t>("uRangedWeaponCapacity");
		shieldBaseCapacity = Settings::Get<uint32_t>("uShieldCapacity");
		//logger::info("...done");
	}

	void Player::CalculateActualCapacities()
	{
		logger::info("Recalculating adjusted capacity limits...");

		hugeCapacity = Base::hugeBaseCapacity;
		largeCapacity = Base::largeBaseCapacity;
		mediumCapacity = Base::mediumBaseCapacity;
		smallCapacity = Base::smallBaseCapacity;
		tinyCapacity = Base::tinyBaseCapacity;
		alchemyCapacity = Base::alchemyBaseCapacity;
		ammoCapacity = Base::ammoBaseCapacity;
		coinCapacity = Base::coinBaseCapacity;
		weaponLargeCapacity = Base::weaponLargeBaseCapacity;
		weaponMediumCapacity = Base::weaponMediumBaseCapacity;
		weaponSmallCapacity = Base::weaponSmallBaseCapacity;
		weaponRangedCapacity = Base::weaponRangedBaseCapacity;
		shieldCapacity = Base::shieldBaseCapacity;

		//TODO: Need to do some experimentation - GetActorValue vs GetBaseActorValue etc., and kAlchemy vs kAlchemyModifier etc.
		if (*Settings::Get<bool*>("bSkillsAffectCapacity")) {
			auto playerAVs = RE::PlayerCharacter::GetSingleton()->AsActorValueOwner();
			auto alchemyLvl = playerAVs->GetActorValue(RE::ActorValue::kAlchemy);
			auto archeryLvl = playerAVs->GetActorValue(RE::ActorValue::kArchery);
			auto speechLvl = playerAVs->GetActorValue(RE::ActorValue::kSpeech);
			auto lockpickLvl = playerAVs->GetActorValue(RE::ActorValue::kLockpicking);
			auto pickpocketLvl = playerAVs->GetActorValue(RE::ActorValue::kPickpocket);

			logger::trace("Skills: Alchemy = {} | Archery = {} | Speech = {} | Lockpick = {} | Pickpocket = {}", alchemyLvl, archeryLvl, speechLvl, lockpickLvl, pickpocketLvl);

			//TODO: Consider making these configurable rather than hardcoded
			auto alchemySkillMod = 1 + (alchemyLvl/100);
			auto archerySkillMod = 1 + (archeryLvl/100);
			auto goldSkillMod = 1 + (((speechLvl + lockpickLvl + pickpocketLvl)/3) / 100);

			alchemyCapacity = int(ceil(alchemyCapacity * alchemySkillMod));
			ammoCapacity = int(ceil(ammoCapacity * archerySkillMod));
			coinCapacity = int(ceil(coinCapacity * goldSkillMod));
		}

		//TODO: Remember to complete this function to add capacity buffs from backpacks etc.
		for (const auto& [category, bonus] : Bonus::GetEquippedStorage()) {
			if (bonus != 0) {
				*Player::capacityMap.at(category) += bonus;
			}
		}

		logger::info("...done");
	}

	int Player::GetCategoryCapacity(int a_cat)
	{
		int capacity = *capacityMap.at(a_cat);

		if (capacity != NULL) {
			return capacity;
		} else {
			logger::error("CapacityHandler::Player::GetCategoryCapacity -> Invalid category provided, returning 0");
			return 0;
		}
	}

	int Player::GetCategoryCount(int a_cat)
	{
		int count = *countMap.at(a_cat);

		if (count != NULL) {
			return count;
		} else {
			logger::error("CapacityHandler::Player::GetCategoryCount -> Invalid category provided, returning 0");
			return 0;
		}
	}

	//TODO: Look into using pointers or something to reduce these functions down by using GetCategoryCount or something
	void Player::IncreaseCategory(int a_cat, int a_count)
	{
		switch (a_cat)
		{
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
			case kWeaponLarge:
				weaponLargeCount += a_count;
				break;
			case kWeaponMedium:
				weaponMediumCount += a_count;
				break;
			case kWeaponSmall:
				weaponSmallCount += a_count;
				break;
			case kWeaponRanged:
				weaponRangedCount += a_count;
				break;
			case kShield:
				shieldCount += a_count;
				break;
			case kGemstone:
				coinCount += a_count * Settings::Get<uint32_t>("uCoinCapacityPerGem");
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
		switch (a_cat)
		{
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
			case kWeaponLarge:
				weaponLargeCount -= a_count;
				break;
			case kWeaponMedium:
				weaponMediumCount -= a_count;
				break;
			case kWeaponSmall:
				weaponSmallCount -= a_count;
				break;
			case kWeaponRanged:
				weaponRangedCount -= a_count;
				break;
			case kShield:
				shieldCount -= a_count;
				break;
			case kGemstone:
				coinCount -= a_count * Settings::Get<uint32_t>("uCoinCapacityPerGem");
				break;
			case kWeightless:
				weightlessCount -= a_count;
				break;
			default:
				logger::error("CapacityHandler::Player::DecreaseCategory -> Invalid category provided");
		}
	}

	void Player::ZeroAllCategories()
	{
		Bonus::equippedStorageItems.clear();
		hugeCount = 0;
		largeCount = 0;
		mediumCount = 0;
		smallCount = 0;
		tinyCount = 0;
		totalCount = 0;
		alchemyCount = 0;
		ammoCount = 0;
		coinCount = 0;
		weaponLargeCount = 0;
		weaponMediumCount = 0;
		weaponSmallCount = 0;
		weaponRangedCount = 0;
		shieldCount = 0;
		weightlessCount = 0;
		logger::debug("All categories reset to 0");
	}

	void Player::UpdateAllCategories()
	{
		ZeroAllCategories();
		
		auto player = RE::PlayerCharacter::GetSingleton();
		int itemCategory;
		
		for (auto& [form, data] : player->GetInventory()) {
			auto itemID = form->GetFormID();
			auto item = RE::TESForm::LookupByID(itemID);
			auto isQuestItem = data.second.get()->IsQuestObject();

			// For some reason a load of random items with a count of 0 appear in the player's inventory when using GetInventory(), so need to ignore those.
			// Also, ignore the 20 sawn logs (ID: HF00300E) that appear in the player's inventory when they own a Hearthfire house.
			if ((data.first > 0) && (itemID != 0x300300E)) {
				itemCategory = Player::GetItemCategory(item, isQuestItem, data.first);
				bool isStorage = Bonus::ItemIsStorage(item);
				if (!data.second->IsWorn() || (Settings::Get<bool>("bSeparateWeaponCategories") && std::ranges::contains(CapacityHandler::weaponCategories, itemCategory))) {
					IncreaseCategory(itemCategory, data.first);
				} else {
					logger::trace("{} is Worn", item->GetName());
					if (isStorage) { Bonus::AddEquippedStorage(itemID); }
				}
			}
		}

		UpdateTotalCount();
	}

	void Player::UpdateTotalCount()
	{
		//The "total" count only actually includes the large, medium, small, and tiny categories (plus huge, if bHugeCapacityShared), plus any OVERFLOW from the misc categories
		int hugeToTiny = Settings::Get<float>("fLargePerHuge") * 
				Settings::Get<float>("fMediumPerLarge") * 
				Settings::Get<float>("fSmallPerMedium") * 
				Settings::Get<float>("fTinyPerSmall");
		int largeToTiny = *Settings::Get<float*>("fMediumPerLarge") * 
				Settings::Get<float>("fSmallPerMedium") * 
				Settings::Get<float>("fTinyPerSmall");
		int mediumToTiny = *Settings::Get<float*>("fSmallPerMedium") * 
				Settings::Get<float>("fTinyPerSmall");
		int smallToTiny = Settings::Get<float>("fTinyPerSmall");

		if (!Settings::Get<bool>("bHugeCapacityShared")) {
			totalCount = (largeCount * largeToTiny) + (mediumCount * mediumToTiny) + (smallCount * smallToTiny) + tinyCount;
		} else {
			totalCount = (hugeCount * hugeToTiny) + (largeCount * largeToTiny) + (mediumCount * mediumToTiny) + (smallCount * smallToTiny) + tinyCount;
		}

		if (alchemyCount > alchemyCapacity) totalCount += alchemyCount - alchemyCapacity;
		if (ammoCount > ammoCapacity) totalCount += ammoCount - ammoCapacity;
		if (coinCount > coinCapacity) totalCount += int(ceil((coinCount - coinCapacity) / Settings::Get<uint32_t>("uCoinsPerTiny")));

		//NOTE: Currently, overflowing weapons are considered huge/large/medium items - could probably come up with a way of making this either more specialised, or user-defined.
		//? Additionally/alternatively, maybe find some way of making it so that, for example, ranged weapons first overflow into the large weapon category, then into totalCount if no space.
		if (weaponLargeCount > weaponLargeCapacity) totalCount += (weaponLargeCount - weaponLargeCapacity) * hugeToTiny;
		if (weaponMediumCount > weaponMediumCapacity) totalCount += (weaponMediumCount - weaponMediumCapacity) * largeToTiny;
		if (weaponSmallCount > weaponSmallCapacity) totalCount += (weaponSmallCount - weaponSmallCapacity) * mediumToTiny;
		if (weaponRangedCount > weaponRangedCapacity) totalCount += (weaponRangedCount - weaponRangedCapacity) * hugeToTiny;
		if (shieldCount > shieldCapacity) totalCount += (shieldCount - shieldCapacity) * hugeToTiny;
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

		UpdateTotalCount();
		LogAllCategories();
	}

	int Player::GetItemCategory(RE::TESForm *a_item, bool is_questItem, int a_count)
	{
		auto kwItem = a_item->As<RE::BGSKeywordForm>();

		auto itemType = a_item->GetFormType();
		auto itemWeight = a_item->GetWeight();
		logger::trace("{}x {} [{}:0x{:X}] - Weight: {} | Quest Item: {}", a_count, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, is_questItem);

		//auto coinKWID = RE::TESDataHandler::GetSingleton()->LookupFormID(0x801, "CapacityOverhaulNG.esp");
		auto coinKWID = RE::TESDataHandler::GetSingleton()->LookupFormID(0x801, "CapacityOverhaulNG.esp");

		if (kwItem) {
			if (kwItem->HasKeywordID(0x8CDEC) || kwItem->HasKeywordID(0x8CDED)) {
				logger::trace("Item Category: kAlchemy");
				return kAlchemy;
			} else if (a_item->IsAmmo()) {
				logger::trace("Item Category: kAmmo");
				return kAmmo;
			} else if ((a_item->IsGold()) || kwItem->HasKeywordID(coinKWID)) {
				logger::trace("Item Category: kCoin");
				return kCoin;
			} else if ( (Settings::Get<bool>("bGemsInCoinCategory")) && (kwItem->HasKeywordID(0x914ED)) && (a_item->GetWeight() == 0.1f) ) {
				logger::trace("Item Category: kGemstone");
				return kGemstone;
			} else if (Settings::Get<bool>("bSeparateWeaponCategories") && (a_item->Is(RE::FormType::Armor) || a_item->Is(RE::FormType::Weapon))) {
				if (int weapCat = GetWeaponCategory(a_item); weapCat != kWeightless) {
					return weapCat;
				}
			}
		} else {
			if (a_item->Is(RE::FormType::Ammo)) {
				logger::trace("Item Category: kAmmo");
				return kAmmo;
			}
		}

		//NOTE: Lockpicks have an internal weight of 0.1 (due to Survival Mode), but are weightless (W = -1) when SM is disabled, so they behave a bit weirdly
		//? So until I find a better solution for whatever's happening there, this is what I'm going with
		if (a_item->IsLockpick()) {
			logger::trace("Item Category: kWeightless");
			return kWeightless;
		}

		return GetBasicCategory(a_item);
	}

	int Player::GetCategoryForEquip(RE::TESForm *a_item)
	{
		auto kwItem = a_item->As<RE::BGSKeywordForm>();

		if (kwItem) {
			if (kwItem->HasKeywordID(0x8CDEC) || kwItem->HasKeywordID(0x8CDED)) {
				logger::trace("Item Category: kAlchemy");
				return kAlchemy;
			}
			
			if (Settings::Get<bool>("bSeparateWeaponCategories") && (a_item->Is(RE::FormType::Armor) || a_item->Is(RE::FormType::Weapon))) {
				if (int weaponCategory = GetWeaponCategory(a_item) != kWeightless) { return weaponCategory; }
			}
		}

		return GetBasicCategory(a_item);
	}

	int Player::GetBasicCategory(RE::TESForm *a_item)
	{
		int itemCategory;
		auto itemWeight = a_item->GetWeight();
		//TODO: Figure out how to identify quest items, if at all possible
		auto isQuestItem = false;

		if (itemWeight <= 0 || (!Settings::Get<bool>("bQuestItemsAffectCapacity") && isQuestItem)) {
			itemCategory = kWeightless;
		} else if (itemWeight >= Settings::Get<float>("fHugeItemWeight")) {
			itemCategory = kHuge;
		} else if (itemWeight >= Settings::Get<float>("fLargeItemWeight")) {
			itemCategory = kLarge;
		} else if (itemWeight >= Settings::Get<float>("fMediumItemWeight")) {
			itemCategory = kMedium;
		} else if (itemWeight >= Settings::Get<float>("fSmallItemWeight")) {
			itemCategory = kSmall;
		} else {
			itemCategory = kTiny;
		}

		logger::trace("Item Category: {}", categoryNames.at(itemCategory));
		return itemCategory;
	}

	int Player::GetWeaponCategory(RE::TESForm *a_item)
	{
		auto kwItem = a_item->As<RE::BGSKeywordForm>();

		if (kwItem->HasKeywordID(0x965B2)) {
			logger::trace("Item Category: kShield");
			return kShield;
		}

		for (auto keyword : kwItem->GetKeywords()) {
			std::string_view kwEditorID{keyword->GetFormEditorID()};
			if (auto it = weaponKeywords.find(kwEditorID); it != weaponKeywords.end()) {
				int weaponCategory = it->second;
				logger::trace("Item Category: {}", categoryNames.at(weaponCategory));
				return weaponCategory;
			}
		}

		return kWeightless;
	}

	void Player::CheckIfOverCapacity()
	{
		// If any or all the large, medium, small, or tiny categories are over-capacity, then the total category is guaranteed be over-capacity
		if (totalCount > tinyCapacity) {
			if (PlayerStatus::GetOverCapacityStatus() == false) {
				PlayerStatus::UpdateCapacityStatus(true);
				RE::DebugNotification("You are carrying more than you have space for!");
			}
		} else {
			if (PlayerStatus::GetOverCapacityStatus() == true) {
				PlayerStatus::UpdateCapacityStatus(false);
				RE::DebugNotification("Your storage is no longer overflowing.");
			}
		}
	};

	void Player::LogAllCategories()
	{
		logger::debug("Capacity Category Counts:\nMAIN || Huge = {}/{}, Large = {}/{}, Medium = {}/{}, Small = {}/{}, Tiny = {}/{}\nMISC || Alchemy = {}/{}, Ammo = {}/{}, Coins = {}/{}\nWEAP || Large = {}/{}, Medium = {}/{}, Small = {}/{}, Ranged = {}/{}, Shields = {}/{}\nTotal = {}/{}, Weightless = {}", 
			hugeCount, hugeCapacity, largeCount, largeCapacity, mediumCount, mediumCapacity, smallCount, smallCapacity, tinyCount, tinyCapacity,
			alchemyCount, alchemyCapacity, ammoCount, ammoCapacity, coinCount, coinCapacity,
			weaponLargeCount, weaponLargeCapacity, weaponMediumCount, weaponMediumCapacity, weaponSmallCount, weaponSmallCapacity, weaponRangedCount, weaponRangedCapacity, shieldCount, shieldCapacity,
			totalCount, tinyCapacity, weightlessCount
		);
	}
}