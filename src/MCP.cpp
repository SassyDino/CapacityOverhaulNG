#include "MCP.h"
#include "WeightHandler.h"
#include "Player.h"
#include "Calc.h"
#include "GUICustom.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <implot.h>
using namespace SKSEMenuFramework;
namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

bool GUI::MCP::Testing::testBool = false;
int GUI::MCP::Testing::testInt = 0;
float GUI::MCP::Testing::testFloat = 0.0f;

bool GUI::MCP::showSeparateWindow = false;

void GUI::MCP::Register()
{
    if (!SKSEMenuFramework::IsInstalled()) {
		logger::warn("SKSE Menu Framework is not installed: cannot register Capacity Overhaul NG's Mod Control Panel");
        return;
    }

	//ImPlot::SetImGuiContext(IMGUI_API::GImGui);
	//ImPlot::CreateContext();

    SKSEMenuFramework::SetSection("Capacity Overhaul NG");

	SKSEMenuFramework::AddSectionItem("General Settings/Toggle Features", ToggleFeatures::Render);
	SKSEMenuFramework::AddSectionItem("General Settings/Capacity System Configuration", CapacityConfigs::Render);
	SKSEMenuFramework::AddSectionItem("General Settings/Carry Weight Settings", WeightConfigs::Render);
	SKSEMenuFramework::AddSectionItem("General Settings/Interface Settings", UIConfigs::Render);
	SKSEMenuFramework::AddSectionItem("Advanced Settings/Complex Carry Weight System", AdvWeightConfigs::Render);
	SKSEMenuFramework::AddSectionItem("Advanced Settings/Debug Settings", AdvDebug::Render);
    SKSEMenuFramework::AddSectionItem("Menu Testing", Example4::Render);

	//ImPlot::DestroyContext();
}

void __stdcall GUI::MCP::ToggleFeatures::Render()
{
	FileManagerButtons();

	if (CustomHeader("Capacity Features")) {
		CustomCheckbox("Disable Hands/Weapons While Over Capacity", Settings::Get<bool*>("bNoHandsOverCap"));
		CustomCheckbox("Prevent Item Pickup While Over Capacity", Settings::Get<bool*>("bPreventPickupOverCap"));
		CustomCheckbox("Prevent Opening Containers While Over Capacity", Settings::Get<bool*>("bNoContainerAccessOverCap"));
		CustomCheckbox("Prevent Object Interactions While Over Capacity", Settings::Get<bool*>("bPreventInteractionsOverCap"));
		CustomCheckbox("Skills Affect Capacity Limits", Settings::Get<bool*>("bSkillsAffectCapacity"));
		CustomCheckbox("Quest Item Take Up Capacity", Settings::Get<bool*>("bQuestItemsAffectCapacity"));
		CustomCheckbox("Weapons & Shields Have Independent Capacity Thresholds", Settings::Get<bool*>("bSeparateWeaponCategories"));
	}

	if (CustomHeader("Weight Features")) {
		CustomCheckbox("Use Vanilla Carry Weight", Settings::Get<bool*>("bVanillaWeightLimit"));
		CustomCheckbox("Max Stamina Affects Carry Weight", Settings::Get<bool*>("bStaminaAffectsWeight"));
		CustomCheckbox("Current Level Affects Carry Weight", Settings::Get<bool*>("bLevelAffectsWeight"));
		CustomCheckbox("Player's Race Affects Carry Weight", Settings::Get<bool*>("bRaceAffectsWeight"));
	}
}

