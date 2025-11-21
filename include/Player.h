#pragma once

class PlayerStatus
{
	static float StamBaseAV;
	static float StamPermAV;
	static float StamAV;

	static int Level;

	static float CurrentWeight;

	static bool isOverCapacity;

	public:
		static RE::PlayerCharacter* Char;
		static RE::ActorValueOwner* AsAV;
		static RE::PlayerControls* Controls;
		static RE::ActorState* State;

		static int ID;
		static RE::TESRace* Race;
		static float raceWeightMod;
		static float BaseStam;

		static float StamAtMaxGrad;
		static float LevelAtMaxGrad;

		static void UpdateCapacityStatus(bool overCapacityStatus);
		static bool GetOverCapacityStatus();

		static void UpdateBurden();
		static int GetBurden();
		static int UpdateAndGetBurden();

		static void UpdateLevel();
		static int GetLevel();
		static int UpdateAndGetLevel();

		static void UpdateStamina();		
		static float GetStamAV();
		static float UpdateAndGetStamAV();

		static float CalcStamAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry);
		static void UpdateStamAtMaxGrad();

		static float CalcLevelAtMaxGrad(float a_rate, uint32_t a_pivot, uint32_t baseCarry);
		static void UpdateLevelAtMaxGrad();
};