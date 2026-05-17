#pragma once

namespace GUI
{
	struct Colour
	{
		//NOTE: RGB/CMY definitions only here for ease of use during development & testing, can probably remove later.
		static const ImU32 RED = HEX_COL32(0xFF0000FF);
		static const ImU32 YELLOW = HEX_COL32(0xFFFF00FF);
		static const ImU32 GREEN = HEX_COL32(0x00FF00FF);
		static const ImU32 CYAN = HEX_COL32(0x00FFFFFF);
		static const ImU32 BLUE = HEX_COL32(0x0000FFFF);
		static const ImU32 MAGENTA = HEX_COL32(0xFF00FFFF);

		static const ImU32 separatorText = HEX_COL32(0xDDDDDDFF);
		static const ImU32 separatorLine = HEX_COL32(0x999999FF);
		static const ImU32 headerBG = HEX_COL32(0x00000099);

		struct Gradient
		{
			struct Stop { float t; ImU32 col; };

			struct Scheme
			{
				const std::vector<Stop> scheme;

				Scheme(std::string a_nameKey, std::vector<Stop> a_scheme): nameKey(a_nameKey), scheme(a_scheme) {};

				const char* GetName() const;

				private:
					const std::string nameKey;
			};

			static inline Scheme gClassic = Scheme("$MCP.Widgets.Colour.Gradient.Classic", 
				{
					{0.0f, HEX_COL32(0xFF0000FF)},
					{1.0f, HEX_COL32(0x0000FFFF)}
				}
			);

			static inline Scheme gRainbow = Scheme("$MCP.Widgets.Colour.Gradient.Rainbow", 
				{
					{0.0f, HEX_COL32(0xCC4CCCFF)},
					{0.25f, HEX_COL32(0x4C4CFFFF)},
					{0.5f, HEX_COL32(0x4CCC4CFF)},
					{0.75f, HEX_COL32(0xFFF24CFF)},
					{1.0f, HEX_COL32(0xFF4C4CFF)}
				}
			);
			
			static inline Scheme gViridis = Scheme("$MCP.Widgets.Colour.Gradient.Viridis", 
				{
					{0.0f, HEX_COL32(0xFDE725FF)},
					{0.25f, HEX_COL32(0x5EC962FF)},
					{0.5f, HEX_COL32(0x21918CFF)},
					{0.75f, HEX_COL32(0x3B528BFF)},
					{1.0f, HEX_COL32(0x440154FF)}
				}
			);

			static inline Scheme gMagma = Scheme("$MCP.Widgets.Colour.Gradient.Magma", 
				{
					{0.0f, HEX_COL32(0xFCFDBFFF)},
					{0.25f, HEX_COL32(0xFC8961FF)},
					{0.5f, HEX_COL32(0xB73779FF)},
					{0.75f, HEX_COL32(0x51127CFF)},
					{1.0f, HEX_COL32(0x000004FF)}
				}
			);

			static inline Scheme gGreyscale = Scheme("$MCP.Widgets.Colour.Gradient.Greyscale", 
				{
					{0.0f, HEX_COL32(0xFFFFFFFF)},
					{1.0f, HEX_COL32(0x000000FF)}
				}
			);

			static inline Scheme gJet = Scheme("$MCP.Widgets.Colour.Gradient.Jet", 
				{
					{0.0f, HEX_COL32(0x00007FFF)},
					{0.125f, HEX_COL32(0x0000FFFF)},
					{0.25f, HEX_COL32(0x007FFFFF)},
					{0.375f, HEX_COL32(0x00FFFFFF)},
					{0.5f, HEX_COL32(0x7FFF7FFF)},
					{0.625f, HEX_COL32(0xFFFF00FF)},
					{0.750f, HEX_COL32(0xFF7F00FF)},
					{0.875f, HEX_COL32(0xFF0000FF)},
					{1.0f, HEX_COL32(0x7F0000FF)}
				}
			);

			static inline Scheme gPastel = Scheme("$MCP.Widgets.Colour.Gradient.Pastel", 
				{
					{0.0f, HEX_COL32(0xC278F0FF)},
					{0.091f, HEX_COL32(0xCE95F3FF)},
					{0.182f, HEX_COL32(0xDDA5C5FF)},
					{0.273f, HEX_COL32(0xE8B2A5FF)},
					{0.364f, HEX_COL32(0xEFC399FF)},
					{0.455f, HEX_COL32(0xF3D796FF)},
					{0.546f, HEX_COL32(0xF5E997FF)},
					{0.637f, HEX_COL32(0xF2F3A2FF)},
					{0.728f, HEX_COL32(0xE3F1B9FF)},
					{0.819f, HEX_COL32(0xC5E2DAFF)},
					{0.91f, HEX_COL32(0x98CCF4FF)},
					{1.0f, HEX_COL32(0x6EB5ECFF)}
				}
			);

			static inline std::array<Scheme*, 7> availableSchemes = {&gClassic, &gRainbow, &gViridis, &gMagma, &gGreyscale, &gJet, &gPastel};

			static void HexToRGBA(ImU32 a_col, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a);

			static ImU32 GetLUTVal(float a_val);

			static void RequestNewLUT(Scheme* a_gradient);

			private:
				static std::vector<ImU32> LUT;
				static bool newLUTRequested;
				static Scheme* newLUT;

				static ImU32 Interpolate(ImU32 col1, ImU32 col2, float t);
				static ImU32 Sample(float a_val, Scheme* a_gradient);

				static void GenerateLUT(Scheme* a_gradient);
		};
	};
}