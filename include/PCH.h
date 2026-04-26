#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>
#include <SimpleIni.h>
#include "REX/REX/Singleton.h"
#include <ClibUtil/timer.hpp>

#include "Settings.h"
#include "Utils.h"
#include "Translations.h"

namespace logger = SKSE::log;

using namespace std::literals;

using ImU32 = unsigned int;

#define IM_COL32_R_SHIFT 0
#define IM_COL32_G_SHIFT 8
#define IM_COL32_B_SHIFT 16
#define IM_COL32_A_SHIFT 24
#define IM_COL32_A_MASK 0xFF000000
#define IM_COL32(R, G, B, A)                                                                                  \
(((ImU32)(A) << IM_COL32_A_SHIFT) | ((ImU32)(B) << IM_COL32_B_SHIFT) | ((ImU32)(G) << IM_COL32_G_SHIFT) | \
((ImU32)(R) << IM_COL32_R_SHIFT))

#define HEX_COL32(hex) IM_COL32(((hex >> 24) & 0xFF), ((hex >> 16) & 0xFF), ((hex >> 8) & 0xFF), ((hex >> 0) & 0xFF))