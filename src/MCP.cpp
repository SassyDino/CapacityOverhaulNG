#include "MCP.h"
#include "WeightHandler.h"
#include "Player.h"
#include "Calc.h"
#include "implot.h"
#include "implot_internal.h"
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
	MCP_API::Checkbox("Disable Hand Interactions While Over Capacity", Settings::Get<bool*>("bNoHandsOverCap"));
	MCP_API::Checkbox("Prevent Item Pickup While Over Capacity", Settings::Get<bool*>("bPreventPickupOverCap"));
	MCP_API::Checkbox("Skills Affect Capacity Limits", Settings::Get<bool*>("bSkillsAffectCapacity"));
	MCP_API::Checkbox("Quest Item Take Up Capacity", Settings::Get<bool*>("bQuestItemsAffectCapacity"));


	MCP_API::Text("");
	MCP_API::Text("Weight Features");
	MCP_API::Checkbox("Use Vanilla Carry Weight", Settings::Get<bool*>("bVanillaWeightLimit"));
	MCP_API::Checkbox("Max Stamina Affects Carry Weight", Settings::Get<bool*>("bStaminaAffectsWeight"));
	MCP_API::Checkbox("Current Level Affects Carry Weight", Settings::Get<bool*>("bLevelAffectsWeight"));
	MCP_API::Checkbox("Player's Race Affects Carry Weight", Settings::Get<bool*>("bRaceAffectsWeight"));
}

void __stdcall GUI::MCP::CapacityConfigs::Render()
{
	//TODO: Provide options to enable/disable misc categories
	//TODO: Also need to see if i can step the sliders, e.g. increment 5 or 10 per step
	MCP_API::SeparatorText("Category Capacities");
	MCP_API::SliderInt("Huge Items", (int*)(Settings::Get<uint32_t*>("uHugeCapacity")), 1, 10);
	MCP_API::Checkbox("Huge Items Share Storage Space With Other Items", Settings::Get<bool*>("bHugeCapacityShared"));
	//TODO: Need to make availability of large items slide conditional based on above setting
	MCP_API::SliderInt("Large Items", (int*)(Settings::Get<uint32_t*>("uLargeCapacity")), 1, 25);

	MCP_API::SliderFloat("Large Items Per Huge Space", Settings::Get<float*>("fLargePerHuge"), 1, 25);
	MCP_API::SliderFloat("Medium Items Per Large Space", Settings::Get<float*>("fMediumPerLarge"), 1, 25);
	MCP_API::SliderFloat("Small Items Per Medium Space ", Settings::Get<float*>("fSmallPerMedium"), 1, 25);
	MCP_API::SliderFloat("Tiny Items Per Small Space", Settings::Get<float*>("fTinyPerSmall"), 1, 25);

	MCP_API::SliderInt("Alchemical Item Capacity", (int*)(Settings::Get<uint32_t*>("uAlchemyCapacity")), 1, 250);
	MCP_API::SliderInt("Ammo Capacity", (int*)(Settings::Get<uint32_t*>("uAmmoCapacity")), 1, 500);
	MCP_API::SliderInt("Coin Capacity", (int*)(Settings::Get<uint32_t*>("uCoinCapacity")), 1, 2500);
	MCP_API::SliderInt("Coins Per Tiny Space", (int*)(Settings::Get<uint32_t*>("uCoinsPerTiny")), 1, 100);

	MCP_API::Text("");
	MCP_API::SeparatorText("Item Categories");
	MCP_API::SliderFloat("Huge Item Weight", Settings::Get<float*>("fHugeItemWeight"), 1, 50);
	MCP_API::SliderFloat("Large Item Weight", Settings::Get<float*>("fLargeItemWeight"), 1, 25);
	MCP_API::SliderFloat("Medium Item Weight", Settings::Get<float*>("fMediumItemWeight"), 1, 25);
	MCP_API::SliderFloat("Small Item Weight", Settings::Get<float*>("fSmallItemWeight"), 1, 10);
}

