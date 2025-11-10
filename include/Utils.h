#pragma once

namespace Utils
{   
    void ConvertToLowerCase(std::string& s);
    void SetupLog();

    void LogIniError(const char* iniKey);
    void LogIniError(const char* iniKey, const char* extraInfo);
    
    void LogStaminaAVs();

	void UpdateModules();

	void KeywordsToLog(RE::TESForm *a_item);

	void MessageListener(SKSE::MessagingInterface::Message* message);
}