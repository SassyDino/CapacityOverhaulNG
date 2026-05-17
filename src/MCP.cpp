#include "MCP.h"
#include "MCPStyle.h"
#include "MCPWidgets.h"
#include "MCPVisualiser.h"
#include "MCPHeatmap.h"
#include "MCPSettings.h"
#include "WeightHandler.h"
#include "Player.h"
#include "Calc.h"
#include <imgui.h>
#include <imgui_internal.h>

using namespace SKSEMenuFramework;

namespace MCP_API = ImGuiMCP::ImGui;
namespace MCPDraw = MCP_API::ImDrawListManager;

bool GUI::MCP::Testing::testBool = false;
int GUI::MCP::Testing::testInt = 0;
float GUI::MCP::Testing::testFloat = 0.0f;

void GUI::MCP::Register()
{
    if (!SKSEMenuFramework::IsInstalled()) {
		logger::warn("SKSE Menu Framework is not installed: cannot register Capacity Overhaul NG's Mod Control Panel");
        return;
    }

    SKSEMenuFramework::SetSection("$ModName"_tr);

	SKSEMenuFramework::AddSectionItem("$MCP.Gen"_tr + "/"s + "$MCP.Gen.ToggleFeatures"_tr, ToggleFeatures::Render);
	SKSEMenuFramework::AddSectionItem("$MCP.Gen"_tr + "/"s + "$MCP.Gen.CapacitySystem"_tr, CapacityConfigs::Render);
	SKSEMenuFramework::AddSectionItem("$MCP.Gen"_tr + "/"s + "$MCP.Gen.Weight"_tr, WeightConfigs::Render);
	SKSEMenuFramework::AddSectionItem("$MCP.Gen"_tr + "/"s + "$MCP.Gen.Debuffs"_tr, DebuffConfigs::Render);
	SKSEMenuFramework::AddSectionItem("$MCP.Gen"_tr + "/"s + "$MCP.Gen.UI"_tr, UIConfigs::Render);
	SKSEMenuFramework::AddSectionItem("$MCP.Adv"_tr + "/"s + "$MCP.Adv.Weight"_tr, AdvWeightConfigs::Render);
	SKSEMenuFramework::AddSectionItem("$MCP.Adv"_tr + "/"s + "$MCP.Adv.Debug"_tr, AdvDebug::Render);
	SKSEMenuFramework::AddSectionItem("$CONG.Testing"_tr, Example4::Render);
}

