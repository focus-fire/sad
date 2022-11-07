#include "sadpch.h"

#include "ResourceManager.h"

#include <Core/FileUtils.h>

#include "ResourceFactory.h"
#include "RenderableResource.h"
#include "Renderer/ShaderResource.h"

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
	if (!m_CachedResourcesFromFile.empty())
	{
		core::Log(ELogType::Info, "[ResourceManager] Reimporting resources from Resources.sad.meta into the ResourceManager");
		m_CachedResourcesFromFile.clear();
	}

	// If file doesn't exist, write the header to it
	if (!core::FileUtils::PathExists(c_ResourceFilePath))
	{
		core::Log(ELogType::Info, "[ResourceManager] Couldn't find a Resources.sad.meta file in the Data folder when importing resources, creating a new one");
		core::FileUtils::WriteFile(c_ResourceFilePath, c_ResourceFileHeader);
	}

	// Import resources to the ResourceManager's resource cache
	// Resources that exist in the file but don't exist in the filesystem will be ignored
	// These resources will be marked as 'orphans' and will require a full export in order to remove them
	bool isCleanImport = ImportResources();
	
	// Recurse through the './Data' directory to search for new resources
	// Resources that were cached during the import step are skipped
	FindResourcesInDataDirectory();

	// Export resources, method is determined on the basis of a clean or unclean initial import
	if (isCleanImport)
	{
		// Only resources that have been cached during steps 1 or 2 will be exported
		ExportUncachedResources();
	}
	else
	{
		// An orphaned file was detected during the import, therefore perform a full export
		// This overwrites the previous Resources.sad.meta file with new data
		ExportAllResources();
	}
}

bool sad::ResourceManager::ImportResources()
{
	bool isCleanImport = true;

	// Retrieve file contents and parse entries to generate map
	// Preserve line breaks to properly delimit string as csv
	const std::string resourceFileContents = core::FileUtils::ReadFile(c_ResourceFilePath, true);
	SAD_ASSERT(!resourceFileContents.empty(), "Retrieved resource file should at least have a header before being parsed");

	core::Log(ELogType::Trace, "[ResourceManager] Importing cached resources from Resources.sad.meta");

	// Avoid parsing CSV if contents only contain the header 
	if (core::StringUtils::Equals(resourceFileContents, c_ResourceFileHeader))
		return true;

	// Split the CSV by line
	std::vector<std::string> lines = core::StringUtils::Split('\n', resourceFileContents);

	for (std::string line : lines)
	{
		// Skip the header of the CSV file
		if (line == c_ResourceFileHeader)
			continue;

		// Split the CSV into it's rows
		std::vector<std::string> rows = core::StringUtils::Split(',', line);
		
		// Extract resource information and reconstruct absolute path to determine if resource still exists on the filesystem
		std::string fileName = std::move(rows[1]);
		std::string relativePath = std::move(rows[2]);
		const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(std::move(relativePath));

		if (core::FileUtils::PathExists(absolutePath))
		{
			core::Guid guid = core::Guid::RecreateGuid(rows[0]);

			// Create generic data for this resource 
			IResource::ResourceData data = { fileName, relativePath, guid };
			EResourceType type = CheckResourceType(fileName);

			// Submit resource to data factory if it has a valid type
			// Resources retrieved from the Resources.sad.meta file should be cached
			if (type != EResourceType::None)
			{
				SendDataToFactory(type, data);
				m_CachedResourcesFromFile.emplace(std::move(fileName));
			}
		}
		else
		{
			// File previously referenced in file wasn't detected, this is considered an 'unclean' import 
			core::Log(ELogType::Warn, "[ResourceManager] Detected orphaned resource - {} was referenced in Resources.sad.meta but doesn't exist on the filesystem", fileName);
			isCleanImport = false;
		}
	}

	return isCleanImport;
}

