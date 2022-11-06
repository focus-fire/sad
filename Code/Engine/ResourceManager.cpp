#include "sadpch.h"

#include "ResourceManager.h"

#include <Core/FileUtils.h>

sad::ResourceManager& sad::ResourceManager::GetInstance()
{
	static ResourceManager instance;
	return instance;
}

void sad::ResourceManager::Import()
{
	GetInstance().MImport();
}

void sad::ResourceManager::MImport()
{
	// Empty the resource cache if a reimport is called 
	if (!m_CachedResources.empty())
		m_CachedResources.clear();

	// If file doesn't exist, write the header to it
	if (!core::FileUtils::PathExists(c_ResourceFilePath))
	{
		core::Log(ELogType::Debug, "Failed to find Resources.sad.meta, creating a new one");
		core::FileUtils::WriteFile(c_ResourceFilePath, c_ResourceFileHeader);
	}

	// Import resources to the ResourceManager's resource cache
	// Resources that exist in the file but don't exist in the filesystem will be ignored
	ImportResources();
	
	// Recurse through the './Data' directory to search new resources
	// Resources that were cached during the import step are skipped
	FindResourcesInDataDirectory();

	// Export resources back to Resources.sad.meta once import is complete
	// Only resources that have been cached during either steps 1 or 2 will be exported
	ExportUncachedResources();
}

void sad::ResourceManager::ImportResources()
{
	// Retrieve file contents and parse entries to generate map
	// Preserve line breaks to properly delimit string as csv
	std::string resourceFileContents = core::FileUtils::ReadFile(c_ResourceFilePath, true);
	SAD_ASSERT(!resourceFileContents.empty(), "Retrieved resource file should at least have a header before being parsed");

	core::Log(ELogType::Trace, "Importing cached resources from Resources.sad.meta");

	// Avoid parsing CSV if contents only contain the header 
	if (core::StringUtils::Equals(resourceFileContents, c_ResourceFileHeader))
		return;

	// Split the CSV by line
	std::vector<std::string> lines = core::StringUtils::Split('\n', resourceFileContents);

	for (std::string line : lines)
	{
		// Skip the header of the CSV file
		if (line == c_ResourceFileHeader)
			continue;

		// Split the CSV by value
		std::vector<std::string> values = core::StringUtils::Split(',', line);
		
		// Extract resource information and reconstruct absolute path to determine if resource still exists on the filesystem
		std::string fileName = std::move(values[1]);
		std::string relativePath = std::move(values[2]);
		std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(std::move(relativePath));

		if (core::FileUtils::PathExists(absolutePath))
		{
			core::Log(ELogType::Trace, "Inserting file from Resources.sad.meta {}", fileName);

			core::Guid guid = core::Guid::RecreateGuid(values[0]);
			core::Pointer<IResource> resource = core::CreatePointer<IResource>(fileName, relativePath, guid);

			m_ResourceLookup.emplace(guid, resource);
			m_CachedResources.emplace(std::move(fileName));
		}
		else
		{
			core::Log(ELogType::Trace, "Could not find {} referenced in Resources.sad.meta, ignoring it", fileName);
		}
	}
}

void sad::ResourceManager::FindResourcesInDataDirectory()
{
	// Recursively search the data directory
	std::string dataDirectory = core::FileUtils::GetDataDirectory();
	std::filesystem::recursive_directory_iterator it = std::filesystem::recursive_directory_iterator(dataDirectory);
	std::filesystem::recursive_directory_iterator end;

	for (it; it != end; ++it)
	{
		std::filesystem::path absolutePath = it->path();

		// Skip the main resources file
		if (absolutePath == c_ResourceFilePath)
			continue;

		if (!std::filesystem::is_directory(absolutePath))
		{
			// Retrieve path relative to the './Data' directory
			std::string fileName = it->path().filename().string();

			// Cache unrecognized files if they have a VALID TYPE
			if (!m_CachedResources.contains(fileName))
			{
				core::Log(ELogType::Debug, "Resource not found in cache with name {} - creating a resource for it now", fileName);

				std::string relativePath = std::filesystem::relative(absolutePath, dataDirectory).string();
				core::Pointer<IResource> resource = core::CreatePointer<IResource>(fileName, relativePath);

				m_ResourceLookup.emplace(resource->GetResourceId(), resource);
			}
		}
	}
}

void sad::ResourceManager::ExportUncachedResources()
{
	if (m_ResourceLookup.empty())
		return;

	for (auto& it : m_ResourceLookup)
	{
		std::string resourceFileName = it.second->GetResourceName();

		// Item wasn't cached previously, save it to the resource map
		if (!m_CachedResources.contains(resourceFileName))
		{
			core::Log(ELogType::Debug, "Writing uncached file {} to Resources.sad.meta", resourceFileName);
			core::FileUtils::AppendFile(c_ResourceFilePath, it.second->Serialize());
		}
	}
}

void sad::ResourceManager::ExportAllResources()
{
	if (m_ResourceLookup.empty())
		return;

	core::FileUtils::RemoveFile(c_ResourceFilePath);
	core::Log(ELogType::Trace, "Performing a full export of all resources to Resources.sad.meta");

	for (auto& it : m_ResourceLookup)
	{
		core::FileUtils::AppendFile(c_ResourceFilePath, it.second->Serialize());
	}
}
