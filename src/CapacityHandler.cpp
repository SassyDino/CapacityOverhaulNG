#include "CapacityHandler.h"
#include "ExtraStorage.h"
#include "Player.h"
#include "FormHandler.h"
#include "MCPSettings.h"
#undef GetObject

namespace CapacityHandler
{

	ItemCat::ItemCat(CategoryID a_id, std::string a_idStr, std::string a_name, std::string a_tooltipKey, ImU32 a_visualiserColour)
	{
		id = a_id;
		idStr = a_idStr;
		name = a_name;
		tooltipKey = a_tooltipKey;

		visualiserColour = a_visualiserColour;

		switch (id) {
			case kWeaponLarge:
			case kWeaponMedium:
			case kWeaponSmall:
			case kWeaponRanged:
			case kShield:
				isWeaponCat = true;
				break;
			default:
				isWeaponCat = false;
		}

		//NOTE: Yeah so I really should redo the entire 'parentModifier' and 'hugeToTiny' thing, cause I am so gonna entirely forget what any of this even means, or how it works
		switch (id) {
			case kHuge:
				parentModifier = &hugeToTiny;
				break;
			case kLarge:
				parentModifier = &largeToTiny;
				break;
			case kMedium:
				parentModifier = &mediumToTiny;
				break;
			case kSmall:
				parentModifier = &smallToTiny;
				break;
			case kTiny:
				parentModifier = &tinyToTiny;
				break;
			case kAlchemy:
				parentModifier = &tinyToTiny;
				break;
			case kAmmo:
				parentModifier = &tinyToTiny;
				break;
			case kCoin:
				parentModifier = &tinyToTiny;
				break;
			case kGemstone:
				parentModifier = &tinyToTiny;
				break;
			case kWeaponLarge:
				parentModifier = &hugeToTiny;
				break;
			case kWeaponMedium:
				parentModifier = &largeToTiny;
				break;
			case kWeaponSmall:
				parentModifier = &mediumToTiny;
				break;
			case kWeaponRanged:
				parentModifier = &hugeToTiny;
				break;
			case kShield:
				parentModifier = &hugeToTiny;
				break;
			case kWeightless:
				parentModifier = &tinyToTiny;
				break;
			default:
				logger::error("Error while initialising ItemCat object '{}' - Invalid ID provided.", this->name);
				parentModifier = &tinyToTiny;
		}
	}

	const char* ItemCat::GetTooltipText()
	{
		return Lang::Get(this->tooltipKey);
	}

	float ItemCat::GetCapacityForGUI()
	{
		if (GUI::MCP::Selections::visualiserBaseValues || !PlayerState::IsGameWorldLoaded()) {
			return static_cast<float>(baseCap);
		} else {
			return static_cast<float>(capacity);
		}
	}

	float ItemCat::GetCountForGUI()
	{
		return static_cast<float>(count);
	}

	void ItemCat::IncreaseCount(int a_qty)
	{
		this->count += a_qty;
	}

	void ItemCat::DecreaseCount(int a_qty)
	{
		this->count -= a_qty;
	}

	int ItemCat::GetNormCapacity()
	{
		int normCapacity = this->capacity * (*this->parentModifier);

		if (this->id == kCoin) {
			normCapacity = int(ceil(normCapacity / Settings::Get<uint32_t>("uCoinsPerTiny")));
		} else if (this->id == kGemstone) {
			normCapacity = int(ceil((normCapacity * Settings::Get<uint32_t>("uCoinCapacityPerGem")) / Settings::Get<uint32_t>("uCoinsPerTiny")));
		}

		return normCapacity;
	}

	int ItemCat::GetNormCount()
	{
		int normCount = this->count * (*this->parentModifier);

		if (this->id == kCoin) {
			normCount = int(ceil(normCount / Settings::Get<uint32_t>("uCoinsPerTiny")));
		} else if (this->id == kGemstone) {
			normCount = int(ceil((normCount * Settings::Get<uint32_t>("uCoinCapacityPerGem")) / Settings::Get<uint32_t>("uCoinsPerTiny")));
		}

		return normCount;
	}

	bool ItemCat::IsOverflowing()
	{
		return this->count > this->capacity;
	}

