#include "Translations.h"
#include <toml++/toml.hpp>
#include <ClibUtil/timer.hpp>

const char* operator"" _tr(const char* key, std::size_t len)
{
	return Lang::translationMap.at(key);
}

namespace Lang
{
	void TranslationVal::SetText(std::string text)
	{
		sText = text;
		cText = sText.c_str();

		logger::trace("Set translation key ---> '{}'", sText);
	}

	void InitTranslations()
	{
		clib_util::Timer timer;
		timer.start();
		logger::info("{:=^50}", "Initialising Translations");

		std::string gameLang = GetGameLanguage();
		std::string settingsLang = Settings::Get<std::string>("sLanguage");

		if (Settings::Get<bool>("bOverrideLanguage")) {
			if (!settingsLang.empty()) {
				if (std::string settingsLangPath = GetLanguageFilePath(settingsLang); settingsLangPath != "NO_FILE"s) {
					LoadTranslations(GetLanguageFilePath(settingsLang));
				} else {
					logger::error("No translation file provided for manually selected language '{}'. Reverting to default behaviour.", settingsLang);
					LoadTranslations();
				}
			} else {
				logger::warn("No language has been provided in settings file, despite manual override. Reverting to default behaviour.");
				LoadTranslations();
			}
		} else {
			LoadTranslations();
		}

		timer.stop();

		logger::info("Translations finished initialising. Time taken: {}μs / {}ms", timer.duration_μs(), timer.duration_ms());
	}

	std::string GetLanguageFromFileName(std::string a_file)
	{
		return a_file.substr(filePrefix, a_file.length() - fileSuffix);
	}

	std::string GetFileNameFromLanguage(std::string a_language)
	{
		return std::format("CapacityOverhaulNG_{}.toml", a_language);
	}

	std::string GetGameLanguage()
	{
		auto skyrimini = RE::INISettingCollection::GetSingleton();
		auto iniLang = skyrimini ? skyrimini->GetSetting("sLanguage:General") : nullptr;
		std::string language = (iniLang && iniLang->GetType() == RE::Setting::Type::kString) ? iniLang->data.s : "ENGLISH"s;

		logger::debug("Game language identified as '{}'", language);
		return language;
	}

	std::string GetLanguageFilePath(std::string a_language)
	{
		auto path = std::string(langDir + GetFileNameFromLanguage(a_language));

		logger::trace("Looking for a translation file at '{}'", path);

		if (std::filesystem::exists(std::filesystem::path(path))) {
			logger::debug("Found translation file at '{}'", path);
			return path;
		} else if (std::filesystem::exists(std::filesystem::path(langDir + "CapacityOverhaulNG_ENGLISH.toml"))) {
			logger::warn("Could not find a language file at '{}' -> Reverting to default file 'CapacityOverhaulNG_ENGLISH.toml'", path);
			return std::string(langDir + "CapacityOverhaulNG_ENGLISH.toml");
		} else {
			logger::error("Could not find a language file at '{}', and default language file 'CapacityOverhaulNG_ENGLISH.toml' appears to be missing. Text from this mod may not appear as expected.", path);
			return "NO_FILE";
		}
	}

	void LoadTranslations(std::string path)
	{
		if (path == "DEFAULT"s) {
			if (std::string gameLangPath = GetLanguageFilePath(GetGameLanguage()); gameLangPath == "NO_FILE"s) {
				if (std::string englishPath = GetLanguageFilePath("ENGLISH"); englishPath == "NO_FILE"s) {
					LoadPlaceholderStrings();
					return;
				} else {
					path = englishPath;
				}
			} else {
				path = gameLangPath;
			}
		}
		
		toml::table result;

		try
		{
			result = toml::parse_file(path);
			logger::debug("Succesfully opened translation file '{}'", path);
		}
		catch(const toml::parse_error& err)
		{
			logger::error("Failed to open TOML file '{}' -> Error description: {}", path, err.description());
			
			LoadPlaceholderStrings();
			return;
		}
		
		std::optional<std::string> language = result["metadata"]["language"].value<std::string>();
		if (!language.has_value()) {
			logger::warn("Translation file '{}' is missing metadata value 'language'.", path);
		} else {
			logger::info("Translation file language: {}", language.value());
		}

		std::optional<std::string> version = result["metadata"]["version"].value<std::string>();
		if (!language.has_value()) {
			logger::warn("Translation file '{}' is missing metadata value 'version'.", path);
		} else {
			logger::info("Translation file version: {}", version.value());
			//TODO: add version checking
		}

		auto translations = result["translation"];
		int mapCount = (int)translationMap.size();
		int tomlCount = (int)translations.as_table()->size();
		int validCount = 0;

		logger::trace("Attempting to read translation file '{}'", path);

		for (auto& [key, val] : translationMap) {
			if (auto tomlVal = translations[key].value<std::string>()) {
				if (!tomlVal.has_value()) {
					logger::error("Key '{}' does not have an assigned value in translation file '{}'", key, path);
					translationMap.at(key).SetText(key);
				} else {
					logger::trace("Setting translation key '{}'", key);
					translationMap.at(key).SetText(tomlVal.value());
					validCount++;
				}
			} else {
				logger::error("Could not find key '{}' in translation file '{}'", key, path);
				translationMap.at(key).SetText(key);
			}
		}

		currentLanguage = path.c_str();

		logger::debug("Finished reading translation file '{}'", path);

		logger::debug("Translation summary:");
		logger::debug("\t-> Required = {}", mapCount);
		logger::debug("\t-> Provided = {}", tomlCount);
		logger::debug("\t-> Accepted = {}", validCount);
		logger::debug("\t-> Missing = {}", mapCount-validCount);

		logger::debug("All valid translations loaded.");
	}

	void LoadPlaceholderStrings()
	{
		logger::debug("Loading placeholder translations...");

		for (const auto& [key, val] : translationMap) {
			translationMap.at(key).SetText(key);
		}

		currentLanguage = "N/A";

		logger::debug("Placeholder translations loaded");
	}

	TranslationVal Get(std::string a_key)
	{
		try
		{
			return translationMap.at(a_key);
		}
		catch(const std::out_of_range& err)
		{
			logger::error("{}", err.what());
			logger::error("Attempted to find translation for key '{}', but was unsuccesful. Most likely cause is key not existing in internal translation map.", a_key);
			return translationMap.at("$Error.KeyNotFound");
		}
	}
}