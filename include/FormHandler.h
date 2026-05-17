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
			const static RE::FormID UnspecifiedDamageFFAimed = 0xEA075;
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

			const static RE::FormID VendorItemPotion = 0x8CDEC;
			const static RE::FormID VendorItemPoison = 0x8CDED;
			const static RE::FormID VendorItemGem = 0x914ED;
			const static RE::FormID ArmorShield = 0x965B2;
		};
		
		struct MISC
		{
			const static RE::FormID BYOHMaterialLog = 0x300300E;
		};

		static void LoadFromGame();

		static bool IsCONGForm(uint32_t a_formID);
};