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

	struct RE3
	{
		RE::CharEvent* AsCharEvent(RE::InputEvent *a_event);
		const RE::CharEvent* AsCharEvent(RE::InputEvent *a_event) const;
	};

	void MessageListener(SKSE::MessagingInterface::Message* message);

	template <class T>
	class Singleton
	{
		public:
			static T* GetSingleton()
			{
				static T singleton;
				return std::addressof(singleton);
			}
		
			protected:
				Singleton() = default;
				~Singleton() = default;
				Singleton(const Singleton&) = delete;
				Singleton(Singleton&&) = delete;
				Singleton& operator=(const Singleton&) = delete;
				Singleton& operator=(Singleton&&) = delete;
	};
}