	int ItemCat::GetOverflow()
	{
		if (this->count > this->capacity) {
			return this->GetNormCount() - this->GetNormCapacity();
		} else {
			return 0;
		}
	}

	float ItemCat::GetMCPPercent()
	{
		//NOTE: This code is functionally almost identical to the original code in CapacityVisualiserTotal() (aka not good, possibly even worse). So still try and improve this at some point.

		float pct;

		switch (this->id) {
			case kHuge:
			case kLarge:
			case kMedium:
			case kSmall:
			case kTiny:
				return this->GetCountForGUI() / this->GetCapacityForGUI();
			case kAlchemy:
			case kAmmo:
				pct = (this->GetCountForGUI() - this->GetCapacityForGUI()) / cTiny.GetCapacityForGUI();
				break;
			case kCoin:
				pct = ((this->GetCountForGUI() - this->GetCapacityForGUI()) / Settings::Get<uint32_t>("uCoinsPerTiny")) / cTiny.GetCapacityForGUI();
				break;
			case kWeaponLarge:
			case kWeaponMedium:
			case kWeaponSmall:
			case kWeaponRanged:
			case kShield:
				pct = (this->GetCountForGUI() - this->GetCapacityForGUI()) / this->GetCapacityForGUI();
				break;
			default:
				pct = 0.0f;
		}

		if (pct < 0) {
			return 0.0f;
		} else {
			return pct;
		}
	}

	std::string ItemCat::FractionStr()
	{
		return std::format("{}/{}", this->count, this->capacity);
	}

	ItemCat cHuge = ItemCat::ItemCat(kHuge, "kHuge", "Huge", "$Category.Huge.TooltipName", HEX_COL32(0xA8005BFF));
	ItemCat cLarge = ItemCat::ItemCat(kLarge, "kLarge", "Large", "$Category.Large.TooltipName", HEX_COL32(0xBA422FFF));
	ItemCat cMedium = ItemCat::ItemCat(kMedium, "kMedium", "Medium", "$Category.Medium.TooltipName", HEX_COL32(0xA87A00FF));
	ItemCat cSmall = ItemCat::ItemCat(kSmall, "kSmall", "Small", "$Category.Small.TooltipName", HEX_COL32(0x7AA62EFF));
	ItemCat cTiny = ItemCat::ItemCat(kTiny, "kTiny", "Tiny", "$Category.Tiny.TooltipName", HEX_COL32(0x00CC85FF));
	ItemCat cAlchemy = ItemCat::ItemCat(kAlchemy, "kAlchemy", "Alchemy", "$Category.Alchemy.TooltipName", HEX_COL32(0xBF4FB0FF));
	ItemCat cAmmo = ItemCat::ItemCat(kAmmo, "kAmmo", "Ammo", "$Category.Ammo.TooltipName", HEX_COL32(0xA64FD6FF));
	ItemCat cCoin = ItemCat::ItemCat(kCoin, "kCoin", "Coin", "$Category.Coin.TooltipName", HEX_COL32(0x635CFFFF));
	ItemCat cGemstone = ItemCat::ItemCat(kGemstone, "kGemstone", "Coin<Gemstone>", "$Category.Gemstone.TooltipName", HEX_COL32(0xFFFFFFFF));
	ItemCat cWeaponLarge = ItemCat::ItemCat(kWeaponLarge, "kWeaponLarge", "Weapon<Large>", "$Category.WeaponLarge.TooltipName", HEX_COL32(0x292E57FF));
	ItemCat cWeaponMedium = ItemCat::ItemCat(kWeaponMedium, "kWeaponMedium", "Weapon<Medium>", "$Category.WeaponMedium.TooltipName", HEX_COL32(0x6B4573FF));
	ItemCat cWeaponSmall = ItemCat::ItemCat(kWeaponSmall, "kWeaponSmall", "Weapon<Small>", "$Category.WeaponSmall.TooltipName", HEX_COL32(0xAD597DFF));
	ItemCat cWeaponRanged = ItemCat::ItemCat(kWeaponRanged, "kWeaponRanged", "Weapon<Ranged>", "$Category.WeaponRanged.TooltipName", HEX_COL32(0xE07D78FF));
	ItemCat cShield = ItemCat::ItemCat(kShield, "kShield", "Shield", "$Category.Shield.TooltipName", HEX_COL32(0xFAB270FF));
	ItemCat cWeightless = ItemCat::ItemCat(kWeightless, "kWeightless", "Weightless", "$Category.Weightless.TooltipName");

