#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"

namespace GUI::MCP
{
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
		void __stdcall Render();
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