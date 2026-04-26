#include "MCPWidgets.h"
#include "MCP.h"

namespace MCP_API = ImGuiMCP::ImGui;

namespace GUI::MCP
{
	//TODO: Custom styling, perhaps
	void FileManagerButtons()
	{
		if (MCP_API::Button("Save")) {
			logger::info("Saving current user settings.");
			Settings::SaveToFile(Settings::userPath);
			Settings::Init();
		}
		MCP_API::SameLine();

		if (MCP_API::Button("Load")) {
			logger::info("Loading saved user settings.");
			Settings::Init();
		}
		MCP_API::SameLine();

		//TODO: Add a modal to prevent accidental clicks
		if (MCP_API::Button("Reset to Default")) {
			logger::info("Loading default mod settings.");
			Settings::Load(Settings::defaultPath);
		}
	}

	void LanguageSelector()
	{
		std::string defaultText = "No Language Selected";
		std::filesystem::path filePath = R"(Data\Interface\Translations\)";

		std::vector<std::string> availableFiles = {defaultText};
		auto selected = 0;
		const char* previewLang = availableFiles[selected].c_str();

		// Button to refresh & search folder to get available translation files
		// Code is based on clib_util::distribution::get_config_paths()
		if (MCP_API::Button("Locate Translation Files")) {
			for (const auto it = std::filesystem::directory_iterator(filePath); const auto& entry : it) {
				if (entry.exists()) {
					if (const auto& path = entry.path(); !path.empty() && path.extension() == ".toml") {
						if (const auto& fileName = entry.path().string(); fileName.rfind("CapacityOverhaulNG_") != std::string::npos) {
							availableFiles.push_back(fileName);
						}
					}
				}
			}
		}

		// Dropdown containing available language files
		if (MCP_API::BeginCombo("Available Languages", previewLang)) {
			for (int n = 0; n < availableFiles.size(); n++) {
				const bool is_selected = (selected == n);
				if (MCP_API::Selectable(availableFiles[n].c_str(), is_selected)) {
					selected = n;
				}

				if (is_selected) {
					MCP_API::SetItemDefaultFocus();
				}
			}

			MCP_API::EndCombo();
		}

		// Button to confirm & apply change
		if (MCP_API::Button("Apply Language File")) {
			if (availableFiles[selected] == defaultText) {
				MCP_API::Text("Select a language file to apply.");
			} else {
				MCP_API::Text("Changing language...");

				Settings::Get<std::string>("sLanguage") = Lang::GetLanguageFromFileName(availableFiles[selected]);

				Lang::LoadTranslations(filePath.string() + availableFiles[selected]);

				MCP_API::Text("Language file applied.");
			}
		}
	}

	void RefreshLogLevelButton()
	{
		if (MCP_API::Button("Update From Settings")) { Utils::UpdateLogLevel(); }
	}
}