void __stdcall GUI::MCP::ToggleFeatures::Render()
{
	FileManagerButtons();

	if (CustomHeader("$MCP.Gen.ToggleFeatures.Capacity")) {
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Capacity.Debuffs", Settings::Get<bool*>("bCapacityBasedDebuffs"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Capacity.Hands", Settings::Get<bool*>("bNoHandsOverCap"), Settings::Get<bool>("bCapacityBasedDebuffs"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Capacity.Pickup", Settings::Get<bool*>("bPreventPickupOverCap"), Settings::Get<bool>("bCapacityBasedDebuffs"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Capacity.Containers", Settings::Get<bool*>("bNoContainerAccessOverCap"), Settings::Get<bool>("bCapacityBasedDebuffs"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Capacity.Interactions", Settings::Get<bool*>("bPreventInteractionsOverCap"), Settings::Get<bool>("bCapacityBasedDebuffs"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Capacity.Skills", Settings::Get<bool*>("bSkillsAffectCapacity"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Capacity.QuestItems", Settings::Get<bool*>("bQuestItemsAffectCapacity"));
	}

	if (CustomHeader("$MCP.Gen.ToggleFeatures.Weight")) {
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Weight.Debuffs", Settings::Get<bool*>("bWeightBasedDebuffs"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Weight.Vanilla", Settings::Get<bool*>("bVanillaWeightLimit"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Weight.Stamina", Settings::Get<bool*>("bStaminaAffectsWeight"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Weight.Level", Settings::Get<bool*>("bLevelAffectsWeight"));
		CustomCheckbox("$MCP.Gen.ToggleFeatures.Weight.Race", Settings::Get<bool*>("bRaceAffectsWeight"));
	}
}

void __stdcall GUI::MCP::CapacityConfigs::Render()
{
	if (CustomHeader("$MCP.Gen.CapacitySystem.Capacities")) {
		//TODO: Provide options to enable/disable misc categories
		//TODO: Also need to see if i can step the sliders, e.g. increment 5 or 10 per step
		CustomSeparator("$MCP.Gen.CapacitySystem.Capacities.Baseline");
		CustomCheckbox("$MCP.Gen.CapacitySystem.Capacities.Baseline.HugeShared", Settings::Get<bool*>("bHugeCapacityShared"));
		//TODO: Need to make availability of large items slide conditional based on above setting
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Baseline.Huge", Settings::Get<uint32_t*>("uHugeCapacity"), 1, 10, Settings::Get<bool>("bHugeCapacityShared"));
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Baseline.Large", Settings::Get<uint32_t*>("uLargeCapacity"), 1, 25, !Settings::Get<bool>("bHugeCapacityShared"));

		CustomSeparator("$MCP.Gen.CapacitySystem.Capacities.Derived");
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Capacities.Derived.LargePerHuge", Settings::Get<float*>("fLargePerHuge"), 1, 25, Settings::Get<bool>("bHugeCapacityShared"));
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Capacities.Derived.MediumPerLarge", Settings::Get<float*>("fMediumPerLarge"), 1, 25);
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Capacities.Derived.SmallPerMedium", Settings::Get<float*>("fSmallPerMedium"), 1, 25);
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Capacities.Derived.TinyPerSmall", Settings::Get<float*>("fTinyPerSmall"), 1, 25);

		CustomSeparator("$MCP.Gen.CapacitySystem.Capacities.Misc");
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Misc.Alchemy", Settings::Get<uint32_t*>("uAlchemyCapacity"), 1, 250);
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Misc.Ammo", Settings::Get<uint32_t*>("uAmmoCapacity"), 1, 500);
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Misc.Coin", Settings::Get<uint32_t*>("uCoinCapacity"), 1, 2500);
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Misc.CoinsPerTiny", Settings::Get<uint32_t*>("uCoinsPerTiny"), 1, 100);
		CustomCheckbox("$MCP.Gen.CapacitySystem.Capacities.Misc.Gems", Settings::Get<bool*>("bGemsInCoinCategory"));
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Misc.CoinsPerGem", Settings::Get<uint32_t*>("uCoinCapacityPerGem"), 1, 100, Settings::Get<bool>("bGemsInCoinCategory"));

		CustomSeparator("$MCP.Gen.CapacitySystem.Capacities.Weapons");
		CustomCheckbox("$MCP.Gen.CapacitySystem.Capacities.Weapons.Enable", Settings::Get<bool*>("bSeparateWeaponCategories"));
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Weapons.Large", Settings::Get<uint32_t*>("uLargeWeaponCapacity"), 0, 25, Settings::Get<bool>("bSeparateWeaponCategories"));
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Weapons.Medium", Settings::Get<uint32_t*>("uMediumWeaponCapacity"), 0, 25, Settings::Get<bool>("bSeparateWeaponCategories"));
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Weapons.Small", Settings::Get<uint32_t*>("uSmallWeaponCapacity"), 0, 25, Settings::Get<bool>("bSeparateWeaponCategories"));
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Weapons.Ranged", Settings::Get<uint32_t*>("uRangedWeaponCapacity"), 0, 25, Settings::Get<bool>("bSeparateWeaponCategories"));
		CustomSliderInt("$MCP.Gen.CapacitySystem.Capacities.Weapons.Shield", Settings::Get<uint32_t*>("uShieldCapacity"), 0, 25, Settings::Get<bool>("bSeparateWeaponCategories"));

		bool playerLoaded = PlayerState::IsGameWorldLoaded();

		//TODO: Add some sort of visualiser or aid for explaining how this all works without needing a lot of words
		if (CustomHeader("$MCP.Gen.CapacitySystem.Capacities.Visualiser")) {
			if (Settings::settingsLoaded && RE::PlayerCharacter::GetSingleton()) {
				CustomCheckbox("$MCP.Widgets.CapacityVisualiser.ShowBase", &Selections::visualiserBaseValues, playerLoaded);
				MCP_API::SameLine();
				CustomCheckbox("$MCP.Widgets.CapacityVisualiser.ShowFill", &Selections::visualiserShowFilled, playerLoaded);

				if (MCP_API::Button("$MCP.Gen.CapacitySystem.Capacities.Visualiser.NewWindow"_tr)) {
					Selections::visualiserSeparateWindow = true;
				}

				if (Selections::visualiserSeparateWindow) {
					MCP_API::Text("$MCP.Gen.CapacitySystem.Capacities.Visualiser.Opened"_tr);
					//TODO: Can probably remove this function param? Check later
					CapacityVisualiserWindow(&Selections::visualiserSeparateWindow, playerLoaded);
				} else {
					CapacityVisualiser(playerLoaded);
				}
			} else {
				MCP_API::Text("$MCP.Gen.CapacitySystem.Capacities.Visualiser.LoadCharacter"_tr);
			}
		}
	}

	if (CustomHeader("$MCP.Gen.CapacitySystem.Definitions")) {
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Definitions.Huge", Settings::Get<float*>("fHugeItemWeight"), 1, 50);
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Definitions.Large", Settings::Get<float*>("fLargeItemWeight"), 1, 25);
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Definitions.Medium", Settings::Get<float*>("fMediumItemWeight"), 1, 25);
		CustomSliderFloat("$MCP.Gen.CapacitySystem.Definitions.Small", Settings::Get<float*>("fSmallItemWeight"), 1, 10);
	}
}

void __stdcall GUI::MCP::WeightConfigs::Render()
{
	if (CustomHeader("$MCP.Gen.Weight.General")) {
		CustomSliderInt("$MCP.Gen.Weight.General.Base", Settings::Get<uint32_t*>("uBaseCarryWeight"), 10, 250);

		CustomSeparator("$MCP.Gen.Weight.General.Stamina");
		CustomSliderFloat("$MCP.Gen.Weight.General.Stamina.Modifier", Settings::Get<float*>("fStaminaWeightMod"), 0, 5);
		CustomCheckbox("$MCP.Gen.Weight.General.Stamina.Simple", Settings::Get<bool*>("bStaminaWeightSimple"));
		CustomSliderFloat("$MCP.Gen.Weight.General.Stamina.Bonus", Settings::Get<float*>("fWeightPerStamina"), 0, 5, Settings::Get<bool>("bStaminaWeightSimple"));
		CustomCheckbox("$MCP.Gen.Weight.General.Stamina.Temp", Settings::Get<bool*>("bTempStaminaAddsWeight"));

		CustomSeparator("$MCP.Gen.Weight.General.Level");
		CustomSliderFloat("$MCP.Gen.Weight.General.Level.Modifier", Settings::Get<float*>("fLevelWeightMod"), 0, 5);
		CustomCheckbox("$MCP.Gen.Weight.General.Level.Simple", Settings::Get<bool*>("bLevelWeightSimple"));
		CustomSliderFloat("$MCP.Gen.Weight.General.Level.Bonus", Settings::Get<float*>("fWeightPerStamina"), 0, 10, Settings::Get<bool>("bLevelWeightSimple"));
	}

	if (CustomHeader("$MCP.Gen.Weight.Race")) {
		CustomSeparator("$MCP.Gen.Weight.Race.Vanilla");
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Altmer", Settings::Get<float*>("fAltmerRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Argonian", Settings::Get<float*>("fArgonianRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Bosmer", Settings::Get<float*>("fBosmerRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Breton", Settings::Get<float*>("fBretonRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Dunmer", Settings::Get<float*>("fDunmerRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Imperial", Settings::Get<float*>("fImperialRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Khajiit", Settings::Get<float*>("fKhajiitRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Nord", Settings::Get<float*>("fNordRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Orc", Settings::Get<float*>("fOrcRaceMod"), 0.5, 2.5);
		CustomSliderFloat("$MCP.Gen.Weight.Race.Vanilla.Redguard", Settings::Get<float*>("fRedguardRaceMod"), 0.5, 2.5);
		CustomSeparator("$MCP.Gen.Weight.Race.Extra");
		CustomSliderFloat("$MCP.Gen.Weight.Race.Extra.Default", Settings::Get<float*>("fDefaultRaceMod"), 0.5, 2.5);
	}
}

void __stdcall GUI::MCP::DebuffConfigs::Render()
{
	CustomSeparator("$MCP.Gen.Debuffs.Activation");
	CustomCheckbox("$MCP.Gen.Debuffs.Activation.FloorIsPct", Settings::Get<bool*>("bWeightDebuffFloorIsPercentage"));
	CustomSliderInt("$MCP.Gen.Debuffs.Activation.FloorConst", Settings::Get<uint32_t*>("uWeightDebuffFloorConst"), 1, 100, !Settings::Get<bool>("bWeightDebuffFloorIsPercentage"));
	CustomSliderFloat("$MCP.Gen.Debuffs.Activation.FloorPercent", Settings::Get<float*>("fWeightDebuffFloorPct"), 0.1f, 1.0f, Settings::Get<bool>("bWeightDebuffFloorIsPercentage"));

	CustomSeparator("$MCP.Gen.Debuffs.Speed");
	CustomCheckbox("$MCP.Gen.Debuffs.Speed.Enable", Settings::Get<bool*>("bWeightAffectsSpeed"));
	CustomSliderFloat("$MCP.Gen.Debuffs.Speed.Max", Settings::Get<float*>("fSpeedDebuffMax"), 1, 100, Settings::Get<bool>("bWeightAffectsSpeed"));
	
	CustomSeparator("$MCP.Gen.Debuffs.Stealth");
	CustomCheckbox("$MCP.Gen.Debuffs.Stealth.Enable", Settings::Get<bool*>("bWeightAffectsStealth"));
	CustomSliderFloat("$MCP.Gen.Debuffs.Stealth.Max", Settings::Get<float*>("fStealthDebuffMax"), 1, 300, Settings::Get<bool>("bWeightAffectsStealth"));
	
	CustomSeparator("$MCP.Gen.Debuffs.StamDrain");
	CustomCheckbox("$MCP.Gen.Debuffs.StamDrain.Enable", Settings::Get<bool*>("bWeightAffectsStamDrain"));
	CustomSliderFloat("$MCP.Gen.Debuffs.StamDrain.Max", Settings::Get<float*>("fStamDrainDebuffMax"), 1, 300, Settings::Get<bool>("bWeightAffectsStamDrain"));
	
	CustomSeparator("$MCP.Gen.Debuffs.StamRegen");
	CustomCheckbox("$MCP.Gen.Debuffs.StamRegen.Enable", Settings::Get<bool*>("bWeightAffectsStamRegen"));
	CustomSliderFloat("$MCP.Gen.Debuffs.StamRegen.Max", Settings::Get<float*>("fStamRegenDebuffMax"), 1, 100, Settings::Get<bool>("bWeightAffectsStamRegen"));
	
	CustomSeparator("$MCP.Gen.Debuffs.AttackDmg");
	CustomCheckbox("$MCP.Gen.Debuffs.AttackDmg.Enable", Settings::Get<bool*>("bWeightAffectsAttackDmg"));
	CustomSliderFloat("$MCP.Gen.Debuffs.AttackDmg.Max", Settings::Get<float*>("fAttackDmgDebuffMax"), 1, 100, Settings::Get<bool>("bWeightAffectsAttackDmg"));
}

void __stdcall GUI::MCP::UIConfigs::Render()
{
	CustomSeparator("$MCP.Gen.UI.Appearance");
	CustomCheckbox("$MCP.Gen.UI.Appearance.CustomStyling", Settings::Get<bool*>("bCustomMenuStyling"));

	CustomSeparator("$MCP.Gen.UI.Language");
	CustomCheckbox("$MCP.Gen.UI.Language.Manual", Settings::Get<bool*>("bOverrideLanguage"));

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

	static int stamMaxX = 250;
	static int lvlMaxX = 100;

	static int stamPlotX = 250;
	static int stamPlotY = 500;
	static int lvlPlotX = 100;
	static int lvlPlotY = 500;

	struct Funcs
	{
		static float StamBonus(void*, int i) {
			return Calc::StaminaWeightBonus(static_cast<float>(i), *stamRateTemp, *stamPivotTemp, *baseCarryTemp, PlayerState::CalcStamAtMaxGrad(*stamRateTemp, *stamPivotTemp, *baseCarryTemp));
		}
		static float LvlBonus(void*, int i) {
			return Calc::LevelWeightBonus(static_cast<float>(i), *lvlRateTemp, *lvlPivotTemp, *baseCarryTemp, PlayerState::CalcLevelAtMaxGrad(*lvlRateTemp, *lvlPivotTemp, *baseCarryTemp));
		}
	};

	if (CustomHeader("$MCP.Adv.Weight.Stamina")) {
		CustomSeparator("$MCP.Adv.Weight.Stamina.Config");
		CustomSliderFloat("$MCP.Adv.Weight.Stamina.Config.Rate", stamRateTemp, -1.0f, 0.99f);
		CustomSliderInt("$MCP.Adv.Weight.Stamina.Config.Pivot", (uint32_t*)stamPivotTemp, 1, 250);

		CustomSeparator("$MCP.Adv.Weight.Stamina.Preview");
		float (*func1)(void*, int) = Funcs::StamBonus;
		MCP_API::PlotLines("", func1, NULL, stamPlotX, 0, NULL, 0.0f, static_cast<float>(stamPlotY), SKSEMenuFramework::ImVec2(0, 320));
		
		MCP_API::SameLine();
		MCP_API::VSliderInt("##X-Axis Bounds (Stamina)", SKSEMenuFramework::ImVec2(80, 320), &stamPlotX, 150, 500, "X: %d");
		MCP_API::SameLine();
		MCP_API::VSliderInt("##Y-Axis Bounds (Bonus)", SKSEMenuFramework::ImVec2(80, 320), &stamPlotY, 200, 2500, "Y: %d");
	}

	if (CustomHeader("$MCP.Adv.Weight.Level")) {
		CustomSeparator("$MCP.Adv.Weight.Level.Config");
		MCP_API::SliderFloat("$MCP.Adv.Weight.Level.Config.Rate", lvlRateTemp, -1.0f, 0.99f);
		MCP_API::SliderInt("$MCP.Adv.Weight.Level.Config.Pivot", lvlPivotTemp, 1, 100);

		CustomSeparator("$MCP.Adv.Weight.Level.Preview");
		float (*func2)(void*, int) = Funcs::LvlBonus;
		MCP_API::PlotLines("", func2, NULL, lvlPlotX, 0, NULL, 0.0f, static_cast<float>(lvlPlotY), SKSEMenuFramework::ImVec2(0, 320));

		MCP_API::SameLine();
		MCP_API::VSliderInt("##X-Axis Bounds (Level)", SKSEMenuFramework::ImVec2(80, 320), &lvlPlotX, 50, 200, "X: %d");
		MCP_API::SameLine();
		MCP_API::VSliderInt("##Y-Axis Bounds (Bonus)", SKSEMenuFramework::ImVec2(80, 320), &lvlPlotY, 200, 2500, "Y: %d");
	}

	if (CustomHeader("$MCP.Adv.Weight.Combined")) {
		CustomSeparator("$MCP.Adv.Weight.Combined.Preview");
	}
}

void __stdcall GUI::MCP::AdvDebug::Render()
{
	const char* logLevelNames[3] = {"Trace", "Debug", "Info"};
	MCP_API::SliderInt("$MCP.Adv.Debug.Level"_tr, (int*)Settings::Get<uint32_t*>("uLogLevel"), 2, 0, logLevelNames[Settings::Get<uint32_t>("uLogLevel")], ImGuiMCP::ImGuiSliderFlags_NoInput);

	MCP_API::Text("%s = %s", "$MCP.Adv.Debug.CurrentLevel"_tr, logLevelNames[spdlog::get_level()]);
	MCP_API::SameLine();
	GUI::MCP::RefreshLogLevelButton();

	CustomCheckbox("$MCP.Adv.Debug.ModEnabled", Settings::Get<bool*>("bModEnabled"));
}

void __stdcall GUI::MCP::Example4::Render()
{
	GUI::MCP::CustomSeparator("$CONG.Testing");

	CustomCheckbox("$CONG.Testing.1", &Testing::testBool);
	MCP_API::Text("Test: %d", Testing::testBool);

	CustomSlider("$CONG.Testing.1", &Testing::testFloat, 0.0f, 250.0f);
	MCP_API::Text("Test: %d", Testing::testFloat);

	if (CustomHeader("$CONG.Testing.1")) {
		HeatmapWidget(900, 450);
	}
}