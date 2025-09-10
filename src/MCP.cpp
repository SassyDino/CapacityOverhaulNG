#include "MCP.h"
#include "WeightHandler.h"
#include "Player.h"
#include "Calc.h"
using namespace SKSEMenuFramework;
namespace MCPDraw = MCP_API::ImDrawListManager;

namespace {
	static void CapacityVisualiser()
	{
		ImVec2 p0;
		MCP_API::GetCursorScreenPos(&p0);
		ImDrawList *drawList = MCP_API::GetWindowDrawList();
		MCP_API::ImDrawListManager::AddRect(drawList, p0, ImVec2(p0.x+10,p0.y+10), MCP_API::GetColorU32(ImGuiCol_Border), 0.0f, 0, MCP_API::GetCurrentWindow()->WindowBorderSize);
	}

	static void FileManagerButtons()
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

		if (MCP_API::Button("Reset to Default")) {
			logger::info("Loading default mod settings.");
			Settings::Load(Settings::defaultPath);
		}
	}
}

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
	FileManagerButtons();

	if (MCP_API::CollapsingHeader("Capacity Features", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::Checkbox("Disable Hand Interactions While Over Capacity", Settings::Get<bool*>("bNoHandsOverCap"));
		MCP_API::Checkbox("Prevent Item Pickup While Over Capacity", Settings::Get<bool*>("bPreventPickupOverCap"));
		MCP_API::Checkbox("Skills Affect Capacity Limits", Settings::Get<bool*>("bSkillsAffectCapacity"));
		MCP_API::Checkbox("Quest Item Take Up Capacity", Settings::Get<bool*>("bQuestItemsAffectCapacity"));
	}

	if (MCP_API::CollapsingHeader("Weight Features", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::Checkbox("Use Vanilla Carry Weight", Settings::Get<bool*>("bVanillaWeightLimit"));
		MCP_API::Checkbox("Max Stamina Affects Carry Weight", Settings::Get<bool*>("bStaminaAffectsWeight"));
		MCP_API::Checkbox("Current Level Affects Carry Weight", Settings::Get<bool*>("bLevelAffectsWeight"));
		MCP_API::Checkbox("Player's Race Affects Carry Weight", Settings::Get<bool*>("bRaceAffectsWeight"));
	}
}

void __stdcall GUI::MCP::CapacityConfigs::Render()
{
	if (MCP_API::CollapsingHeader("Category Capacities", ImGuiTreeNodeFlags_DefaultOpen)) {
		//TODO: Provide options to enable/disable misc categories
		//TODO: Also need to see if i can step the sliders, e.g. increment 5 or 10 per step
		MCP_API::SeparatorText("Baseline Settings");
		MCP_API::SliderInt("Huge Items", (int*)(Settings::Get<uint32_t*>("uHugeCapacity")), 1, 10);
		MCP_API::Checkbox("Huge Items Share Storage Space With Other Items", Settings::Get<bool*>("bHugeCapacityShared"));
		//TODO: Need to make availability of large items slide conditional based on above setting
		MCP_API::SliderInt("Large Items", (int*)(Settings::Get<uint32_t*>("uLargeCapacity")), 1, 25);

		MCP_API::SeparatorText("Derived Category Capacities");
		MCP_API::SliderFloat("Large Items Per Huge Space", Settings::Get<float*>("fLargePerHuge"), 1, 25);
		MCP_API::SliderFloat("Medium Items Per Large Space", Settings::Get<float*>("fMediumPerLarge"), 1, 25);
		MCP_API::SliderFloat("Small Items Per Medium Space ", Settings::Get<float*>("fSmallPerMedium"), 1, 25);
		MCP_API::SliderFloat("Tiny Items Per Small Space", Settings::Get<float*>("fTinyPerSmall"), 1, 25);

		MCP_API::SeparatorText("Miscellaneous Categories");
		MCP_API::SliderInt("Alchemical Item Capacity", (int*)(Settings::Get<uint32_t*>("uAlchemyCapacity")), 1, 250);
		MCP_API::SliderInt("Ammo Capacity", (int*)(Settings::Get<uint32_t*>("uAmmoCapacity")), 1, 500);
		MCP_API::SliderInt("Coin Capacity", (int*)(Settings::Get<uint32_t*>("uCoinCapacity")), 1, 2500);
		MCP_API::SliderInt("Coins Per Tiny Space", (int*)(Settings::Get<uint32_t*>("uCoinsPerTiny")), 1, 100);

		//TODO: Add some sort of visualiser or aid for explaining how this all works without needing a lot of words
		CapacityVisualiser();
	}

	if (MCP_API::CollapsingHeader("Item Category Definitions", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::SliderFloat("Huge Item Weight", Settings::Get<float*>("fHugeItemWeight"), 1, 50);
		MCP_API::SliderFloat("Large Item Weight", Settings::Get<float*>("fLargeItemWeight"), 1, 25);
		MCP_API::SliderFloat("Medium Item Weight", Settings::Get<float*>("fMediumItemWeight"), 1, 25);
		MCP_API::SliderFloat("Small Item Weight", Settings::Get<float*>("fSmallItemWeight"), 1, 10);
	}
}

void __stdcall GUI::MCP::WeightConfigs::Render()
{
	if (MCP_API::CollapsingHeader("General Weight Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::SliderInt("Base Carry Weight", (int*)(Settings::Get<uint32_t*>("uBaseCarryWeight")), 10, 250);

		MCP_API::SeparatorText("Stamina-based Carry Weight Modification");
		MCP_API::SliderFloat("Stamina Weight Bonus Modifier", Settings::Get<float*>("fStaminaWeightMod"), 0, 5);
		MCP_API::Checkbox("Use Simple Calculation For Stamina Weight Bonus", Settings::Get<bool*>("bStaminaWeightSimple"));
		MCP_API::SliderFloat("Bonus Carry Weight Per Stamina Point", Settings::Get<float*>("fWeightPerStamina"), 0, 5);

		MCP_API::SeparatorText("Level-based Carry Weight Modification");
		MCP_API::SliderFloat("Level Weight Bonus Modifier", Settings::Get<float*>("fLevelWeightMod"), 0, 5);
		MCP_API::Checkbox("Use Simple Calculation For Level Weight Bonus ", Settings::Get<bool*>("bLevelWeightSimple"));
		MCP_API::SliderFloat("Bonus Carry Weight Per Level", Settings::Get<float*>("fWeightPerStamina"), 0, 10);
	}

	if (MCP_API::CollapsingHeader("Race-Based Carry Weight Modifiers", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::SeparatorText("Vanilla Races");
		MCP_API::SliderFloat("Altmer", Settings::Get<float*>("fAltmerRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Argonian", Settings::Get<float*>("fArgonianRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Bosmer", Settings::Get<float*>("fBosmerRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Breton", Settings::Get<float*>("fBretonRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Dunmer", Settings::Get<float*>("fDunmerRaceMod"), 0.5, 2.5);
		
		MCP_API::SliderFloat("Imperial", Settings::Get<float*>("fImperialRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Khajiit", Settings::Get<float*>("fKhajiitRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Nord", Settings::Get<float*>("fNordRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Orc", Settings::Get<float*>("fOrcRaceMod"), 0.5, 2.5);
		MCP_API::SliderFloat("Redguard", Settings::Get<float*>("fRedguardRaceMod"), 0.5, 2.5);
		MCP_API::SeparatorText("Extra Races");
		MCP_API::SliderFloat("Default Race", Settings::Get<float*>("fDefaultRaceMod"), 0.5, 2.5);
	}
}

void __stdcall GUI::MCP::AdvWeightConfigs::Render()
{
	static float *stamRateTemp = Settings::Get<float*>("fStaminaWeightRate");
	static int *stamPivotTemp = reinterpret_cast<int*>(Settings::Get<uint32_t*>("uStaminaWeightPivot"));
	static float *lvlRateTemp = Settings::Get<float*>("fLevelWeightRate");
	static int *lvlPivotTemp = reinterpret_cast<int*>(Settings::Get<uint32_t*>("uLevelWeightPivot"));
	static uint32_t *baseCarryTemp = Settings::Get<uint32_t*>("uBaseCarryWeight");

	//static float maxGradStam = Player::CalcStamAtMaxGrad(stamRateTemp, stamPivotTemp, baseCarryTemp);
	///static float maxGradLvl = Player::CalcLevelAtMaxGrad(lvlRateTemp, lvlPivotTemp, baseCarryTemp);
	//TODO: Make these into adjustable values, controllable in the MCP to change the graph plotspaces
	static int stamMaxX = 250;
	static int lvlMaxX = 100;

	static int stamPlotX = 250;
	static int stamPlotY = 500;
	static int lvlPlotX = 100;
	static int lvlPlotY = 500;


	//logger::debug("{} {} {} {} {} {} {}", stamRateTemp, stamPivotTemp, lvlRateTemp, lvlPivotTemp, baseCarryTemp, maxGradStam, maxGradLvl);

	struct Funcs
	{
		static float StamBonus(void*, int i) {
			return Calc::StaminaWeightBonus(i, *stamRateTemp, *stamPivotTemp, *baseCarryTemp, Player::CalcStamAtMaxGrad(*stamRateTemp, *stamPivotTemp, *baseCarryTemp));
		}
		static float LvlBonus(void*, int i) {
			return Calc::LevelWeightBonus(i, *lvlRateTemp, *lvlPivotTemp, *baseCarryTemp, Player::CalcLevelAtMaxGrad(*lvlRateTemp, *lvlPivotTemp, *baseCarryTemp));
		}
	};

	if (MCP_API::CollapsingHeader("Parameters for Stamina-Based Bonus to Carry Weight", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::SeparatorText("Configure Parameters");
		MCP_API::SliderFloat("Stamina Bonus Growth Rate", stamRateTemp, -1.0f, 0.99f);
		MCP_API::SliderInt("Stamina Bonus Pivot", stamPivotTemp, 1, 250);

		MCP_API::SeparatorText("Preview");
		float (*func1)(void*, int) = Funcs::StamBonus;
		MCP_API::PlotLines("", func1, NULL, stamPlotX, 0, NULL, 0.0f, static_cast<float>(stamPlotY), SKSEMenuFramework::ImVec2(0, 320));
		
		MCP_API::SameLine();
		MCP_API::VSliderInt("##X-Axis (Stamina)", ImVec2(80, 320), &stamPlotX, 150, 500, "X: %d");
		MCP_API::SameLine();
		MCP_API::VSliderInt("##Y-Axis (Bonus)", ImVec2(80, 320), &stamPlotY, 200, 2500, "Y: %d");
	}

	if (MCP_API::CollapsingHeader("Parameters for Level-Based Bonus to Carry Weight", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::SeparatorText("Configure Parameters");
		MCP_API::SliderFloat("Level Bonus Growth Rate", lvlRateTemp, -1.0f, 0.99f);
		MCP_API::SliderInt("Level Bonus Pivot", lvlPivotTemp, 1, 100);

		MCP_API::SeparatorText("Preview");
		float (*func2)(void*, int) = Funcs::LvlBonus;
		MCP_API::PlotLines("", func2, NULL, lvlPlotX, 0, NULL, 0.0f, static_cast<float>(lvlPlotY), SKSEMenuFramework::ImVec2(0, 320));

		MCP_API::SameLine();
		MCP_API::VSliderInt("##X-Axis (Level)", ImVec2(80, 320), &lvlPlotX, 50, 200, "X: %d");
		MCP_API::SameLine();
		MCP_API::VSliderInt("##Y-Axis (Bonus)", ImVec2(80, 320), &lvlPlotY, 200, 2500, "Y: %d");
	}

	if (MCP_API::CollapsingHeader("Combined Result of all Carry Weight Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		MCP_API::SeparatorText("Preview");
	}
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