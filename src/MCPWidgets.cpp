#include "MCPWidgets.h"
#include "MCP.h"
#include "Logging.h"

namespace MCP_API = ImGuiMCP::ImGui;

namespace GUI::MCP
{
	//TODO: Custom styling, perhaps
	void FileManagerButtons()
	{
		if (MCP_API::Button("$MCP.Widgets.FileManagerButtons.Save"_tr)) {
			logger::info("Saving current user settings.");
			Settings::SaveToFile(Settings::userPath);
			Settings::Init();
			Utils::UpdateModules();
		}
		MCP_API::SameLine();

		if (MCP_API::Button("$MCP.Widgets.FileManagerButtons.Load"_tr)) {
			logger::info("Loading saved user settings.");
			Settings::Init();
			Utils::UpdateModules();
		}
		MCP_API::SameLine();

		//TODO: Add a modal to prevent accidental clicks
		if (MCP_API::Button("$MCP.Widgets.FileManagerButtons.Reset"_tr)) {
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
		if (MCP_API::Button("$MCP.Widgets.Language.Locate"_tr)) {
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
		if (MCP_API::BeginCombo("$MCP.Widgets.Language.Available"_tr, previewLang)) {
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
		if (MCP_API::Button("$MCP.Widgets.Language.Apply"_tr)) {
			if (availableFiles[selected] == defaultText) {
				MCP_API::Text("$MCP.Widgets.Language.Select"_tr);
			} else {
				MCP_API::Text("$MCP.Widgets.Language.Changing"_tr);

				Settings::Get<std::string>("sLanguage") = Lang::GetLanguageFromFileName(availableFiles[selected]);

				Lang::LoadTranslations(filePath.string() + availableFiles[selected]);

				MCP_API::Text("$MCP.Widgets.Language.Applied"_tr);
			}
		}
	}

	void RefreshLogLevelButton()
	{
		if (MCP_API::Button("$MCP.Widgets.RefreshLogLevelButton.Update"_tr)) { Logging::UpdateLevel(); }
	}
}