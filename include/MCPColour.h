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

			// {1.0f, HEX_COL32(0xA1B2C3D4)}

			//NOTE: I've tried making these const (cause realistically they could be), but then I can't seem to assign them to another non-const variable later on. So un-consting for now, maybe come back to this
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

			static inline std::array<Scheme*, 5> availableSchemes = {&gClassic, &gRainbow, &gViridis, &gMagma, &gGreyscale};

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