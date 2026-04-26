#pragma once
#include "SKSEMCP/SKSEMenuFramework.hpp"
#include "CapacityHandler.h"

using ImU32 = ImGuiMCP::ImU32;

namespace GUI
{
	struct Assets
	{
		static const std::string dir;
		static const std::string type;

		static ImGuiMCP::ImTextureID CheckboxEmpty;
		static ImGuiMCP::ImTextureID CheckboxFilled;

		static ImGuiMCP::ImTextureID SeparatorLeft;
		static ImGuiMCP::ImTextureID SeparatorRight;

		static ImGuiMCP::ImTextureID HeaderBackLeft;
		static ImGuiMCP::ImTextureID HeaderBackRight;
		static ImGuiMCP::ImTextureID HeaderFrontLeft;
		static ImGuiMCP::ImTextureID HeaderFrontRight;
		static ImGuiMCP::ImTextureID HeaderArrowDown;
		static ImGuiMCP::ImTextureID HeaderArrowUpLeft;
		static ImGuiMCP::ImTextureID HeaderArrowUpRight;

		static ImGuiMCP::ImTextureID ScrollbarLeft;
		static ImGuiMCP::ImTextureID ScrollbarRight;
		static ImGuiMCP::ImTextureID ScrollbarSelector;

		static float defaultFontSize;

		static std::string GetTexPath(std::string a_filename);
		static void LoadTextures();
	};

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

	namespace MCP
	{
		const extern REX::W32::ID3D11Device* BSRendererDevice;

		const extern float borderThick;
		const extern float borderThin;
		const extern std::unordered_map<CapacityHandler::CategoryID, const char *> categoryTooltips;

		struct Selections
		{
			static inline Colour::Gradient::Scheme* heatmapGradient = &Colour::Gradient::gClassic;
			static inline bool heatmapConstrainGradient = true;
			static inline float heatmapConstraintVal = 1500;
		};

		const enum hAlign {NoHAlign, LeftAlign, CentreHAlign, RightAlign};
		const enum vAlign {NoVAlign, TopAlign, CentreVAlign, BottomAlign};

		void PushCustomText(float a_scaleMult, ImU32 a_colour);
		void PopCustomText();
		SKSEMenuFramework::ImVec4 AlignedText(const char *text, SKSEMenuFramework::ImVec2* a_p0, SKSEMenuFramework::ImVec2* a_p1, hAlign kh, vAlign kv);
		void CenteredText(const char *text, SKSEMenuFramework::ImVec2 a_p0);
		void VerticalText(SKSEMenuFramework::ImDrawList* draw_list, const char *text, SKSEMenuFramework::ImVec2* a_p0);

		bool CustomHeader(const char *text);
		SKSEMenuFramework::ImVec4 DrawHeaderBar(ImGuiMCP::ImDrawList* drawList, SKSEMenuFramework::ImVec2 p0, float textSize, float textVertPadding);
		void DrawHeaderArrows(ImGuiMCP::ImDrawList* drawList, bool is_Open, SKSEMenuFramework::ImVec2 hp0, SKSEMenuFramework::ImVec2 hp1, SKSEMenuFramework::ImVec2 tp0, SKSEMenuFramework::ImVec2 tp1, float textVertPadding);

		void CustomSeparator(const char *text);

		void CustomCheckbox(const char *text, bool *a_toggle);

		void CustomSlider(const char *text, float* a_setting, float a_minValue, float a_maxValue);
		void CustomIntSlider(const char *text);
		void CustomFloatSlider(const char *text);

		SKSEMenuFramework::ImVec4 DrawSliderTrack(ImGuiMCP::ImDrawList* drawList, float scale, SKSEMenuFramework::ImVec2 p0);
		std::pair<bool, float> DrawSliderSelector(ImGuiMCP::ImDrawList* drawList, float scale, SKSEMenuFramework::ImVec4 bounds);
		float ValueToSliderXPos(float a_settingValue, float x0, float x1, float a_minValue, float a_maxValue);
		float SliderXPosToValue(float x_pos, float x0, float x1, float a_minValue, float a_maxValue);

		void SnapFloatSlider(float *a_target, float snap);

		void FileManagerButtons();

		void LanguageSelector();

		void RefreshLogLevelButton();
		
		SKSEMenuFramework::ImU32 PercentageColour(float a_count, float a_capacity);
		void CapacityCategoryTooltip(SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1, const char* a_title, CapacityHandler::ItemCat* a_category);
		void DrawHatchFill(SKSEMenuFramework::ImDrawList *drawList, SKSEMenuFramework::ImVec2 p0, SKSEMenuFramework::ImVec2 p1);

		void CapacityVisualiser();
		void CapacityVisualiserWindow(bool *open_state);

		void CapacityVisualiserMain();
		void CapacityVisualiserTotal();
		void CapacityVisualiserMisc(float y_max);
		float CapacityVisualiserWeapons();

		// Disclaimer: Pretty much all of the DX11 and rendering stuff in this 'Heatmap' class is vibe coded. Or at least, AI did all of the heavy lifting and then I just made the code work with my mod. Not a fan of doing it that way, but the rendering stuff mostly just flew straight over my head, so I took the easy way out :(
		class Heatmap
		{
			ID3D11Device* m_device = nullptr;
			ID3D11DeviceContext* m_ctx = nullptr;
			ID3D11Texture2D* m_texture = nullptr;
			ID3D11ShaderResourceView* m_srv = nullptr;

			std::vector<uint8_t> m_pixels;

			int m_width;
			int m_height;

			public:
				Heatmap() = default;
				~Heatmap() = default;

				int m_padWidth;
				int m_padHeight;

				void Init();

				void Update(const std::vector<float>& data);

				ImGuiMCP::ImTextureID GetTextureID() const;

				void Release();

				void CreateTexture();
				void GeneratePixels(const std::vector<float>& data);
				void Upload();
		};

		extern Heatmap heatmap;

		struct HeatmapCursorData
		{
			HeatmapCursorData() = default;
			HeatmapCursorData(int x, int y, float val): stamina(x), level(y), weight(val) {};

			int stamina;
			int level;
			float weight;
		};

		void HeatmapWidget(int plot_width, int plot_height);
		void HeatmapUpdateButton();
		void HeatmapGradientSelector();
		void HeatmapPlot(int a_width, int a_height);
		HeatmapCursorData GetHeatmapCursorVal(SKSEMenuFramework::ImVec2 a_pCursor, SKSEMenuFramework::ImVec2 a_p0, SKSEMenuFramework::ImVec2 a_p1);

		void CursorDebugMarker(unsigned int a_colour, SKSEMenuFramework::ImVec2 a_p0 = SKSEMenuFramework::ImVec2(0.0f,0.0f), float a_radius = 5.0f);
	}
}