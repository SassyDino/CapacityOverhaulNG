#pragma once
#include "CapacityHandler.h"
#include "FileHandler.h"

namespace CapacityHandler
{
	struct Bonus
	{
		const static std::unordered_map<std::string_view, ItemCat*> keyEnumMap;

		class StorageItem
		{
			std::string modName;
			std::string itemName;
			uint32_t rawFormID;
			uint32_t orderedFormID;

			std::unordered_map<ItemCat*, uint32_t> capacityBonuses = {
				{&cHuge, 0}, {&cLarge, 0}, {&cMedium, 0}, {&cSmall, 0}, {&cTiny, 0}, {&cAlchemy, 0}, {&cAmmo, 0}, {&cCoin, 0},
				{&cWeaponLarge, 0}, {&cWeaponMedium, 0}, {&cWeaponSmall, 0}, {&cWeaponRanged, 0}, {&cShield, 0}
			};

			public:
				StorageItem(std::string a_mod, std::string a_name, uint32_t a_rawID, std::unordered_map<ItemCat*, uint32_t> bonusMap);

				std::string GetItemName();
				uint32_t GetLoadOrderID();
				std::string GetBonusesAsString();

				std::unordered_map<ItemCat*, uint32_t> GetBonuses();
		};

		static std::unordered_map<uint32_t, StorageItem> storageItems;
		static std::vector<StorageItem*> equippedStorageItems;

		static void RegisterStorageItem(std::string a_mod, std::string a_name, uint32_t a_rawID, std::unordered_map<ItemCat*, uint32_t> bonusMap);

		static void ParseAllTOMLFiles();
		static void ParseItemsFromFile(std::string modPath, toml::table tomlFile);
		static void ParseItemsFromTable(std::string pluginName, toml::table tomlTable);
		static ItemCat* CategoryFromTOML(toml::key a_key);

		static bool ItemIsStorage(RE::TESForm *a_form);

		static StorageItem* GetStorageItem(uint32_t a_formID);

		static void AddEquippedStorage(uint32_t a_formID);
		static void RemoveEquippedStorage(uint32_t a_formID);
		static std::unordered_map<ItemCat*, uint32_t> GetEquippedStorage();
	};
}