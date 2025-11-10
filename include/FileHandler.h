#pragma once
#include <toml++/toml.hpp>

namespace FileHandler
{
	class TOML
	{
		static const std::filesystem::path tomlDir;

		static std::vector<std::filesystem::path> FindTOMLFiles();

		static void ReadAllTOML(std::vector<std::filesystem::path> file_paths);

		public:
			static std::unordered_map<std::filesystem::path, toml::table> tomlDataMap;

			static bool LoadTOMLData();
	};

	void LogFilePaths(std::vector<std::filesystem::path> file_paths);
}