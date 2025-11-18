#include "FileHandler.h"
#include <toml++/toml.hpp>
#include <ClibUtil/distribution.hpp>

namespace FileHandler
{
	const std::filesystem::path TOML::tomlDir = R"(Data\SKSE\Plugins\Capacity Overhaul NG\)";
	std::unordered_map<std::filesystem::path, toml::table> TOML::tomlDataMap;

	std::vector<std::filesystem::path> TOML::FindTOMLFiles()
	{
		std::vector<std::filesystem::path> tomlPaths = clib_util::distribution::get_configs_paths(tomlDir, "_CONG"sv, ".toml"sv);

		LogFilePaths(tomlPaths);

		return tomlPaths;
	}

	void TOML::ReadAllTOML(std::vector<std::filesystem::path> file_paths)
	{
		tomlDataMap.clear();

		for (auto path : file_paths) {
			try
			{
				toml::table result = toml::parse_file(path.string());
				tomlDataMap.insert({path, result});
			}
			catch(const toml::parse_error& err)
			{
				logger::error("Failed to load TOML file: {}", err.description());
			}
		}
	}

	bool TOML::LoadTOMLData()
	{
		logger::info("Looking for TOML config files...");
		auto tomlPaths = FindTOMLFiles();

		if (tomlPaths.size() == 0) {
			logger::info("No TOML config files were found. If this is unexpected, ensure your files are named in the format 'xxx_CONG.toml' and are located in the folder 'Data/SKSE/Plugins/CapacityOverhaulNG/'.");
			return false;
		}

		logger::info("Loading TOML config files...");
		ReadAllTOML(tomlPaths);

		logger::info("All TOML config files loaded!\n{}", std::string(100, '-'));
		return true;
	}

	void LogFilePaths(std::vector<std::filesystem::path> file_paths)
	{
		std::string pathsStr;

		for (auto path : file_paths) {
			pathsStr.append(path.string());
			pathsStr.append("\n");
		}

		if (pathsStr.size() == 0) { return; }
		logger::debug("TOML config files located:\n{}{}", pathsStr, std::string(100, '-'));
	}
}