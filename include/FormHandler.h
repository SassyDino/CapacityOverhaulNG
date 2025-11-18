#pragma once

class Forms final : public REX::Singleton<Forms>
{
	public:
		static const std::string_view pluginName;
		static RE::TESDataHandler* dataHandler;
		static std::optional<std::uint8_t> rawModIndex;
		static std::pair<uint32_t, uint32_t> modIndex;
		
		struct Global
		{
			static RE::TESGlobal* debuffSpeed;
		};

		struct Spell
		{
			static RE::SpellItem* weightDebuff;
		};

		struct Effect
		{
			static RE::Effect* debuffSpeed;
		};

		static void LoadFromGame();

		static bool IsCONGForm(uint32_t a_formID);
};