#include "FileHandler.h"
#include <toml++/toml.hpp>
#include <ClibUtil/distribution.hpp>

namespace FileHandler
{
	const std::filesystem::path TOML::tomlDir = R"(Data\SKSE\Plugins\CapacityOverhaulNG\)";
	std::unordered_map<std::filesystem::path, toml::table> TOML::tomlDataMap;

	std::vector<std::filesystem::path> TOML::FindTOMLFiles()
	{
		std::vector<std::filesystem::path> tomlPaths = clib_util::distribution::get_configs_paths(tomlDir, "_CONG"sv, ".toml"sv);

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
		clib_util::Timer timer;
		timer.start();
		logger::info("{:=^50}", "Loading TOML Config Files");

		logger::info("Looking for TOML config files...");
		auto tomlPaths = FindTOMLFiles();

		if (tomlPaths.size() == 0) {
			logger::info("No TOML config files were found. If this is unexpected, ensure your files are named in the format 'xxx_CONG.toml' and are located in the folder 'Data/SKSE/Plugins/CapacityOverhaulNG/'.");
			return false;
		}

		LogFilePaths(tomlPaths);

		logger::info("Loading files...");
		ReadAllTOML(tomlPaths);

		timer.stop();
		logger::info("All TOML config files loaded. Time taken: {}μs / {}ms", timer.duration_μs(), timer.duration_ms());
		return true;
	}

	void LogFilePaths(std::vector<std::filesystem::path> file_paths)
	{
		logger::debug("TOML config files located:");

		for (auto path : file_paths) { logger::debug("{}", path.string()); }
	}
}