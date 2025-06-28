#pragma once
namespace Utils
{
    void LogIniError(const char* iniKey);
    void LogIniError(const char* iniKey, const char* extraInfo);
    
    void LogStaminaAVs();

    float GetRaceWeightMod(RE::FormID formID);
}