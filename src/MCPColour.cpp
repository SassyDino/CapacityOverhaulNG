#include "MCPColour.h"

namespace GUI
{
	std::vector<ImU32> Colour::Gradient::LUT;
	bool Colour::Gradient::newLUTRequested = true;
	Colour::Gradient::Scheme* Colour::Gradient::newLUT = &Colour::Gradient::gClassic;

	const char* Colour::Gradient::Scheme::GetName() const
	{
		return Lang::Get(nameKey.c_str());
	}

	void Colour::Gradient::HexToRGBA(ImU32 a_col, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a)
	{
		r = (a_col >> 0) & 0xFF;
		g = (a_col >> 8) & 0xFF;
		b = (a_col >> 16) & 0xFF;
		a = (a_col >> 24) & 0xFF;
	}

	ImU32 Colour::Gradient::Interpolate(ImU32 col1, ImU32 col2, float t)
	{
		uint8_t r1, g1, b1, a1;
		uint8_t r2, g2, b2, a2;

		HexToRGBA(col1, r1, g1, b1, a1);
		HexToRGBA(col2, r2, g2, b2, a2);

		uint8_t r = static_cast<uint8_t>(r1 + t * (r2 - r1));
		uint8_t g = static_cast<uint8_t>(g1 + t * (g2 - g1));
		uint8_t b = static_cast<uint8_t>(b1 + t * (b2 - b1));
		uint8_t a = static_cast<uint8_t>(a1 + t * (a2 - a1));

		return (r << 0) | (g << 8) | (b << 16) | (a << 24);
	}

	ImU32 Colour::Gradient::Sample(float a_val, Scheme* a_gradient)
	{
		a_val = std::clamp(a_val, 0.0f, 1.0f);

		for (size_t i = 0; i < a_gradient->scheme.size() - 1; i++) {
			if (a_val >= a_gradient->scheme[i].t && a_val <= a_gradient->scheme[i + 1].t) {
				float localT = (a_val - a_gradient->scheme[i].t) / (a_gradient->scheme[i + 1].t - a_gradient->scheme[i].t);

				return Interpolate(a_gradient->scheme[i].col, a_gradient->scheme[i + 1].col, localT);
			}
		}

		return a_gradient->scheme.back().col;
	}

	void Colour::Gradient::GenerateLUT(Scheme* a_gradient)
	{
		logger::trace("Generating gradient LUT for scheme '{}'", a_gradient->GetName());
		
		//logger::trace("Scheme stops: 0x{:X}, 0x{:X}, 0x{:X}, 0x{:X}, 0x{:X}", a_gradient.scheme.at(0).col, a_gradient.scheme.at(1).col, a_gradient.scheme.at(2).col, a_gradient.scheme.at(3).col, a_gradient.scheme.at(4).col);
		uint8_t tr, tg, tb, ta;
		HexToRGBA(HEX_COL32(0xA1B2C3FF), tr, tg, tb, ta);
		

		if (!LUT.empty()) {
			for (int i = 0; i < 256; i++) {
				float val = i / 255.0f;
				LUT.at(i) = Sample(val, a_gradient);
			}
		} else {
			for (int i = 0; i < 256; i++) {
				float val = i / 255.0f;
				LUT.push_back(Sample(val, a_gradient));
			}
		}

		logger::trace("Gradient LUT generated!");
	}

	void Colour::Gradient::RequestNewLUT(Scheme* a_gradient)
	{
		newLUTRequested = true;
		newLUT = a_gradient;
	}

	//NOTE: Currently, I'm taking in a hex colour, converting it to RGBA to modify it, changing it back to hex to store it, and then converting to RGBA again to add to texture pixel map
	//TODO: So, definitely refine/optimise this at some point. For now though, just wanna try and get it all working.
	ImU32 Colour::Gradient::GetLUTVal(float a_val)
	{
		//TODO: Add a setting/global val that determines which gradient to use, and only default to classic if none is set.
		if (LUT.empty() || newLUTRequested) {
			logger::debug("New LUT requested...");
			if (newLUT == nullptr) { newLUT = &gClassic; }

			GenerateLUT(newLUT);
			newLUTRequested = false;
		}

		if (a_val > 1.0f || a_val < 0.0f) {
			logger::warn("Value of '{}' provided to GetLUTVal is outside of bounds: [ 0.0f < val < 1.0f ] -> Clamping value", a_val);
			a_val = std::clamp(a_val, 0.0f, 1.0f);
		}

		return LUT[static_cast<ImU32>(a_val * 255)];
	}
}