	const std::array<ItemCat*, 15> categoryArr = {&cHuge, &cLarge, &cMedium, &cSmall, &cTiny, &cAlchemy, &cAmmo, &cCoin, &cGemstone, &cWeaponLarge, &cWeaponMedium, &cWeaponSmall, &cWeaponRanged, &cShield, &cWeightless};

	ItemCat* GetCategory(CategoryID a_categoryID)
	{
		switch (a_categoryID) {
			case kHuge: return &cHuge;
			case kLarge: return &cLarge;
			case kMedium: return &cMedium;
			case kSmall: return &cSmall;
			case kTiny: return &cTiny;
			case kAlchemy: return &cAlchemy;
			case kAmmo: return &cAmmo;
			case kCoin: return &cCoin;
			case kGemstone: return &cGemstone;
			case kWeaponLarge: return &cWeaponLarge;
			case kWeaponMedium: return &cWeaponMedium;
			case kWeaponSmall: return &cWeaponSmall;
			case kWeaponRanged: return &cWeaponRanged;
			case kShield: return &cShield;
			case kWeightless: return &cWeightless;
			default: return nullptr;
		}
	}

	void ZeroAllCategories(bool suppressLog)
	{
		Bonus::equippedStorageItems.clear();
		totalCount = 0;

		for (auto& cat : categoryArr) { cat->count = 0; }

		if (!suppressLog) { logger::debug("All categories reset to 0.");}
	}

	void UpdateAllCategories(bool suppressLog)
	{
		clib_util::Timer timer;
		timer.start();
		if (!suppressLog) { logger::debug("Updating all capacity categories...");}

		ZeroAllCategories(suppressLog);

		ItemCat* category;
		
		for (auto& [form, data] : PlayerState::Char->GetInventory()) {
			auto itemID = form->GetFormID();
			auto item = RE::TESForm::LookupByID(itemID);
			auto isQuestItem = data.second.get()->IsQuestObject();

			// For some reason a load of random items with a count of 0 appear in the player's inventory when using GetInventory(), so need to ignore those.
			// Also, ignore the 20 sawn logs (ID: HF00300E) that appear in the player's inventory when they own a Hearthfire house.
			if ((data.first > 0) && (itemID != 0x300300E)) {
				category = GetItemCategory(item, isQuestItem, data.first, suppressLog);
				bool isStorage = Bonus::ItemIsStorage(item);
				if (!data.second->IsWorn() || (Settings::Get<bool>("bSeparateWeaponCategories") && category->isWeaponCat)) {
					category->IncreaseCount(data.first);
				} else {
					if (!suppressLog) { logger::trace("\t-> '{}' is Worn", item->GetName()); }
					if (isStorage) { Bonus::AddEquippedStorage(itemID); }
				}
			}
		}

		timer.stop();
		if (!suppressLog) { logger::debug("Capacity category update completed! Time taken: {}μs / {}ms", timer.duration_μs(), timer.duration_ms()); }
	}

