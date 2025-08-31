# Capacity Overhaul NG

An SKSE mod designed with the intent of completely overhauling Skyrim's item carry weight system, introducing additional limits based on the quantity of items and a range of modifiers that impact the player's carry weight limit, as well as a number of Buffs and Debuffs that apply dynamically depending on the player's current burden.

Entirely user customisable, allowing for configuration to the exact gameplay wants and needs of the player.

Built with CommonLibSSE NG, compatible with SE, AE, and VR.

[Nexus]()

## Notes
### Version Compatibility
- While this mod is built with CommonLibSSE NG - and should therefore theoretically be compatible with all versions of Skyrim Special Edition (SE, AE, VR) - I have only tested this mod on AE, and so any bugs or issues that may be present on SE or VR will likely be unknown to me unless already reported.
- I may consider doing testing on SE eventually to ensure that everything functions as it should, but I lack the hardware necessary to perform any testing for the VR version.
### Translations
- This mod is currently only available in English, but I encourage anyone with the interest and capability to translate this mod into your own languages. The localisation files can be found in the standard file location, `Data/Interface/Translations/`.
- If you have made any translations for this mod, get in touch with me and I would be happy to include them in this repo and on the mod's Nexus page (credited, of course). Alternatively, feel free to upload it yourself as a translation on Nexus.

## User Requirements
SE/AE Users:
- [SKSE64](https://www.nexusmods.com/skyrimspecialedition/mods/30379)
- [Address Library for SKSE](https://www.nexusmods.com/skyrimspecialedition/mods/32444)

VR Users:
- [SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/30457)
- [Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)

Required for MCM Functionality:
- [SkyUI](https://www.nexusmods.com/skyrimspecialedition/mods/12604)
- [MCM Helper](https://www.nexusmods.com/skyrimspecialedition/mods/53000)

## Build Requirements

- [Visual Studio 2022](https://visualstudio.microsoft.com/)
- [CMake](https://cmake.org/download/) 
  * (v3.21+)
- [vcpkg](https://github.com/microsoft/vcpkg)
  * Add `VCPKG_ROOT` as an Environment Variable, with the path to the folder in your workspace containing vcpkg as the value.

## Credits
- [`SkyrimScripting`](https://github.com/SkyrimScripting)/[`MrowrPurr`](https://github.com/mrowrpurr) for the [template plugin](https://github.com/SkyrimScripting/SKSE_Template_Logging) which this mod was built using.
  * I also owe a HUGE thanks to her [Youtube tutorials](https://youtube.com/playlist?list=PLektTyeQhBZeDIRp2g15SsK1GX2Ig8YVW&si=G10sz3apdL0PFQ9V), without which I am almost certain I would not have been able to figure out the whole SKSE plugin workflow.
- [`CharmedBaryon`](https://github.com/CharmedBaryon) and the CommonLib team for whatever witchcraft it is that they do to make CommonLibSSE a reality.
- [`Dylbill`](https://github.com/Dylbill-Iroh) for some logger code.