void __stdcall GUI::MCP::CapacityConfigs::Render()
{
	if (CustomHeader("Category Capacities")) {
		//TODO: Provide options to enable/disable misc categories
		//TODO: Also need to see if i can step the sliders, e.g. increment 5 or 10 per step
		CustomSeparator("Baseline Settings");
		MCP_API::SliderInt("Huge Item Capacity", (int*)(Settings::Get<uint32_t*>("uHugeCapacity")), 1, 10);
		CustomCheckbox("Huge Items Share Storage Space With Other Items", Settings::Get<bool*>("bHugeCapacityShared"));
		//TODO: Need to make availability of large items slide conditional based on above setting
		MCP_API::SliderInt("Large Item Capacity", (int*)(Settings::Get<uint32_t*>("uLargeCapacity")), 1, 25);

		CustomSeparator("Derived Category Capacities");
		MCP_API::SliderFloat("Large Items Per Huge Space", Settings::Get<float*>("fLargePerHuge"), 1, 25);
		MCP_API::SliderFloat("Medium Items Per Large Space", Settings::Get<float*>("fMediumPerLarge"), 1, 25);
		MCP_API::SliderFloat("Small Items Per Medium Space", Settings::Get<float*>("fSmallPerMedium"), 1, 25);
		MCP_API::SliderFloat("Tiny Items Per Small Space", Settings::Get<float*>("fTinyPerSmall"), 1, 25);

		CustomSeparator("Miscellaneous Categories");
		MCP_API::SliderInt("Alchemical Item Capacity", (int*)(Settings::Get<uint32_t*>("uAlchemyCapacity")), 1, 250);
		MCP_API::SliderInt("Ammo Capacity", (int*)(Settings::Get<uint32_t*>("uAmmoCapacity")), 1, 500);
		MCP_API::SliderInt("Coin Capacity", (int*)(Settings::Get<uint32_t*>("uCoinCapacity")), 1, 2500);
		MCP_API::SliderInt("Coins Per Tiny Space", (int*)(Settings::Get<uint32_t*>("uCoinsPerTiny")), 1, 100);
		CustomCheckbox("Gems Use The Same Storage Category As Coins", Settings::Get<bool*>("bGemsInCoinCategory"));
		MCP_API::SliderInt("Amount Of Coin Storage Used By Each Gem", (int*)(Settings::Get<uint32_t*>("uCoinCapacityPerGem")), 1, 100);

		CustomSeparator("Weapon Capacities");
		MCP_API::SliderInt("Large Weapon Capacity", (int*)(Settings::Get<uint32_t*>("uLargeWeaponCapacity")), 0, 25);
		MCP_API::SliderInt("Medium Weapon Capacity", (int*)(Settings::Get<uint32_t*>("uMediumWeaponCapacity")), 0, 25);
		MCP_API::SliderInt("Small Weapon Capacity", (int*)(Settings::Get<uint32_t*>("uSmallWeaponCapacity")), 0, 25);
		MCP_API::SliderInt("Ranged Weapon Capacity", (int*)(Settings::Get<uint32_t*>("uRangedWeaponCapacity")), 0, 25);
		MCP_API::SliderInt("Shield Capacity", (int*)(Settings::Get<uint32_t*>("uShieldCapacity")), 0, 25);

		//TODO: Add some sort of visualiser or aid for explaining how this all works without needing a lot of words
		if (CustomHeader("Capacity Visualiser")) {
			if (Settings::settingsLoaded && RE::PlayerCharacter::GetSingleton()) {
				if (MCP_API::Button("Open Visualiser in Separate Window")) {
					showSeparateWindow = true;
				}

				if (showSeparateWindow) {
					MCP_API::Text("Visualiser Opened in Separate Window");
					CapacityVisualiserWindow(&showSeparateWindow);
				} else {
					CapacityVisualiser();
				}
			} else {
				MCP_API::Text("To view the capacity visualiser, player character must be present and mod settings must be loaded.");
			}
		}
	}

	if (CustomHeader("Item Category Definitions")) {
		MCP_API::SliderFloat("Huge Item Weight", Settings::Get<float*>("fHugeItemWeight"), 1, 50);
		MCP_API::SliderFloat("Large Item Weight", Settings::Get<float*>("fLargeItemWeight"), 1, 25);
		MCP_API::SliderFloat("Medium Item Weight", Settings::Get<float*>("fMediumItemWeight"), 1, 25);
		MCP_API::SliderFloat("Small Item Weight", Settings::Get<float*>("fSmallItemWeight"), 1, 10);
	}
}