	void UpdateBaseCapacities()
	{
		cHuge.baseCap = Settings::Get<uint32_t>("uHugeCapacity");
		if (!Settings::Get<bool>("bHugeCapacityShared")) {
			cLarge.baseCap = Settings::Get<uint32_t>("uLargeCapacity");
		} else {
			cLarge.baseCap = int(ceil(cHuge.baseCap * Settings::Get<float>("fLargePerHuge")));
		}
		cMedium.baseCap = int(ceil(cLarge.baseCap * Settings::Get<float>("fMediumPerLarge")));
		cSmall.baseCap = int(ceil(cMedium.baseCap * Settings::Get<float>("fSmallPerMedium")));
		cTiny.baseCap = int(ceil(cSmall.baseCap * Settings::Get<float>("fTinyPerSmall")));
		cAlchemy.baseCap = Settings::Get<uint32_t>("uAlchemyCapacity");
		cAmmo.baseCap  = Settings::Get<uint32_t>("uAmmoCapacity");
		cCoin.baseCap  = Settings::Get<uint32_t>("uCoinCapacity");
		cWeaponLarge.baseCap  = Settings::Get<uint32_t>("uLargeWeaponCapacity");
		cWeaponMedium.baseCap = Settings::Get<uint32_t>("uMediumWeaponCapacity");
		cWeaponSmall.baseCap = Settings::Get<uint32_t>("uSmallWeaponCapacity");
		cWeaponRanged.baseCap = Settings::Get<uint32_t>("uRangedWeaponCapacity");
		cShield.baseCap = Settings::Get<uint32_t>("uShieldCapacity");
	}

	void CalculateActualCapacities()
	{
		clib_util::Timer timer;
		logger::info("Recalculating adjusted capacity limits...");
		timer.start();

		for (auto& cat : categoryArr) { cat->capacity = cat->baseCap; }

		//TODO: Need to do some experimentation - GetActorValue vs GetBaseActorValue etc., and kAlchemy vs kAlchemyModifier etc.
		if (*Settings::Get<bool*>("bSkillsAffectCapacity")) {
			auto alchemyLvl = PlayerState::AsAV->GetActorValue(RE::ActorValue::kAlchemy);
			auto archeryLvl = PlayerState::AsAV->GetActorValue(RE::ActorValue::kArchery);
			auto speechLvl = PlayerState::AsAV->GetActorValue(RE::ActorValue::kSpeech);
			auto lockpickLvl = PlayerState::AsAV->GetActorValue(RE::ActorValue::kLockpicking);
			auto pickpocketLvl = PlayerState::AsAV->GetActorValue(RE::ActorValue::kPickpocket);

			//TODO: Consider making these configurable rather than hardcoded
			auto alchemySkillMod = 1 + (alchemyLvl/100);
			auto archerySkillMod = 1 + (archeryLvl/100);
			auto goldSkillMod = 1 + (((speechLvl + lockpickLvl + pickpocketLvl)/3) / 100);

			cAlchemy.capacity = int(ceil(cAlchemy.capacity * alchemySkillMod));
			cAmmo.capacity = int(ceil(cAmmo.capacity * archerySkillMod));
			cCoin.capacity = int(ceil(cCoin.capacity * goldSkillMod));
		}

		//TODO: Remember to complete this function to add capacity buffs from backpacks etc.
		for (const auto& [category, bonus] : Bonus::GetEquippedStorage()) {
				category->capacity += bonus;
		}

		timer.stop();
		logger::info("...done in {}μs / {}ms.",  timer.duration_μs(), timer.duration_ms());
	}

	void UpdateCategoryRatios()
	{		
		//TODO: If I can figure out some sort of "settingsChanged" bool, these can be made into global/static variables, rather than being recalculated every time UpdateTotalCount() runs
		hugeToTiny = (int)(Settings::Get<float>("fLargePerHuge") * 
				Settings::Get<float>("fMediumPerLarge") * 
				Settings::Get<float>("fSmallPerMedium") * 
				Settings::Get<float>("fTinyPerSmall"));
		largeToTiny = (int)(Settings::Get<float>("fMediumPerLarge") * 
				Settings::Get<float>("fSmallPerMedium") * 
				Settings::Get<float>("fTinyPerSmall"));
		mediumToTiny = (int)(Settings::Get<float>("fSmallPerMedium") * 
				Settings::Get<float>("fTinyPerSmall"));
		smallToTiny = (int)(Settings::Get<float>("fTinyPerSmall"));
	}