void sad::ResourceManager::FindResourcesInDataDirectory()
{
	const std::string dataDirectory = core::FileUtils::GetDataDirectory();

	std::filesystem::recursive_directory_iterator end;
	for (auto it = std::filesystem::recursive_directory_iterator(dataDirectory); it != end; ++it)
	{
		const std::filesystem::path absolutePath = it->path();

		// Skip the main resources file
		if (absolutePath == c_ResourceFilePath)
			continue;

		if (!std::filesystem::is_directory(absolutePath))
		{
			// Retrieve path relative to the './Data' directory
			std::string fileName = it->path().filename().string();

			// Cache unrecognized files if they have a VALID TYPE
			if (!m_CachedResourcesFromFile.contains(fileName))
			{
				core::Log(ELogType::Trace, "[ResourceManager] Resource not found in cache with name {} - creating a resource for it now", fileName);

				const std::string relativePath = std::filesystem::relative(absolutePath, dataDirectory).string();

				// Create standard resource data
				IResource::ResourceData data = { fileName, relativePath };
				EResourceType type = CheckResourceType(fileName);

				// Send data to factory if the resource has a valid type
				if (type != EResourceType::None)
					SendDataToFactory(type, data);
			}
		}
	}
}

void sad::ResourceManager::ExportUncachedResources()
{
	SAD_ASSERT(!m_ResourceLookup.empty(), "Attempting to export resources when none exist");
	if (m_ResourceLookup.empty())
		return;

	for (auto& it : m_ResourceLookup)
	{
		const std::string resourceFileName = it.second->GetResourceName();

		// Item wasn't cached previously, save it to the resource map
		if (!m_CachedResourcesFromFile.contains(resourceFileName))
		{
			core::Log(ELogType::Trace, "[ResourceManager] Writing uncached file {} to Resources.sad.meta", resourceFileName);
			core::FileUtils::AppendFile(c_ResourceFilePath, it.second->Serialize());
		}
	}
}

void sad::ResourceManager::ExportAllResources()
{
	SAD_ASSERT(!m_ResourceLookup.empty(), "Attempting to export resources when none exist");
	if (m_ResourceLookup.empty())
		return;

	core::FileUtils::RemoveFile(c_ResourceFilePath);
	core::Log(ELogType::Info, "[ResourceManager] Performing a full export of all resources to Resources.sad.meta");

	for (auto& it : m_ResourceLookup)
	{
		core::FileUtils::AppendFile(c_ResourceFilePath, it.second->Serialize());
	}
}

void sad::ResourceManager::SendDataToFactory(const EResourceType& resourceType, const IResource::ResourceData& resourceData)
{
	SAD_ASSERT(resourceType != EResourceType::None, "Resources with unidentified types should not be submitted to the factory for insertion");

	switch (resourceType)
	{
	case EResourceType::Model:
		ResourceFactory::CreateResource<RenderableResource>(resourceData);
		break;
	case EResourceType::Audio:
		ResourceFactory::CreateResource<AudioResource>(resourceData);
		break;
	case EResourceType::Texture:
		ResourceFactory::CreateResource<rad::TextureResource>(resourceData);
		break;
	case EResourceType::Shader:
		ResourceFactory::CreateResource<rad::ShaderResource>(resourceData);
		break;
	case EResourceType::None:
		core::Log(ELogType::Warn, "[ResourceManager] Attempting to submit {} for creation but it has an unrecognized type, ignoring it", resourceData.Name);
		break;
	}
}

sad::ResourceManager::EResourceType sad::ResourceManager::CheckResourceType(const std::string& fileName)
{
	SAD_ASSERT(!fileName.empty(), "Empty string is being evaluated as a type of resource");

	const std::filesystem::path filePath = std::filesystem::path(fileName);
	const std::string ext = filePath.extension().string();

	using String = core::StringUtils;

	if (String::Equals(ext, ".fbx"))
		return EResourceType::Model;
	
	if (String::Equals(ext, ".png") || String::Equals(ext, ".jpg"))
		return EResourceType::Texture;

	if (String::Equals(ext, ".glsl") || String::Equals(ext, ".shader"))
		return EResourceType::Shader;

	if (String::Equals(ext, ".mp3") || String::Equals(ext, ".wav"))
		return EResourceType::Audio;

	core::Log(ELogType::Warn, "[ResourceManager] {} is being processed as a resource but {} is not a valid type yet, it will be ignored.", fileName, ext);

	return EResourceType::None;
}
