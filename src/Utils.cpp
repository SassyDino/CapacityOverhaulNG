#include <spdlog/sinks/basic_file_sink.h>
#include "Utils.h"

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
        auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
        auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
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
            "ActorValue: {}\nBaseActorValue: {}\nPermanentActorValue: {}\nClampedActorValue: {}",
            playerAV, playerBaseAV, playerPermAV, playerClampAV
        );
    }

    float GetRaceWeightMod(RE::FormID formID)
    {
        std::map<RE::FormID, float> raceWeightIndex {
            {0x13743, Settings::fAltmerRaceMod}, {0x13740, Settings::fArgonianRaceMod}, {0x13749, Settings::fBosmerRaceMod},
            {0x13741, Settings::fBretonRaceMod}, {0x13742, Settings::fDunmerRaceMod}, {0x13744, Settings::fImperialRaceMod},
            {0x13745, Settings::fKhajiitRaceMod}, {0x13746, Settings::fNordRaceMod}, {0x13747, Settings::fOrcRaceMod},
            {0x13748, Settings::fRedguardRaceMod}
        };

        float raceWeightMod;
        // Fallback to default race modifier (x1) if player's race does not exist in index (to prevent custom races breaking this mod)
        if (raceWeightIndex.at(formID)) {
            raceWeightMod = raceWeightIndex[formID];
            auto raceName = RE::TESRace::LookupByID(formID)->GetName();
            logger::info("Player race identified as {}. Carry weight limit modifier = x{}", raceName, raceWeightMod);
        } else {
            raceWeightMod = Settings::fDefaultRaceMod;
            logger::warn("Unable to identify player race: reverting to Default race modifier (x{}). A custom player race is the most likely cause for this warning, and if this applies to you then you may ignore this message. Otherwise, this warning may indicate an issue with the mod which you are advised to report.", raceWeightMod);
        }

        return raceWeightMod;
        // if i need more values stored per race, maybe try something like
        // Settings::raceIndex[formID]->WeightMod()
        // with the index's values being classes/structs or something
    }
}