	void UpdateTotalCount()
	{
		//The "total" count only actually includes the large, medium, small, and tiny categories (plus huge, if bHugeCapacityShared), plus any OVERFLOW from the misc categories
		UpdateCategoryRatios();

		if (!Settings::Get<bool>("bHugeCapacityShared")) {
			totalCount = (cLarge.count * largeToTiny) + (cMedium.count * mediumToTiny) + (cSmall.count * smallToTiny) + cTiny.count;
		} else {
			totalCount = (cHuge.count * hugeToTiny) + (cLarge.count * largeToTiny) + (cMedium.count * mediumToTiny) + (cSmall.count * smallToTiny) + cTiny.count;
		}

		totalCount += cAlchemy.GetOverflow();
		totalCount += cAmmo.GetOverflow();
		totalCount += cCoin.GetOverflow();

		//NOTE: Currently, overflowing weapons are considered huge/large/medium items - could probably come up with a way of making this either more specialised, or user-defined.
		//? Additionally/alternatively, maybe find some way of making it so that, for example, ranged weapons first overflow into the large weapon category, then into totalCount if no space.
		for (ItemCat* weapCat: weaponCategories) {
			totalCount += weapCat->GetOverflow();
		}
	}

	void AdjustSingleCategory(const RE::TESContainerChangedEvent *a_event)
	{
		auto item = RE::TESForm::LookupByID(a_event->baseObj);

		auto oldContainer = a_event->oldContainer;
		auto itemCount = a_event->itemCount;
		//TODO: Figure out how to identify quest items, if at all possible
		auto isQuestItem = false;

		if (a_event->baseObj != Forms::MISC::BYOHMaterialLog) {
			ItemCat* itemCategory = GetItemCategory(item, isQuestItem, itemCount, false);

			if (oldContainer == 0x14) { // If moving item out of inventory
				itemCategory->DecreaseCount(itemCount);
			} else { // If moving item into inventory
				itemCategory->IncreaseCount(itemCount);
			}
		}

		UpdateTotalCount();
		LogAllCategories();
	}

