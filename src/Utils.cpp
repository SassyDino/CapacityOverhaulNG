#include <spdlog/sinks/basic_file_sink.h>
#include "Utils.h"
#include "WeightHandler.h"
#include "CapacityHandler.h"

namespace Utils
{   
    // Credits to Dylbill: https://github.com/Dylbill-Iroh
    void ConvertToLowerCase(std::string& s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
    }

    void SetupLog() 
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

	void UpdateModules()
	{
		WeightHandler::UpdateFromSettings();
		WeightHandler::UpdateWeightLimit();

		CapacityHandler::Limits::UpdateBaseValues();
		CapacityHandler::Limits::CalculatePlayValues();
		CapacityHandler::Player::UpdateAllCategories();
	}

	void KeywordsToLog(RE::TESForm *a_item)
	{
		auto kwItem = a_item->As<RE::BGSKeywordForm>();

		if (kwItem) {
			auto kwSpan = kwItem->GetKeywords();
			std::vector<const char*> kwList;

			for (RE::BGSKeyword *kw : kwSpan) {
				kwList.push_back(kw->GetFormEditorID());
			}

			logger::debug("'{}' Keywords: [{}]", a_item->GetName(), fmt::join(kwList, ", "));
		} else {
			logger::debug("'{}' has no keywords", a_item->GetName());
		}
	}

	RE::CharEvent* RE3::AsCharEvent(RE::InputEvent *a_event)
	{
		auto eventType = a_event->GetEventType();
		return eventType == RE::INPUT_EVENT_TYPE::kChar ? static_cast<RE::CharEvent*>(a_event) : nullptr;
	}

	const RE::CharEvent* RE3::AsCharEvent(RE::InputEvent *a_event) const
	{
		if (a_event->GetEventType() == RE::INPUT_EVENT_TYPE::kChar) {
			return static_cast<const RE::CharEvent*>(a_event);
		}
		return nullptr;
	}
}