#pragma once

namespace Logging
{
	void ConvertToLowerCase(std::string& s);
    void Init();
	void UpdateLevel();

	void LogIniError(const char* iniKey);
    void LogIniError(const char* iniKey, const char* extraInfo);

	namespace Testing
	{
		void LogStaminaAVs();

		void LogItemKeywords(RE::TESForm *a_item);
	}
}