void __stdcall GUI::MCP::WeightConfigs::Render()
{
	MCP_API::SeparatorText("General Weight Settings");
	MCP_API::SliderInt("Base Carry Weight", (int*)(Settings::Get<uint32_t*>("uBaseCarryWeight")), 10, 250);

	MCP_API::SliderFloat("Stamina Weight Bonus Modifier", Settings::Get<float*>("fStaminaWeightMod"), 0, 5);
	MCP_API::Checkbox("Use Simple Calculation For Stamina Weight Bonus", Settings::Get<bool*>("bStaminaWeightSimple"));
	MCP_API::SliderFloat("Bonus Carry Weight Per Stamina Point", Settings::Get<float*>("fWeightPerStamina"), 0, 5);

	MCP_API::SliderFloat("Level Weight Bonus Modifier", Settings::Get<float*>("fLevelWeightMod"), 0, 5);
	MCP_API::Checkbox("Use Simple Calculation For Level Weight Bonus ", Settings::Get<bool*>("bLevelWeightSimple"));
	MCP_API::SliderFloat("Bonus Carry Weight Per Level", Settings::Get<float*>("fWeightPerStamina"), 0, 10);

	MCP_API::Text("");
	MCP_API::SeparatorText("Race-Based Carry Weight Modifiers");
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
	MCP_API::SliderFloat("Default Race", Settings::Get<float*>("fDefaultRaceMod"), 0.5, 2.5);
}

void __stdcall GUI::MCP::AdvWeightConfigs::Render()
{
	static float stamRateTemp = *Settings::Get<float*>("fStaminaWeightRate");
	static int stamPivotTemp = *Settings::Get<uint32_t*>("uStaminaWeightPivot");
	static float lvlRateTemp = *Settings::Get<float*>("fLevelWeightRate");
	static int lvlPivotTemp = *Settings::Get<uint32_t*>("uLevelWeightPivot");
	static uint32_t baseCarryTemp = *Settings::Get<uint32_t*>("uBaseCarryWeight");

	static float maxGradStam = Player::CalcStamAtMaxGrad(stamRateTemp, stamPivotTemp, baseCarryTemp);
	static float maxGradLvl = Player::CalcLevelAtMaxGrad(lvlRateTemp, lvlPivotTemp, baseCarryTemp);
	static int stamMaxX = 500;
	static int lvlMaxX = 250;

	struct Funcs
	{
		static float StamBonus(void*, int i) {
			return Calc::StaminaWeightBonus(i, stamRateTemp, stamPivotTemp, baseCarryTemp, maxGradStam);
		}
		static float LvlBonus(void*, int i) {
			return Calc::LevelWeightBonus(i, lvlRateTemp, lvlPivotTemp, baseCarryTemp, maxGradLvl);
		}
	};

	MCP_API::SeparatorText("Configure Advanced Carry Weight Calculation Parameters");

	MCP_API::SliderFloat("Stamina Bonus Growth Rate", &stamRateTemp, -1.0f, 0.99f);
	MCP_API::SliderInt("Stamina Bonus Pivot", &stamPivotTemp, 10, 500);
	float (*func1)(void*, int) = Funcs::StamBonus;
	MCP_API::PlotLines("Stamina Bonus", func1, NULL, stamMaxX, 0, NULL, 0.0f, 1000.0f, SKSEMenuFramework::ImVec2(0, 320));

	MCP_API::SliderFloat("Level Bonus Growth Rate", &lvlRateTemp, -1.0f, 0.99f);
	MCP_API::SliderInt("Level Bonus Pivot", &lvlPivotTemp, 10, 250);
	float (*func2)(void*, int) = Funcs::LvlBonus;
	MCP_API::PlotLines("Level Bonus", func2, NULL, lvlMaxX, 0, NULL, 0.0f, 1000.0f, SKSEMenuFramework::ImVec2(0, 320));
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