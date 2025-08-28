#include "MCP.h"
#include "Utils.h"
using namespace SKSEMenuFramework;

void GUI::MCP::Register()
{
    if (!SKSEMenuFramework::IsInstalled()) {
		logger::warn("SKSE Menu Framework is not installed: cannot register Capacity Overhaul NG's Mod Control Panel");
        return;
    }
    SKSEMenuFramework::SetSection("Capacity Overhaul NG");

	SKSEMenuFramework::AddSectionItem("General Settings/Toggle Features", ToggleFeatures::Render);
	SKSEMenuFramework::AddSectionItem("General Settings/Capacity System Configuration", CapacityConfigs::Render);
	SKSEMenuFramework::AddSectionItem("General Settings/Carry Weight Settings", WeightConfigs::Render);
	SKSEMenuFramework::AddSectionItem("Advanced Settings/Complex Carry Weight System", AdvWeightConfigs::Render);
	SKSEMenuFramework::AddSectionItem("Advanced Settings/Debug Settings", AdvDebug::Render);
    SKSEMenuFramework::AddSectionItem("Font Awesome", Example4::Render);
}

void __stdcall GUI::MCP::ToggleFeatures::Render()
{
	MCP_API::Text("Capacity Features");
	//MCP_API::Checkbox("Disable Hand Interactions While Over Capacity", Settings::bNoHandsOverCap);

	MCP_API::Text("Weight Features");
}

void __stdcall GUI::MCP::CapacityConfigs::Render()
{

}

void __stdcall GUI::MCP::WeightConfigs::Render()
{

}

void __stdcall GUI::MCP::AdvWeightConfigs::Render()
{

}

void __stdcall GUI::MCP::AdvDebug::Render()
{

}

void __stdcall GUI::MCP::Example4::Render()
{
    FontAwesome::PushBrands();
    MCP_API::Text(TitleText.c_str());
    FontAwesome::Pop();

    FontAwesome::PushSolid();
    MCP_API::Button(Button1Text.c_str());
    FontAwesome::Pop();

    MCP_API::SameLine();

    FontAwesome::PushRegular();
    MCP_API::Button(Button2Text.c_str());
    FontAwesome::Pop();
}