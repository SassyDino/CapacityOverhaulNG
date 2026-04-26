#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"

namespace GUI
{
	struct Assets
	{
		static const std::string dir;
		static const std::string type;

		static ImGuiMCP::ImTextureID CheckboxEmpty;
		static ImGuiMCP::ImTextureID CheckboxFilled;

		static ImGuiMCP::ImTextureID SeparatorLeft;
		static ImGuiMCP::ImTextureID SeparatorRight;

		static ImGuiMCP::ImTextureID HeaderBackLeft;
		static ImGuiMCP::ImTextureID HeaderBackRight;
		static ImGuiMCP::ImTextureID HeaderFrontLeft;
		static ImGuiMCP::ImTextureID HeaderFrontRight;
		static ImGuiMCP::ImTextureID HeaderArrowDown;
		static ImGuiMCP::ImTextureID HeaderArrowUpLeft;
		static ImGuiMCP::ImTextureID HeaderArrowUpRight;

		static ImGuiMCP::ImTextureID ScrollbarLeft;
		static ImGuiMCP::ImTextureID ScrollbarRight;
		static ImGuiMCP::ImTextureID ScrollbarSelector;

		static float defaultFontSize;

		static std::string GetTexPath(std::string a_filename);
		static void LoadTextures();
	};
}