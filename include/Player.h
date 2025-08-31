#pragma once

class Player
{
	static float StamBaseAV;
	static float StamPermAV;
	static float StamAV;
	static int Level;

	public:
		static RE::PlayerCharacter* Char;
		static RE::ActorValueOwner* AsAV;

		static int ID;
		static RE::TESRace* Race;
		static float BaseStam;

		static float StamAtMaxGrad;
		static float LevelAtMaxGrad;

		static void UpdateLevel();
		static int GetLevel();
		static int UpdateAndGetLevel();

		static void UpdateStamina();		
		static float GetStamAV();
		static float UpdateAndGetStamAV();

		static void UpdateStamAtMaxGrad(float a_rate, uint32_t a_pivot);
		static void UpdateStamAtMaxGrad();
		static float UpdateAndGetStamAtMaxGrad(float a_rate, uint32_t a_pivot);


		static void UpdateLevelAtMaxGrad(float a_rate, uint32_t a_pivot);
		static void UpdateLevelAtMaxGrad();
};