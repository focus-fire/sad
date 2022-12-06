#include "sadpch.h"

#include "LevelManager.h"

#include <iomanip>

#include <nlohmann/json.hpp>

#include "LevelResource.h"
#include "LevelSnapshotInput.h"
#include "LevelSnapshotOutput.h"
#include "ResourceManager.h"

#include "Engine/ECS/Registry.h"
#include "Engine/ECS/Components/ComponentTypes.h"

using json = nlohmann::json;

sad::Level* sad::LevelManager::ImportLevel(int saveFileId)
{
	std::stringstream id;
	id << std::setw(2) << std::setfill('0') << saveFileId;
	
	// Retrieve level resource from ResourceManager
	std::string levelFileName = "SaveFile" + id.str() + ".json";
	LevelResource* levelResource = ResourceManager::GetResource<LevelResource>(levelFileName);

	if (!levelResource)
	{
		core::Log(ELogType::Warn, "Save file doesn't exist, creating a new one");

		// Create a new save file
		const std::string newSaveFile = core::FileUtils::GetPathInsideDataDirectory("Save/" + levelFileName);
		core::FileUtils::WriteFile(newSaveFile, "");
		
		// TODO: Add a file watcher so the resource manager can detect file changes
		ResourceManager::Import();
		
		return new sad::Level();
	}

	// Read level file contents into JSON string
	std::string levelFileJson = core::FileUtils::ReadFile(levelResource->GetResourceAbsolutePath());

	// Create input archive for JSON snapshot
	Level* level = new sad::Level();
	level->LevelName = levelResource->GetResourceFileName();
	LevelSnapshotInput levelSnapshot = LevelSnapshotInput(levelFileJson);

	// Load JSON snapshot
	entt::basic_snapshot_loader loader = entt::basic_snapshot_loader(sad::ecs::Registry::GetEntityWorld());
	loader.entities(levelSnapshot).component<SERIALIZED_COMPONENT_TYPES>(levelSnapshot);

	// Populate GUIDs throughout entities in the level
	level->PopulateLevelGuids();

	return level;
}

bool sad::LevelManager::ExportLevel(int saveFileId)
{
	// Prevent default save file from changing
	if (saveFileId == 0)
	{
		core::Log(ELogType::Error, "SaveFile00.json is the default save file, don't change this");
		return false;
	}
	else if (0 > saveFileId && saveFileId < 100)
	{
		core::Log(ELogType::Error, "{} is an invalid save file ID", saveFileId);
		return false;
	}

	std::stringstream id;
	id << std::setw(2) << std::setfill('0') << saveFileId;

	std::string levelFileName = "SaveFile" + id.str() + ".json";
	LevelResource* levelResource = ResourceManager::GetResource<LevelResource>(levelFileName);

	// Prepare an export of the snapsho
	LevelSnapshotOutput jsonLevelOutput;
	entt::basic_snapshot snapshot = entt::basic_snapshot(sad::ecs::Registry::GetEntityWorld());
	snapshot.entities(jsonLevelOutput).component<NEW_SERIALIZED_COMPONENT_TYPES>(jsonLevelOutput);
	jsonLevelOutput.Close();

	// Write the export to the save file
	std::string jsonOutput = jsonLevelOutput.AsString();
	core::FileUtils::WriteFile(levelResource->GetResourceAbsolutePath(), jsonOutput);

	return true;
}