#pragma once

const char* operator"" _tr(const char* key, std::size_t len);

namespace Lang
{
	const inline std::string langDir = "Data\\Interface\\Translations\\";
	const inline std::string defaultLanguage = "ENGLISH";
	const inline size_t filePrefix = 19; // "CapacityOverhaulNG_" character length
	const inline size_t fileSuffix = 5; // ".toml" character length
	const inline const char *defaultText = "Translation Not Found";
	inline const char* currentLanguage = "N/A";

	//inline std::string currentLanguage = defaultLanguage;

	inline std::unordered_map<std::string, std::string> translationMap = {
		{"$ModName", "$ModName"},
		{"$MCP.Gen", "$MCP.Gen"},
		{"$MCP.Gen.ToggleFeatures", "$MCP.Gen.ToggleFeatures"},
		{"$MCP.Gen.ToggleFeatures.Capacity", "$MCP.Gen.ToggleFeatures.Capacity"},
		{"$MCP.Gen.ToggleFeatures.Capacity.Hands", "$MCP.Gen.ToggleFeatures.Capacity.Hands"},
		{"$MCP.Gen.ToggleFeatures.Capacity.Pickup", "$MCP.Gen.ToggleFeatures.Capacity.Pickup"},
		{"$MCP.Gen.ToggleFeatures.Capacity.Containers", "$MCP.Gen.ToggleFeatures.Capacity.Containers"},
		{"$MCP.Gen.ToggleFeatures.Capacity.Interactions", "$MCP.Gen.ToggleFeatures.Capacity.Interactions"},
		{"$MCP.Gen.ToggleFeatures.Capacity.Skills", "$MCP.Gen.ToggleFeatures.Capacity.Skills"},
		{"$MCP.Gen.ToggleFeatures.Capacity.QuestItems", "$MCP.Gen.ToggleFeatures.Capacity.QuestItems"},
		{"$MCP.Gen.ToggleFeatures.Capacity.Weapons", "$MCP.Gen.ToggleFeatures.Capacity.Weapons"},
		{"$MCP.Gen.ToggleFeatures.Weight", "$MCP.Gen.ToggleFeatures.Weight"},
		{"$MCP.Gen.ToggleFeatures.Weight.Vanilla", "$MCP.Gen.ToggleFeatures.Weight.Vanilla"},
		{"$MCP.Gen.ToggleFeatures.Weight.Stamina", "$MCP.Gen.ToggleFeatures.Weight.Stamina"},
		{"$MCP.Gen.ToggleFeatures.Weight.Level", "$MCP.Gen.ToggleFeatures.Weight.Level"},
		{"$MCP.Gen.ToggleFeatures.Weight.Race", "$MCP.Gen.ToggleFeatures.Weight.Race"},
		{"$MCP.Gen.CapacitySystem", "$MCP.Gen.CapacitySystem"},
		{"$MCP.Gen.CapacitySystem.Capacities", "$MCP.Gen.CapacitySystem.Capacities"},
		{"$MCP.Gen.CapacitySystem.Capacities.Baseline", "$MCP.Gen.CapacitySystem.Capacities.Baseline"},
		{"$MCP.Gen.CapacitySystem.Capacities.Baseline.Huge", "$MCP.Gen.CapacitySystem.Capacities.Baseline.Huge"},
		{"$MCP.Gen.CapacitySystem.Capacities.Baseline.HugeShared", "$MCP.Gen.CapacitySystem.Capacities.Baseline.HugeShared"},
		{"$MCP.Gen.CapacitySystem.Capacities.Baseline.Large", "$MCP.Gen.CapacitySystem.Capacities.Baseline.Large"},
		{"$MCP.Gen.CapacitySystem.Capacities.Derived", "$MCP.Gen.CapacitySystem.Capacities.Derived"},
		{"$MCP.Gen.CapacitySystem.Capacities.Derived.LargePerHuge", "$MCP.Gen.CapacitySystem.Capacities.Derived.LargePerHuge"},
		{"$MCP.Gen.CapacitySystem.Capacities.Derived.MediumPerLarge", "$MCP.Gen.CapacitySystem.Capacities.Derived.MediumPerLarge"},
		{"$MCP.Gen.CapacitySystem.Capacities.Derived.SmallPerMedium", "$MCP.Gen.CapacitySystem.Capacities.Derived.SmallPerMedium"},
		{"$MCP.Gen.CapacitySystem.Capacities.Derived.TinyPerSmall", "$MCP.Gen.CapacitySystem.Capacities.Derived.TinyPerSmall"},
		{"$MCP.Gen.CapacitySystem.Capacities.Misc", "$MCP.Gen.CapacitySystem.Capacities.Misc"},
		{"$MCP.Gen.CapacitySystem.Capacities.Misc.Alchemy", "$MCP.Gen.CapacitySystem.Capacities.Misc.Alchemy"},
		{"$MCP.Gen.CapacitySystem.Capacities.Misc.Ammo", "$MCP.Gen.CapacitySystem.Capacities.Misc.Ammo"},
		{"$MCP.Gen.CapacitySystem.Capacities.Misc.Coin", "$MCP.Gen.CapacitySystem.Capacities.Misc.Coin"},
		{"$MCP.Gen.CapacitySystem.Capacities.Misc.CoinsPerTiny", "$MCP.Gen.CapacitySystem.Capacities.Misc.CoinsPerTiny"},
		{"$MCP.Gen.CapacitySystem.Capacities.Misc.Gems", "$MCP.Gen.CapacitySystem.Capacities.Misc.Gems"},
		{"$MCP.Gen.CapacitySystem.Capacities.Misc.CoinsPerGem", "$MCP.Gen.CapacitySystem.Capacities.Misc.CoinsPerGem"},
		{"$MCP.Gen.CapacitySystem.Capacities.Weapons", "$MCP.Gen.CapacitySystem.Capacities.Weapons"},
		{"$MCP.Gen.CapacitySystem.Capacities.Weapons.Large", "$MCP.Gen.CapacitySystem.Capacities.Weapons.Large"},
		{"$MCP.Gen.CapacitySystem.Capacities.Weapons.Medium", "$MCP.Gen.CapacitySystem.Capacities.Weapons.Medium"},
		{"$MCP.Gen.CapacitySystem.Capacities.Weapons.Small", "$MCP.Gen.CapacitySystem.Capacities.Weapons.Small"},
		{"$MCP.Gen.CapacitySystem.Capacities.Weapons.Ranged", "$MCP.Gen.CapacitySystem.Capacities.Weapons.Ranged"},
		{"$MCP.Gen.CapacitySystem.Capacities.Weapons.Shield", "$MCP.Gen.CapacitySystem.Capacities.Weapons.Shield"},
		{"$MCP.Gen.CapacitySystem.Capacities.Visualiser", "$MCP.Gen.CapacitySystem.Capacities.Visualiser"},
		{"$MCP.Gen.CapacitySystem.Capacities.Visualiser.NewWindow", "$MCP.Gen.CapacitySystem.Capacities.Visualiser.NewWindow"},
		{"$MCP.Gen.CapacitySystem.Capacities.Visualiser.Opened", "$MCP.Gen.CapacitySystem.Capacities.Visualiser.Opened"},
		{"$MCP.Gen.CapacitySystem.Capacities.Visualiser.LoadCharacter", "$MCP.Gen.CapacitySystem.Capacities.Visualiser.LoadCharacter"},
		{"$MCP.Gen.CapacitySystem.Definitions", "$MCP.Gen.CapacitySystem.Definitions"},
		{"$MCP.Gen.CapacitySystem.Definitions.Huge", "$MCP.Gen.CapacitySystem.Definitions.Huge"},
		{"$MCP.Gen.CapacitySystem.Definitions.Large", "$MCP.Gen.CapacitySystem.Definitions.Large"},
		{"$MCP.Gen.CapacitySystem.Definitions.Medium", "$MCP.Gen.CapacitySystem.Definitions.Medium"},
		{"$MCP.Gen.CapacitySystem.Definitions.Small", "$MCP.Gen.CapacitySystem.Definitions.Small"},
		{"$MCP.Gen.Weight", "$MCP.Gen.Weight"},
		{"$MCP.Gen.Weight.General", "$MCP.Gen.Weight.General"},
		{"$MCP.Gen.Weight.General.Base", "$MCP.Gen.Weight.General.Base"},
		{"$MCP.Gen.Weight.General.Stamina", "$MCP.Gen.Weight.General.Stamina"},
		{"$MCP.Gen.Weight.General.Stamina.Modifier", "$MCP.Gen.Weight.General.Stamina.Modifier"},
		{"$MCP.Gen.Weight.General.Stamina.Simple", "$MCP.Gen.Weight.General.Stamina.Simple"},
		{"$MCP.Gen.Weight.General.Stamina.Bonus", "$MCP.Gen.Weight.General.Stamina.Bonus"},
		{"$MCP.Gen.Weight.General.Level", "$MCP.Gen.Weight.General.Level"},
		{"$MCP.Gen.Weight.General.Level.Modifier", "$MCP.Gen.Weight.General.Level.Modifier"},
		{"$MCP.Gen.Weight.General.Level.Simple", "$MCP.Gen.Weight.General.Level.Simple"},
		{"$MCP.Gen.Weight.General.Level.Bonus", "$MCP.Gen.Weight.General.Level.Bonus"},
		{"$MCP.Gen.Weight.Race", "$MCP.Gen.Weight.Race"},
		{"$MCP.Gen.Weight.Race.Vanilla", "$MCP.Gen.Weight.Race.Vanilla"},
		{"$MCP.Gen.Weight.Race.Vanilla.Altmer", "$MCP.Gen.Weight.Race.Vanilla.Altmer"},
		{"$MCP.Gen.Weight.Race.Vanilla.Argonian", "$MCP.Gen.Weight.Race.Vanilla.Argonian"},
		{"$MCP.Gen.Weight.Race.Vanilla.Bosmer", "$MCP.Gen.Weight.Race.Vanilla.Bosmer"},
		{"$MCP.Gen.Weight.Race.Vanilla.Breton", "$MCP.Gen.Weight.Race.Vanilla.Breton"},
		{"$MCP.Gen.Weight.Race.Vanilla.Dunmer", "$MCP.Gen.Weight.Race.Vanilla.Dunmer"},
		{"$MCP.Gen.Weight.Race.Vanilla.Imperial", "$MCP.Gen.Weight.Race.Vanilla.Imperial"},
		{"$MCP.Gen.Weight.Race.Vanilla.Khajiit", "$MCP.Gen.Weight.Race.Vanilla.Khajiit"},
		{"$MCP.Gen.Weight.Race.Vanilla.Nord", "$MCP.Gen.Weight.Race.Vanilla.Nord"},
		{"$MCP.Gen.Weight.Race.Vanilla.Orc", "$MCP.Gen.Weight.Race.Vanilla.Orc"},
		{"$MCP.Gen.Weight.Race.Vanilla.Redguard", "$MCP.Gen.Weight.Race.Vanilla.Redguard"},
		{"$MCP.Gen.Weight.Race.Extra", "$MCP.Gen.Weight.Race.Extra"},
		{"$MCP.Gen.Weight.Race.Extra.Default", "$MCP.Gen.Weight.Race.Extra.Default"},
		{"$MCP.Gen.UI.Appearance", "$MCP.Gen.UI.Appearance"},
		{"$MCP.Gen.UI.Appearance.CustomStyling", "$MCP.Gen.UI.Appearance.CustomStyling"},
		{"$MCP.Gen.UI.Language", "$MCP.Gen.UI.Language"},
		{"$MCP.Gen.UI.Language.Manual", "$MCP.Gen.UI.Language.Manual"},
		{"$MCP.Adv", "$MCP.Adv"},
		{"$MCP.Adv.Weight", "$MCP.Adv.Weight"},
		{"$MCP.Adv.Weight.Stamina", "$MCP.Adv.Weight.Stamina"},
		{"$MCP.Adv.Weight.Stamina.Config", "$MCP.Adv.Weight.Stamina.Config"},
		{"$MCP.Adv.Weight.Stamina.Config.Rate", "$MCP.Adv.Weight.Stamina.Config.Rate"},
		{"$MCP.Adv.Weight.Stamina.Config.Pivot", "$MCP.Adv.Weight.Stamina.Config.Pivot"},
		{"$MCP.Adv.Weight.Stamina.Preview", "$MCP.Adv.Weight.Stamina.Preview"},
		{"$MCP.Adv.Weight.Stamina.Preview.X", "$MCP.Adv.Weight.Stamina.Preview.X"},
		{"$MCP.Adv.Weight.Stamina.Preview.Y", "$MCP.Adv.Weight.Stamina.Preview.Y"},
		{"$MCP.Adv.Weight.Level", "$MCP.Adv.Weight.Level"},
		{"$MCP.Adv.Weight.Level.Config", "$MCP.Adv.Weight.Level.Config"},
		{"$MCP.Adv.Weight.Level.Config.Rate", "$MCP.Adv.Weight.Level.Config.Rate"},
		{"$MCP.Adv.Weight.Level.Config.Pivot", "$MCP.Adv.Weight.Level.Config.Pivot"},
		{"$MCP.Adv.Weight.Level.Preview", "$MCP.Adv.Weight.Level.Preview"},
		{"$MCP.Adv.Weight.Level.Preview.X", "$MCP.Adv.Weight.Level.Preview.X"},
		{"$MCP.Adv.Weight.Level.Preview.Y", "$MCP.Adv.Weight.Level.Preview.Y"},
		{"$MCP.Adv.Weight.Combined", "$MCP.Adv.Weight.Combined"},
		{"$MCP.Adv.Weight.Combined.Preview", "$MCP.Adv.Weight.Combined.Preview"},
		{"$MCP.Adv.Debug", "$MCP.Adv.Debug"},
		{"$MCP.Adv.Debug.Level", "$MCP.Adv.Debug.Level"},
		{"$MCP.Adv.Debug.CurrentLevel", "$MCP.Adv.Debug.CurrentLevel"},
		{"$MCP.Adv.Debug.ModEnabled", "$MCP.Adv.Debug.ModEnabled"},
		{"$MCP.Widgets.FileManagerButtons.Save", "$MCP.Widgets.FileManagerButtons.Save"},
		{"$MCP.Widgets.FileManagerButtons.Load", "$MCP.Widgets.FileManagerButtons.Load"},
		{"$MCP.Widgets.FileManagerButtons.Reset", "$MCP.Widgets.FileManagerButtons.Reset"},
		{"$MCP.Widgets.RefreshLogLevelButton.Update", "$MCP.Widgets.RefreshLogLevelButton.Update"},
		{"$MCP.Widgets.CapacityVisualiser.ShowBase", "$MCP.Widgets.CapacityVisualiser.ShowBase"},
		{"$MCP.Widgets.CapacityVisualiser.ShowFill", "$MCP.Widgets.CapacityVisualiser.ShowFill"},
		{"$MCP.Widgets.CapacityVisualiser.Summary", "$MCP.Widgets.CapacityVisualiser.Summary"},
		{"$MCP.Widgets.CapacityVisualiser.Main", "$MCP.Widgets.CapacityVisualiser.Main"},
		{"$MCP.Widgets.CapacityVisualiser.Weapon", "$MCP.Widgets.CapacityVisualiser.Weapon"},
		{"$MCP.Widgets.CapacityVisualiser.Misc", "$MCP.Widgets.CapacityVisualiser.Misc"},
		{"$MCP.Widgets.CapacityVisualiserWindow.Title", "$MCP.Widgets.CapacityVisualiserWindow.Title"}
	};

	void InitTranslations();

	std::string GetLanguageFromFileName(std::string a_file);
	std::string GetFileNameFromLanguage(std::string a_language);

	std::string GetGameLanguage();
	std::string GetLanguageFilePath(std::string a_language);

	void LoadTranslations(std::string path = "DEFAULT");
	void LoadPlaceholderStrings();

	const char* Get(const char* a_key);
}