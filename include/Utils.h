#pragma once

namespace Utils
{   
    void ConvertToLowerCase(std::string& s);
    void SetupLog();

    void LogIniError(const char* iniKey);
    void LogIniError(const char* iniKey, const char* extraInfo);
    
    void LogStaminaAVs();

    float GetRaceWeightMod(RE::FormID formID);

	void UpdateModules();

	void KeywordsToLog(RE::TESForm *a_item);
}