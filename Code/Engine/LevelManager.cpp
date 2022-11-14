#include "sadpch.h"

#include "LevelManager.h"

#include <iomanip>

#include <nlohmann/json.hpp>

#include "LevelSnapshotInput.h"
#include "LevelSnapshotOutput.h"

#include "Engine/ECS/Registry.h"
#include "Engine/ECS/Components/ComponentTypes.h"

using json = nlohmann::json;

sad::Level* sad::LevelManager::ImportLevel(int saveFile)
{
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << saveFile;
	std::string levelFilePath = core::FileUtils::GetPathInsideDataDirectory("/Save/SaveFile" + ss.str() + ".json");

	if (!core::FileUtils::PathExists(levelFilePath))
	{
		core::Log(ELogType::Error, "Save file doesn't exist");
		return nullptr;
	}

	std::string levelFileJson = core::FileUtils::ReadFile(levelFilePath);

	// Create input archive for JSON snapshot
	Level* level = new sad::Level();
	LevelSnapshotInput levelSnapshot = LevelSnapshotInput(levelFileJson);

	// Load JSON snapshot
	entt::basic_snapshot_loader loader = entt::basic_snapshot_loader(sad::ecs::Registry::GetEntityWorld());
	loader.entities(levelSnapshot).component<SERIALIZED_COMPONENT_TYPES>(levelSnapshot);

	// Populate GUIDs throughout entities in the level
	level->PopulateLevelGuids();

	return level;
}

bool sad::LevelManager::ExportLevel(int saveFile)
{
	//prevent default save file from changing
	if (saveFile == 0)
	{
		core::Log(ELogType::Error, "SaveFile00.json is the default save file, don't change this");
		return false;
	}
	else if (0 > saveFile && saveFile < 100)
	{
		core::Log(ELogType::Error, "{} is an invalid save file ID", saveFile);
		return false;
	}

	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << saveFile;
	std::string levelFilePath = core::FileUtils::GetPathInsideDataDirectory("/Save/SaveFile" + ss.str() + ".json");

	LevelSnapshotOutput jsonLevelOutput;
	entt::basic_snapshot snapshot = entt::basic_snapshot(sad::ecs::Registry::GetEntityWorld());

	snapshot.entities(jsonLevelOutput).component<SERIALIZED_COMPONENT_TYPES>(jsonLevelOutput);
	jsonLevelOutput.Close();

	std::string jsonOutput = jsonLevelOutput.AsString();
	core::FileUtils::WriteFile(levelFilePath, jsonOutput);

	return true;
}