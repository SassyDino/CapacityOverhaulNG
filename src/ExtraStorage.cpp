#include "ExtraStorage.h"
#include "FileHandler.h"
#include <ClibUtil/distribution.hpp>
#include <ClibUtil/timer.hpp>


namespace CapacityHandler
{
	std::unordered_map<uint32_t, Bonus::StorageItem> Bonus::storageItems;
	std::vector<Bonus::StorageItem*> Bonus::equippedStorageItems;

	const std::unordered_map<std::string_view, ItemCategories> Bonus::keyEnumMap = {
		{"huge", kHuge}, {"large", kLarge}, {"medium", kMedium}, {"small", kSmall}, {"tiny", kTiny}, 
		{"alchemy", kAlchemy}, {"ammo", kAmmo}, {"coins", kCoin},
		{"weaponlarge", kWeaponLarge}, {"weaponmedium", kWeaponMedium}, {"weaponsmall", kWeaponSmall}, {"weaponranged", kWeaponRanged}, {"shield", kShield}
	};

	Bonus::StorageItem::StorageItem(std::string a_mod, std::string a_name, uint32_t a_rawID, std::unordered_map<ItemCategories, uint32_t> bonusMap)
	{
		modName = a_mod;
		itemName = a_name;
		rawFormID = a_rawID;
		orderedFormID = RE::TESDataHandler::GetSingleton()->LookupFormID(rawFormID, modName);

		for (auto& [category, bonus] : bonusMap) {
			capacityBonuses.at(category) = bonus;
		}
	}

	std::string Bonus::StorageItem::GetItemName()
	{
		return itemName;
	}

	uint32_t Bonus::StorageItem::GetLoadOrderID()
	{
		return orderedFormID;
	}

	std::string Bonus::StorageItem::GetBonusesAsString()
	{
		std::string str;

		for (auto& [category, bonus] : capacityBonuses) {
			if (bonus != 0) {
				str.append(std::format("{} = {} | ", categoryNames.at(category), bonus));
			}
		}

		str.pop_back();
		str.pop_back();

		return str;
	}

	std::unordered_map<ItemCategories, uint32_t> Bonus::StorageItem::GetBonuses()
	{
		return capacityBonuses;
	}

	void Bonus::RegisterStorageItem(std::string a_mod, std::string a_name, uint32_t a_rawID, std::unordered_map<ItemCategories, uint32_t> bonusMap)
	{
		auto newStorageItem = StorageItem::StorageItem(a_mod, a_name, a_rawID, bonusMap);
		storageItems.insert({newStorageItem.GetLoadOrderID(), newStorageItem});

		if ((bonusMap.size() == 0) || (bonusMap.contains(kWeightless))) {
			logger::trace("Storage item registered: '{}' [0x{:X}] -> No Capacity Bonuses Found", newStorageItem.GetItemName(), newStorageItem.GetLoadOrderID());
		} else {
			logger::trace("Storage item registered: '{}' [0x{:X}] -> {}", newStorageItem.GetItemName(), newStorageItem.GetLoadOrderID(), newStorageItem.GetBonusesAsString());
		}
	}

	void Bonus::ParseAllTOMLFiles()
	{
		clib_util::Timer timer;
		bool tomlFilesFound = FileHandler::TOML::LoadTOMLData();

		if (!tomlFilesFound) {
			logger::info("No TOML config files to parse...");
			return;
		} else {
			logger::info("Parsing TOML files...");
			timer.start();
		}

		for (const auto& [path, data] : FileHandler::TOML::tomlDataMap){
			logger::debug("Parsing TOML file: '{}'", path.string());
			ParseItemsFromFile(path.string(), data);
		}

		timer.stop();
		logger::info("All TOML config files parsed successfully! Time taken: {}μs / {}ms\n{}", timer.duration_μs(), timer.duration_ms(), std::string(100, '='));
	}

	void Bonus::ParseItemsFromFile(std::string modPath, toml::table tomlFile)
	{
		std::optional<std::string> pluginName = tomlFile["mod"].value<std::string>();

		if (!pluginName.has_value()) {
			logger::error("Error with TOML config file located at: '{}'", modPath);
			logger::error("Capacity Overhaul NG could not parse a mod name. Ensure the config file begins with the key 'mod', followed by the mod name + file extension. For example: mod = \"mymodname.esp\"");
			return;
		}
		
		if (!clib_util::distribution::is_mod_name(pluginName.value())) {
			logger::error("Error with TOML config file located at: '{}'", modPath);
			logger::error("The file extension provided for the mod '{}' is invalid. Ensure that the mod name provided ends with .esp, .esm, or .esl.", pluginName.value());
			return;
		}

		if (RE::TESDataHandler::GetSingleton()->LookupModByName(pluginName.value()) == nullptr) {
			logger::info("'{}' is not currently installed (or is misspelt). Skipping TOML config file...", pluginName.value());
			return;
		}

		auto tables = tomlFile["storageItems"];
		toml::array* tableArray = tables.as_array();

		tableArray->for_each([&pluginName](auto&& tab)
		{
			if constexpr (toml::is_table<decltype(tab)>) {
				ParseItemsFromTable(pluginName.value(), *tab.as_table());
			} else {
				logger::error("Failed to read data in TOML file for mod '{}' : Ensure formatting is correct", pluginName.value());
			}
		});
	}