void __stdcall GUI::MCP::WeightConfigs::Render()
{
	if (CustomHeader("General Weight Settings")) {
		MCP_API::SliderInt("Base Carry Weight", (int*)(Settings::Get<uint32_t*>("uBaseCarryWeight")), 10, 250);

		CustomSeparator("Stamina-based Carry Weight Modification");
		MCP_API::SliderFloat("Stamina Weight Bonus Modifier", Settings::Get<float*>("fStaminaWeightMod"), 0, 5);
		CustomCheckbox("Use Simple Calculation For Stamina Weight Bonus", Settings::Get<bool*>("bStaminaWeightSimple"));
		MCP_API::SliderFloat("Bonus Carry Weight Per Stamina Point", Settings::Get<float*>("fWeightPerStamina"), 0, 5);

		CustomSeparator("Level-based Carry Weight Modification");
		MCP_API::SliderFloat("Level Weight Bonus Modifier", Settings::Get<float*>("fLevelWeightMod"), 0, 5);
		CustomCheckbox("Use Simple Calculation For Level Weight Bonus", Settings::Get<bool*>("bLevelWeightSimple"));
		MCP_API::SliderFloat("Bonus Carry Weight Per Level", Settings::Get<float*>("fWeightPerStamina"), 0, 10);
	}

	if (CustomHeader("Race-Based Carry Weight Modifiers")) {
		CustomSeparator("Vanilla Races");
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
		CustomSeparator("Extra Races");
		MCP_API::SliderFloat("Default Race", Settings::Get<float*>("fDefaultRaceMod"), 0.5, 2.5);
	}
}

void __stdcall GUI::MCP::UIConfigs::Render()
{
	CustomSeparator("Config Menu Appearance");
	CustomCheckbox("Enable Custom Menu Styling", Settings::Get<bool*>("bCustomMenuStyling"));

	CustomSeparator("Language");
	CustomCheckbox("Manually Choose Language", Settings::Get<bool*>("bOverrideLanguage"));

	if (!Settings::Get<bool>("bOverrideLanguage")) { MCP_API::BeginDisabled(); }
	LanguageSelector();
	if (!Settings::Get<bool>("bOverrideLanguage")) { MCP_API::EndDisabled(); }
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

	struct Funcs
	{
		static float StamBonus(void*, int i) {
			return Calc::StaminaWeightBonus(i, *stamRateTemp, *stamPivotTemp, *baseCarryTemp, PlayerStatus::CalcStamAtMaxGrad(*stamRateTemp, *stamPivotTemp, *baseCarryTemp));
		}
		static float LvlBonus(void*, int i) {
			return Calc::LevelWeightBonus(i, *lvlRateTemp, *lvlPivotTemp, *baseCarryTemp, PlayerStatus::CalcLevelAtMaxGrad(*lvlRateTemp, *lvlPivotTemp, *baseCarryTemp));
		}
	};

	if (CustomHeader("Parameters for Stamina-Based Bonus to Carry Weight")) {
		CustomSeparator("Configure Parameters");
		MCP_API::SliderFloat("Stamina Bonus Growth Rate", stamRateTemp, -1.0f, 0.99f);
		MCP_API::SliderInt("Stamina Bonus Pivot", stamPivotTemp, 1, 250);

		CustomSeparator("Preview");
		float (*func1)(void*, int) = Funcs::StamBonus;
		MCP_API::PlotLines("", func1, NULL, stamPlotX, 0, NULL, 0.0f, static_cast<float>(stamPlotY), SKSEMenuFramework::ImVec2(0, 320));
		
		MCP_API::SameLine();
		MCP_API::VSliderInt("##X-Axis Bounds (Stamina)", SKSEMenuFramework::ImVec2(80, 320), &stamPlotX, 150, 500, "X: %d");
		MCP_API::SameLine();
		MCP_API::VSliderInt("##Y-Axis Bounds (Bonus)", SKSEMenuFramework::ImVec2(80, 320), &stamPlotY, 200, 2500, "Y: %d");
	}

	if (CustomHeader("Parameters for Level-Based Bonus to Carry Weight")) {
		CustomSeparator("Configure Parameters");
		MCP_API::SliderFloat("Level Bonus Growth Rate", lvlRateTemp, -1.0f, 0.99f);
		MCP_API::SliderInt("Level Bonus Pivot", lvlPivotTemp, 1, 100);

		CustomSeparator("Preview");
		float (*func2)(void*, int) = Funcs::LvlBonus;
		MCP_API::PlotLines("", func2, NULL, lvlPlotX, 0, NULL, 0.0f, static_cast<float>(lvlPlotY), SKSEMenuFramework::ImVec2(0, 320));

		MCP_API::SameLine();
		MCP_API::VSliderInt("##X-Axis Bounds (Level)", SKSEMenuFramework::ImVec2(80, 320), &lvlPlotX, 50, 200, "X: %d");
		MCP_API::SameLine();
		MCP_API::VSliderInt("##Y-Axis Bounds (Bonus)", SKSEMenuFramework::ImVec2(80, 320), &lvlPlotY, 200, 2500, "Y: %d");
	}

	if (CustomHeader("Combined Result of all Carry Weight Settings")) {
		CustomSeparator("Preview");
		/* 
		if (ImPlot::BeginPlot("Test Plot")) {
			ImPlot::PlotBars("Barsss", plotData, 10);
			ImPlot::EndPlot();
		} 
		*/
		CustomSeparator("test");
	}

	//ImPlot::DestroyContext();
}


