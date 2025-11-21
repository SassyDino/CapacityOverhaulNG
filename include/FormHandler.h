#pragma once

class Forms final : public REX::Singleton<Forms>
{
	public:
		static const std::string_view pluginName;
		static RE::TESDataHandler* dataHandler;
		static std::optional<std::uint8_t> rawModIndex;
		static std::pair<uint32_t, uint32_t> modIndex;

		struct Spell
		{
			static RE::SpellItem* weightDebuff;
		};
		
		struct Global
		{
			static RE::TESGlobal* debuffSpeed;
			static RE::TESGlobal* debuffStealth;
			static RE::TESGlobal* debuffStamDrain;
			static RE::TESGlobal* debuffStamRegen;
			static RE::TESGlobal* debuffWeapSpeed;
			static RE::TESGlobal* debuffAttackDmg;
			
			static RE::TESGlobal* debuffStealthDisplay;
			static RE::TESGlobal* debuffStamDrainDisplay;
			static RE::TESGlobal* debuffWeapSpeedDisplay;
			static RE::TESGlobal* debuffAttackDmgDisplay;
		};

		struct Effect
		{
			static RE::Effect* debuffSpeed;
			static RE::Effect* debuffStealth;
			static RE::Effect* debuffStamDrain;
			static RE::Effect* debuffStamRegen;
			static RE::Effect* debuffWeapSpeed;
			static RE::Effect* debuffAttackDmg;
		};

		
		struct KYWD
		{
			static RE::FormID CONG_CoinItem;

			static RE::FormID VendorItemPotion;
			static RE::FormID VendorItemPoison;
			static RE::FormID VendorItemGem;
			static RE::FormID ArmorShield;
		};
		
		struct MISC
		{
			static RE::FormID BYOHMaterialLog;
		};

		static void LoadFromGame();

		static bool IsCONGForm(uint32_t a_formID);
};