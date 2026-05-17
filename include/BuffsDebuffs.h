#pragma once

struct Buffs
{

};

struct Debuffs
{
	struct Current
	{
		static bool weightDebuffsActive;
		static bool overDebuffFloor;
		static float speedMod;

		static bool capacityDebuffsActive;
		static bool enableHands;
		static bool handsEnabled;
		static bool enablePickup;
		static bool pickupEnabled;
		static bool enableContainers;
		static bool containersEnabled;
		static bool enableInteract;
		static bool interactEnabled;
	};

	static void CheckWeight();
	static void AddDebuffSpellToPlayer();
	static void RefreshDebuffSpell();

	static void EnableWeightEffects();
	static void DisableWeightEffects();
	static void ModifyWeightEffects(float a_debuffScale);

	static void EnableSpellMGEF(RE::TESGlobal* a_global);
	static void DisableSpellMGEF(RE::TESGlobal* a_global);

	static void ModifyMoveSpeedMGEF(float a_debuffScale);
	static void ModifyStealthMGEF(float a_debuffScale);
	static void ModifyStamRegenMGEF(float a_debuffScale);
	static void ModifyStamDrainMGEF(float a_debuffScale);
	static void ModifyWeapSpeedMGEF(float a_debuffScale);
	static void ModifyAttackDmgMGEF(float a_debuffScale);

	static void CapacityEffects();
	static void QueueEnableCapacityDebuffs();
	static void QueueDisableCapacityDebuffs();
	static void ConfirmCapacityDebuffStatus();

	static void EnableHands();
	static void DisableHands();
	static void EnablePickup();
	static void DisablePickup();
	static void EnableContainers();
	static void DisableContainers();
	static void EnableInteractions();
	static void DisableInteractions();

	static bool CanDrawWeapon();
	static bool CanPickup();
	static bool CanOpenContainers();
	static bool CanInterect();
};