	ItemCat* GetItemCategory(RE::TESForm *a_item, bool is_questItem, int a_qty, bool suppressLog)
	{
		auto kwItem = a_item->As<RE::BGSKeywordForm>();

		auto itemType = a_item->GetFormType();
		auto itemWeight = a_item->GetWeight();

		if (kwItem) {
			if (kwItem->HasKeywordID(Forms::KYWD::VendorItemPotion) || kwItem->HasKeywordID(Forms::KYWD::VendorItemPoison)) {
				if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, cAlchemy.name, is_questItem);}
				return &cAlchemy;
			} else if (a_item->IsAmmo()) {
				if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, cAmmo.name, is_questItem);}
				return &cAmmo;
			} else if ((a_item->IsGold()) || kwItem->HasKeywordID(Forms::KYWD::CONG_CoinItem)) {
				if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, cCoin.name, is_questItem);}
				return &cCoin;
			} else if ((kwItem->HasKeywordID(Forms::KYWD::VendorItemGem)) && (Settings::Get<bool>("bGemsInCoinCategory")) && (a_item->GetWeight() == 0.1f)) {
				if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, cGemstone.name, is_questItem);}
				return &cGemstone;
			} else if (Settings::Get<bool>("bSeparateWeaponCategories") && (a_item->Is(RE::FormType::Armor) || a_item->Is(RE::FormType::Weapon))) {
				if (ItemCat* weapCat = GetWeaponCategory(a_item); weapCat != &cWeightless) {
					if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, weapCat->name, is_questItem);}
					return weapCat;
				}
			}
		} else {
			if (a_item->Is(RE::FormType::Ammo)) {
				if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, cAmmo.name, is_questItem);}
				return &cAmmo;
			}
		}

		//NOTE: Lockpicks have an internal weight of 0.1 (due to Survival Mode), but are weightless (W = -1) when SM is disabled, so they behave a bit weirdly
		//? So until I find a better solution for whatever's happening there, this is what I'm going with
		if (a_item->IsLockpick()) {
			if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, cWeightless.name, is_questItem);}
			return &cWeightless;
		}

		ItemCat* category = GetBasicCategory(a_item);
		if (!suppressLog) {logger::trace("{}x {} [{}:0x{:X}] | Weight: {} | Category: {} | Quest Item: {}", a_qty, a_item->GetName(), RE::FormTypeToString(itemType), a_item->GetFormID(), itemWeight, category->name, is_questItem);}
		return category;
	}

	ItemCat* GetCategoryForEquip(RE::TESForm *a_item)
	{
		ItemCat* category;
		auto kwItem = a_item->As<RE::BGSKeywordForm>();

		if (kwItem) {
			if (kwItem->HasKeywordID(Forms::KYWD::VendorItemPotion) || kwItem->HasKeywordID(Forms::KYWD::VendorItemPoison)) {
				logger::trace("Equipped Item Category: {}", cAlchemy.name);
				return &cAlchemy;
			}
			
			if (Settings::Get<bool>("bSeparateWeaponCategories") && (a_item->Is(RE::FormType::Armor) || a_item->Is(RE::FormType::Weapon))) {
				category = GetWeaponCategory(a_item);

				if (category != &cWeightless) {
					logger::trace("Equipped Item Category: {}", category->name);
					return category;
				}
			}
		}
		category = GetBasicCategory(a_item);
		logger::trace("Equipped Item Category: {}", category->name);

		return category;
	}

	ItemCat* GetBasicCategory(RE::TESForm *a_item)
	{
		ItemCat* itemCategory;
		auto itemWeight = a_item->GetWeight();
		//TODO: Figure out how to identify quest items, if at all possible
		auto isQuestItem = false;

		if (itemWeight <= 0 || (!Settings::Get<bool>("bQuestItemsAffectCapacity") && isQuestItem)) {
			itemCategory = &cWeightless;
		} else if (itemWeight >= Settings::Get<float>("fHugeItemWeight")) {
			itemCategory = &cHuge;
		} else if (itemWeight >= Settings::Get<float>("fLargeItemWeight")) {
			itemCategory = &cLarge;
		} else if (itemWeight >= Settings::Get<float>("fMediumItemWeight")) {
			itemCategory = &cMedium;
		} else if (itemWeight >= Settings::Get<float>("fSmallItemWeight")) {
			itemCategory = &cSmall;
		} else {
			itemCategory = &cTiny;
		}

		return itemCategory;
	}

	ItemCat* GetWeaponCategory(RE::TESForm *a_item)
	{
		auto kwItem = a_item->As<RE::BGSKeywordForm>();

		if (kwItem->HasKeywordID(Forms::KYWD::ArmorShield)) {
			return &cShield;
		}

		for (auto keyword : kwItem->GetKeywords()) {
			std::string_view kwEditorID{keyword->GetFormEditorID()};
			if (auto it = weaponKeywords.find(kwEditorID); it != weaponKeywords.end()) {
				return it->second;
			}
		}

		return &cWeightless;
	}

	void CheckIfOverCapacity()
	{
		// If any or all the large, medium, small, or tiny categories are over-capacity, then the total category is guaranteed be over-capacity
		if (totalCount > cTiny.capacity) {
			if (PlayerState::GetOverCapacityStatus() == false) {
				PlayerState::UpdateCapacityStatus(true);
				RE::DebugNotification("You are carrying more than you have space for!");
			}
		} else {
			if (PlayerState::GetOverCapacityStatus() == true) {
				PlayerState::UpdateCapacityStatus(false);
				RE::DebugNotification("Your storage is no longer overflowing.");
			}
		}
	};

	void LogAllCategories()
	{
		logger::debug("{:~^50}", "Capacity Category Counts");
		logger::debug("\tMAIN || Huge = {}, Large = {}, Medium = {}, Small = {}, Tiny = {}", cHuge.FractionStr(), cLarge.FractionStr(), cMedium.FractionStr(), cSmall.FractionStr(), cTiny.FractionStr());
		logger::debug("\tMISC || Alchemy = {}, Ammo = {}, Coins = {}", cAlchemy.FractionStr(), cAmmo.FractionStr(), cCoin.FractionStr());
		logger::debug("\tWEAP || Large = {}, Medium = {}, Small = {}, Ranged = {}, Shields = {}", cWeaponLarge.FractionStr(), cWeaponMedium.FractionStr(), cWeaponSmall.FractionStr(), cWeaponRanged.FractionStr(), cShield.FractionStr());
		logger::debug("\tTotal = {}/{}, Weightless = {}", totalCount, cTiny.capacity, cWeightless.count);
		logger::debug("{}", std::string(50, '~'));
	}
}