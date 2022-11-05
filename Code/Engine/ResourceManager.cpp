#include "sadpch.h"

#include "ResourceManager.h"

#include <Core/FileUtils.h>

sad::ResourceManager& sad::ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

void sad::ResourceManager::Start()
{
	GetInstance().MStart();
}

void sad::ResourceManager::MStart()
{
	// Load GUID resource file 
	//	- Check if it exists
	//		- If it does:
	//			- Load resources that exist (according to file)
	//			- Check workspace for unclean files
	//		- If it does not:
	//			- Initialize resource map file

	core::Log(ELogType::Trace, "Checking if a valid resource file already exists at {}", c_ResourceFilePath);

	// If file doesn't exist, write the header to it
	if (!core::FileUtils::PathExists(c_ResourceFilePath))
	{
		core::Log(ELogType::Debug, "The file resource configuration file was not found, starting a new one!");
		core::FileUtils::WriteFile(c_ResourceFilePath, c_ResourceFileHeader);
	}

	// Retrieve file contents and parse entries to generate map
	// Preserve line breaks to properly delimit string as csv
	std::string resourceMapFileContents = core::FileUtils::ReadFile(c_ResourceFilePath, true);
	ParseResourceMapCSV(resourceMapFileContents);
	
	// Recursively search the data directory
	std::filesystem::recursive_directory_iterator it = std::filesystem::recursive_directory_iterator(core::FileUtils::GetDataDirectory());
	std::filesystem::recursive_directory_iterator end;

	for (it; it != end; ++it)
	{
		std::filesystem::path filePath = it->path();

		if (!std::filesystem::is_directory(filePath))
		{
			core::Log(ELogType::Debug, "File -> {}  @  Path -> {}", filePath.filename().string(), filePath.string());

			// Retrieve path relative to the './Data' directory
			std::filesystem::path relativePath = std::filesystem::relative(filePath, core::FileUtils::GetDataDirectory());
			core::Log(ELogType::Debug, "...with relative path {}", core::FileUtils::ConvertOSPathString(relativePath.string()));
		}
	}

	// Idea: Emplace resources using smart pointers?
	//  core::Pointer<IResource> resource = core::CreatePointer<IResource>("Test.fbx");
	//	m_ResourceMap.emplace(resource->GetResourceId(), resource);

	// Idea: Use set intersections to determine which files need to be recached
	//	std::unordered_set<std::string> cached = { "A", "B", "C" };
	//	std::unordered_set<std::string> fileSystem = { "A", "B", "D" };
	//	std::unordered_set<std::string> out;
	//	std::set_symmetric_difference(cached.begin(), cached.end(), fileSystem.begin(), fileSystem.end(), std::inserter(out, out.begin()));
}

void sad::ResourceManager::ParseResourceMapCSV(const std::string& csvFileContents)
{
	SAD_ASSERT(!csvFileContents.empty(), "Retrieved resource file should at least have a header before being parsed");
	core::Log(ELogType::Trace, "Beginning to parse cached resource from CSV file...");

	// Avoid parsing CSV if contents only contain the header 
	if (core::StringUtils::Equals(csvFileContents, c_ResourceFileHeader))
		return;

	// Split the CSV by line
	std::vector<std::string> lines = core::StringUtils::Split('\n', csvFileContents);

	for (std::string line : lines)
	{
		// Skip the header of the CSV file
		if (line == c_ResourceFileHeader)
			continue;

		// Split the CSV by value
		std::vector<std::string> values = core::StringUtils::Split(',', line);
		
		// Construct map entries... 
		ResourceMapEntry mapEntry = { 
			core::Guid::RecreateGuid(values[0]), 
			values[1].c_str(),
			values[2].c_str(),
			values[3].c_str()
		};

		core::Log(ELogType::Debug, "GUID -> {}", mapEntry.m_Guid.ToString());
		core::Log(ELogType::Debug, "Name -> {}", mapEntry.m_Name);
		core::Log(ELogType::Debug, "Type -> {}", mapEntry.m_Type);
		core::Log(ELogType::Debug, "Path -> {}", mapEntry.m_FilePath);
	}
}
