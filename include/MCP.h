#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"

namespace GUI::MCP
{
	class TestSettings final : public Utils::Singleton<TestSettings>
	{
		public:
			bool noHands = true;
			bool noPickup = true;
			bool capacitySkills = true;
			bool capacityQuest = false;
			bool hugeNotShared = true;

			bool vanillaWeight = false;
			bool staminaWeight = true;
			bool levelWeight = true;
			bool raceWeight = true;
	};

	extern bool showSeparateWindow;

	void Register();

	namespace ToggleFeatures {
		void __stdcall Render();
	}

	namespace CapacityConfigs {
		void __stdcall Render();
	}

	namespace WeightConfigs {
		void __stdcall Render();
	}

	namespace AdvWeightConfigs {
		static RE::PlayerCharacter *player;
		static RE::ActorValueOwner *playerAsAV;
		static float playerStamina;
		static int playerLevel;

		void __stdcall Render();
		//float StaminaEquation();
		//float LevelEquation();
	}

	namespace AdvDebug {
		void __stdcall Render();
	}


	
	namespace Example4 {
		inline std::string TitleText = "This is an " + FontAwesome::UnicodeToUtf8(0xf2b4) + " Font Awesome usage example";
		inline std::string Button1Text = FontAwesome::UnicodeToUtf8(0xf0e9) + " Umbrella";
		inline std::string Button2Text = FontAwesome::UnicodeToUtf8(0xf06e) + " Eye";
		void __stdcall Render();
	}
};