void __stdcall GUI::MCP::AdvDebug::Render()
{
	const char* logLevelNames[3] = {"Trace", "Debug", "Info"};
	MCP_API::SliderInt("Log Level", (int*)Settings::Get<uint32_t*>("uLogLevel"), 2, 0, logLevelNames[Settings::Get<uint32_t>("uLogLevel")], ImGuiMCP::ImGuiSliderFlags_NoInput);

	MCP_API::Text("Current Log Level = %s", logLevelNames[spdlog::get_level()]);
	MCP_API::SameLine();
	GUI::MCP::RefreshLogLevelButton();

	CustomCheckbox("Mod Enabled", Settings::Get<bool*>("bModEnabled"));
}

void __stdcall GUI::MCP::Example4::Render()
{
	MCP_API::Text("Testing Map String (Lang::Get()):");
	MCP_API::Text(Lang::Get("$MCP.Gen.Weight.General.Level.Simple"));

	MCP_API::Text("Testing Map String (_tr):");
	MCP_API::Text("$MCP.Adv.Weight.Stamina"_tr);

	CustomCheckbox("Test Checkbox", &Testing::testBool);
	MCP_API::Text("Test: %d", Testing::testBool);

	CustomSlider("Test Slider", &Testing::testFloat, 0.0f, 250.0f);
	MCP_API::Text("Test: %d", Testing::testFloat);

	MCP_API::Separator();
	MCP_API::Text("Testing Separator");

	MCP_API::SeparatorEx(NULL, 10);
	MCP_API::Text("Testing SeparatorEx");

	MCP_API::SeparatorText("SeparatorText");
	MCP_API::Text("Testing SeparatorText");

	if (CustomHeader("CollapsingHeader")) {
		MCP_API::Text("Testing CollapsingHeader");
	}

	GUI::MCP::CustomSeparator("CustomSeparator");
	MCP_API::Text("Testing CustomSeparator");

	if (GUI::MCP::CustomHeader("CustomHeader")) {
		MCP_API::Text("Testing CustomHeader!!!");
	}
}