	void Bonus::ParseItemsFromTable(std::string pluginName, toml::table tomlTable)
	{
		std::vector<std::string_view> names;
		std::vector<uint32_t> rawIDs;
		std::unordered_map<std::string, uint32_t> nameIDMap;

		std::vector<uint32_t> bonuses;
		std::unordered_map<ItemCategories, uint32_t> bonusMap;

		//NOTE: Could probably do with some checks here to make sure that the formIDs actually match up to an item in-game.
		toml::array& tNames = *tomlTable.get_as<toml::array>("names");
		toml::array& tIDs = *tomlTable.get_as<toml::array>("formids");

		if (tNames.empty()) {
			logger::error("No item names found in group: Ensure all item groups in TOML config file have at least one item name provided. Skipping group...");
			return;
		}

		if (tIDs.empty()) {
			logger::error("No FormIDs found in group containing the item '{}': Ensure that the amount of FormIDs provided per item group is at least equal to the number of names provided. Skipping group...", (std::string)*tNames.get(0)->as_string());
			return;
		}

		if (tNames.size() == tIDs.size()) {
			for (int i = 0; i < tNames.size(); i++) {
				auto strName = (std::string)*tNames.get(i)->as_string();
				auto ID = (uint32_t)(int64_t)*tIDs.get(i)->as_integer();
				nameIDMap.insert({strName, ID});
			}
		} else {
			if ((tNames.size() == 1) && (tIDs.size() > 1)) {
				auto strName = (std::string)*tNames.get(0)->as_string();
				for (int i = 0; i < tIDs.size(); i++) {
					auto ID = (uint32_t)(int64_t)*tIDs.get(i)->as_integer();
					nameIDMap.insert({strName, ID});
				}
			} else {
				logger::error("List of item names is a different length to the list of FormIDs: Ensure each list contains an equal number of elements, or that 'names' only contains one variable. Skipping group...");
				return;
			}
		}

		if (const auto bonusNode = tomlTable.get("capacity")) {
			const auto& tBonuses = *bonusNode->as_table();
			tBonuses.for_each([&bonusMap](const toml::key& key, const toml::node& node)
			{
				if (auto val = node.value<int64_t>(); val.has_value()) {
					bonusMap.insert({EnumKeyFromTOML(key), static_cast<uint32_t>(*val)});
				}
			});
		}

		for (const auto& [name, formid] : nameIDMap) {
			RegisterStorageItem(pluginName, name, formid, bonusMap);
		}
	}

	ItemCategories Bonus::EnumKeyFromTOML(toml::key a_key)
	{
		ItemCategories enumKey = kWeightless;

		enumKey = keyEnumMap.at(a_key.str());

		if (enumKey == kWeightless) { logger::error("Error occurred obtaining equivalent enum for category key."); }

		return enumKey;
	}

	bool Bonus::ItemIsStorage(RE::TESForm *a_form)
	{
		if (!a_form->Is(RE::FormType::Armor)) { return false; }

		auto formID = a_form->GetFormID();
		if (!storageItems.contains(formID)) { return false; }

		logger::debug("Armour identified as a storage item: '{}' [0x{:X}]", a_form->GetName(), formID);
		return true;
	}

	Bonus::StorageItem* Bonus::GetStorageItem(uint32_t a_formID)
	{
		return &storageItems.at(a_formID);
	}

	void Bonus::AddEquippedStorage(uint32_t a_formID)
	{
		auto bag = GetStorageItem(a_formID);
		
		equippedStorageItems.push_back(bag);
	}

	void Bonus::RemoveEquippedStorage(uint32_t a_formID)
	{
		auto bag = &storageItems.at(a_formID);

		equippedStorageItems.erase(find(equippedStorageItems.begin(), equippedStorageItems.end(), bag));
	}

	std::unordered_map<ItemCategories, uint32_t> Bonus::GetEquippedStorage()
	{
		std::unordered_map<ItemCategories, uint32_t> bonusMap = {
			{kHuge, 0}, {kLarge, 0}, {kMedium, 0}, {kSmall, 0}, {kTiny, 0}, {kAlchemy, 0}, {kAmmo, 0}, {kCoin, 0},
			{kWeaponLarge, 0}, {kWeaponMedium, 0}, {kWeaponSmall, 0}, {kWeaponRanged, 0}, {kShield, 0}
		};

		if (equippedStorageItems.size() == 0) { return bonusMap;}

		for (auto bag : equippedStorageItems) {
			for (const auto& [category, bonus] : bag->GetBonuses()) {
				if (bonus != 0) {
					bonusMap.at(category) += bonus;
				}
			}
		}

		if (Settings::Get<bool>("bHugeCapacityShared")) {
			bonusMap.at(kLarge) += uint32_t(ceil(bonusMap.at(kHuge) * Settings::Get<float>("fLargePerHuge")));
		}
		bonusMap.at(kMedium) += uint32_t(ceil(bonusMap.at(kLarge) * Settings::Get<float>("fMediumPerLarge")));
		bonusMap.at(kSmall) += uint32_t(ceil(bonusMap.at(kMedium) * Settings::Get<float>("fSmallPerMedium")));
		bonusMap.at(kTiny) += uint32_t(ceil(bonusMap.at(kSmall) * Settings::Get<float>("fTinyPerSmall")));

		return bonusMap;
	}
}