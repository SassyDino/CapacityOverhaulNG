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
	};
/* 
	static std::string_view pluginName;
	static RE::TESDataHandler* dataHandler;

	static RE::TESGlobal* speedGlobal;

	static RE::SpellItem* weightSpell;
	static RE::Effect* speedEffect;
 */
	static void CheckWeight();
	static void AddDebuffSpellToPlayer();
	static void EnableWeightEffects();
	static void ModifyWeightEffects(float a_debuffAmount);
	static void DisableWeightEffects();

	static void EnableSpellMGEF(RE::TESGlobal* a_global);
	static void DisableSpellMGEF(RE::TESGlobal* a_global);

	static void ModifyMoveSpeedMGEF(float a_debuffAmount);
	static void ReduceCombatSkill(float a_debuffAmount);
	static void ReduceStealth(float a_debuffAmount);
	static void IncreaseStamDrain(float a_debuffAmount);
	static void ReduceStamRegen(float a_debuffAmount);

	static void CapacityEffects();
};