#include "GUIAssets.h"
#include "MCP.h"

namespace SKSEMF = SKSEMenuFramework;

namespace GUI
{
	const std::string Assets::dir = ".\\Data\\Interface\\CapacityOverhaulNG\\Icons\\";
	const std::string Assets::type = "png";

	ImGuiMCP::ImTextureID Assets::CheckboxEmpty;
	ImGuiMCP::ImTextureID Assets::CheckboxFilled;

	ImGuiMCP::ImTextureID Assets::SeparatorLeft;
	ImGuiMCP::ImTextureID Assets::SeparatorRight;

	ImGuiMCP::ImTextureID Assets::HeaderBackLeft;
	ImGuiMCP::ImTextureID Assets::HeaderBackRight;
	ImGuiMCP::ImTextureID Assets::HeaderFrontLeft;
	ImGuiMCP::ImTextureID Assets::HeaderFrontRight;
	ImGuiMCP::ImTextureID Assets::HeaderArrowDown;
	ImGuiMCP::ImTextureID Assets::HeaderArrowUpLeft;
	ImGuiMCP::ImTextureID Assets::HeaderArrowUpRight;

	ImGuiMCP::ImTextureID Assets::ScrollbarLeft;
	ImGuiMCP::ImTextureID Assets::ScrollbarRight;
	ImGuiMCP::ImTextureID Assets::ScrollbarSelector;

	float Assets::defaultFontSize;

	std::string Assets::GetTexPath(std::string a_filename)
	{
		std::string path = std::string(dir + a_filename + "." + type);
		logger::debug("{}", path);
		return path;
	}

	void Assets::LoadTextures()
	{
		if (!SKSEMenuFramework::IsInstalled()) {
			logger::warn("SKSE Menu Framework is not installed: loading textures skipped.");
			return;
		}

		//NOTE: Loading textures here rather than in the actual widget function (and making use of the caching) because otherwise the game freezes for a brief moment when opening the CONG MCP while it loads the textures for the first time.
		logger::info("Loading textures...");

		CheckboxEmpty = SKSEMF::LoadTexture(GetTexPath("Checkbox_Empty"));
		CheckboxFilled = SKSEMF::LoadTexture(GetTexPath("Checkbox_Filled"));

		SeparatorLeft = SKSEMF::LoadTexture(GetTexPath("Separator_Knot_L"));
		SeparatorRight = SKSEMF::LoadTexture(GetTexPath("Separator_Knot_R"));

		HeaderBackLeft = SKSEMF::LoadTexture(GetTexPath("Header_Back_L"));
		HeaderBackRight = SKSEMF::LoadTexture(GetTexPath("Header_Back_R"));
		HeaderFrontLeft = SKSEMF::LoadTexture(GetTexPath("Header_Detail_L"));
		HeaderFrontRight = SKSEMF::LoadTexture(GetTexPath("Header_Detail_R"));
		HeaderArrowDown = SKSEMF::LoadTexture(GetTexPath("Header_Arrow_Down"));
		HeaderArrowUpLeft = SKSEMF::LoadTexture(GetTexPath("Header_Arrow_Up_L"));
		HeaderArrowUpRight = SKSEMF::LoadTexture(GetTexPath("Header_Arrow_Up_R"));

		ScrollbarLeft = SKSEMF::LoadTexture(GetTexPath("Scrollbar_Arrow_Left"));
		ScrollbarRight = SKSEMF::LoadTexture(GetTexPath("Scrollbar_Arrow_Right"));
		ScrollbarSelector = SKSEMF::LoadTexture(GetTexPath("Scrollbar_Selector"));

		logger::info("Textures loaded!\n{}", std::string(100, '='));
	}
}