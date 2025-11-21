#pragma once

struct Buffs
{

};

struct Debuffs
{
	struct Current
	{
		static bool overDebuffFloor;
		static float speedMod;

		static bool enableHands;
		static bool handsEnabled;
		static bool enablePickup;
		static bool pickupEnabled;
	};

	static void CheckWeight();
	static void AddDebuffSpellToPlayer();
	static void RefreshDebuffSpell();

	static void EnableWeightEffects();
	static void DisableWeightEffects();
	static void ModifyWeightEffects(float a_debuffAmount);

	static void EnableSpellMGEF(RE::TESGlobal* a_global);
	static void DisableSpellMGEF(RE::TESGlobal* a_global);

	static void ModifyMoveSpeedMGEF(float a_debuffAmount);
	static void ModifyStealthMGEF(float a_debuffAmount);
	static void ModifyStamRegenMGEF(float a_debuffAmount);
	static void ModifyStamDrainMGEF(float a_debuffAmount);
	static void ModifyWeapSpeedMGEF(float a_debuffAmount);
	static void ModifyAttackDmgMGEF(float a_debuffAmount);

	static void CapacityEffects();
	static void EnableHands();
	static void DisableHands();
	static void EnablePickup();
	static void DisablePickup();
	static void CheckCapacityDebuffStatus();
};