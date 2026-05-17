#include "Logging.h"
#include "Version.h"

#include <spdlog/sinks/basic_file_sink.h>

namespace Logging
{
	// Credits to Dylbill: https://github.com/Dylbill-Iroh
    void ConvertToLowerCase(std::string& s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
    }

    void Init() 
    {
        auto logsFolder = logger::log_directory();
        if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");

        //fix the bug on AE where logs are written to "My Games/Skyrim.INI/ instead of "My Games/Skyrim Special Edition/SKSE
        // Credits to Dylbill: https://github.com/Dylbill-Iroh
        std::filesystem::path logsFolderPath = logsFolder.value();
        std::string sLogPath = logsFolderPath.generic_string(); 
        ConvertToLowerCase(sLogPath);

        if (sLogPath.find("my games") != std::string::npos) {
            std::string parentPathName = logsFolderPath.filename().string();
            ConvertToLowerCase(parentPathName);
            while (logsFolderPath.has_parent_path() && parentPathName != "my games") {
                logsFolderPath = logsFolderPath.parent_path();
                parentPathName = logsFolderPath.filename().string();
                ConvertToLowerCase(parentPathName);
            }

            if (parentPathName == "my games") {
                logsFolderPath.append("Skyrim Special Edition");
                logsFolderPath.append("SKSE");
            }
            else {
                logsFolderPath = logsFolder.value();
            }
        }

        auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();

		std::string pluginFileName = std::format("{}.log", pluginName);
		logsFolderPath.append(pluginFileName);

        auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logsFolderPath.string(), true);
        auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
        spdlog::set_default_logger(std::move(loggerPtr));
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);

		spdlog::set_pattern("[%Y-%m-%d][%H:%M:%S.%e] [%L] [%s:%#] %v");

		logger::info("{}", std::string(50, '='));
		logger::info("||");
		logger::info("||\t{} v{}.{}.{}", Version::NAME, Version::MAJOR, Version::MINOR, Version::PATCH);
		logger::info("||\tAuthor: {}", Version::AUTHOR);
		logger::info("||\tBuild Date: {} {}", Version::DATE, Version::TIME);
		logger::info("||");
		logger::info("{}", std::string(50, '='));
    }

	void UpdateLevel()
	{
		auto currentLogLevel = spdlog::get_level();
		auto settingLogLevel = spdlog::level::level_enum(Settings::Get<uint32_t>("uLogLevel"));

		if ((settingLogLevel != spdlog::level::info) && (settingLogLevel != spdlog::level::debug) && (settingLogLevel != spdlog::level::trace)) {
			logger::error("Invalid Setting -> 'uLogLevel' must be an unsigned integer equal to 0 (trace), 1 (debug), or 2 (info) [Currently set to '{}']", Settings::Get<uint32_t>("uLogLevel"));
			logger::info("Logger left at level '{}'", spdlog::level::to_string_view(currentLogLevel));
			return;
		}

		if (settingLogLevel != currentLogLevel) {
			spdlog::set_level(spdlog::level::level_enum(Settings::Get<uint32_t>("uLogLevel")));
			logger::info("Setting logger to level: {}", spdlog::level::to_string_view(spdlog::get_level()));
		} else {
			logger::info("Logger already set to level '{}'. No change made.", spdlog::level::to_string_view(currentLogLevel));
		}

		switch (spdlog::get_level())
		{
			case spdlog::level::info:
				spdlog::set_pattern("[%Y-%m-%d][%H:%M:%S] [%L] %v");
				break;
			case spdlog::level::debug:
				spdlog::set_pattern("[%Y-%m-%d][%H:%M:%S.%e] [%L] %v");
				break;
			case spdlog::level::trace:
				spdlog::set_pattern("[%Y-%m-%d][%H:%M:%S.%e] [%l] [%s:%#] %v");
				break;
			default:
				spdlog::set_pattern("[%Y-%m-%d][%H:%M:%S.%e] [[%L]] %v");
		}
	}

	void LogIniError(const char* iniKey) 
    {
        logger::error("Invalid value provided in 'Data/MCM/Settings/CapacityOverhaulNG.ini'...\n"
            "Ini key(s): {}\n",
            iniKey
        );
    }

    void LogIniError(const char* iniKey, const char* extraInfo) 
    {
        logger::error("Invalid value provided in 'Data/MCM/Settings/CapacityOverhaulNG.ini'...\n"
            "Ini key(s): {}\n"
            "Info: {}",
            iniKey, extraInfo
        );
    }

	namespace Testing
	{
		void LogStaminaAVs()
		{
			// NOTE: This is just for testing, probably try and remember to delete once done using.
			auto player = RE::PlayerCharacter::GetSingleton();
			auto playerAsAV = player->AsActorValueOwner();
			auto playerAV = playerAsAV->GetActorValue(RE::ActorValue::kStamina);
			auto playerBaseAV = playerAsAV->GetBaseActorValue(RE::ActorValue::kStamina);
			auto playerPermAV = playerAsAV->GetPermanentActorValue(RE::ActorValue::kStamina);
			auto playerClampAV = playerAsAV->GetClampedActorValue(RE::ActorValue::kStamina);
			logger::debug("Actor value testing:\n"
				"ActorValue: {}\nBaseActorValue (): {}\nPermanentActorValue (Max): {}\nClampedActorValue (Curr.): {}",
				playerAV, playerBaseAV, playerPermAV, playerClampAV
			);
		}

		void LogItemKeywords(RE::TESForm *a_item)
		{
			auto kwItem = a_item->As<RE::BGSKeywordForm>();

			if (kwItem) {
				auto kwSpan = kwItem->GetKeywords();
				std::vector<const char*> kwList;

				for (RE::BGSKeyword *kw : kwSpan) {
					kwList.push_back(kw->GetFormEditorID());
				}

				//logger::debug("'{}' Keywords: [{}]", a_item->GetName(), fmt::join(kwList, ", "));
			} else {
				logger::debug("'{}' has no keywords", a_item->GetName());
			}
		}
	}
}