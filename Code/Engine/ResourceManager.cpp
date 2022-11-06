#include "sadpch.h"

#include "ResourceManager.h"

#include <Core/FileUtils.h>

#include "ResourceFactory.h"
#include "RenderableResource.h"
#include "Renderer/Shader.h"

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
		m_CachedResourcesFromFile.clear();

	// If file doesn't exist, write the header to it
	if (!core::FileUtils::PathExists(c_ResourceFilePath))
	{
		core::Log(ELogType::Debug, "Failed to find Resources.sad.meta, creating a new one");
		core::FileUtils::WriteFile(c_ResourceFilePath, c_ResourceFileHeader);
	}

	// Import resources to the ResourceManager's resource cache
	// Resources that exist in the file but don't exist in the filesystem will be ignored
	ImportResources();
	
	// Recurse through the './Data' directory to search for new resources
	// Resources that were cached during the import step are skipped
	FindResourcesInDataDirectory();

	// Export resources back to Resources.sad.meta once import is complete
	// Only resources that have been cached during steps 1 or 2 will be exported
	ExportUncachedResources();
}

void sad::ResourceManager::ImportResources()
{
	// Retrieve file contents and parse entries to generate map
	// Preserve line breaks to properly delimit string as csv
	const std::string resourceFileContents = core::FileUtils::ReadFile(c_ResourceFilePath, true);
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

		// Split the CSV into it's rows
		std::vector<std::string> rows = core::StringUtils::Split(',', line);
		
		// Extract resource information and reconstruct absolute path to determine if resource still exists on the filesystem
		std::string fileName = std::move(rows[1]);
		std::string relativePath = std::move(rows[2]);
		const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(std::move(relativePath));

		if (core::FileUtils::PathExists(absolutePath))
		{
			core::Log(ELogType::Trace, "Inserting file from Resources.sad.meta {}", fileName);
			core::Guid guid = core::Guid::RecreateGuid(rows[0]);

			// Create generic data for this resource 
			IResource::ResourceData data = { fileName, relativePath, guid };
			EResourceType type = CheckResourceType(fileName);

			// Submit resource to data factory, if the resource was valid it should be cached
			SendDataToFactory(type, data);
			if (type != EResourceType::None)
				m_CachedResourcesFromFile.emplace(std::move(fileName));
		}
		else
		{
			core::Log(ELogType::Trace, "Could not find {} referenced in Resources.sad.meta, ignoring it", fileName);
		}
	}
}

void sad::ResourceManager::FindResourcesInDataDirectory()
{
	const std::string dataDirectory = core::FileUtils::GetDataDirectory();
	std::filesystem::recursive_directory_iterator it = std::filesystem::recursive_directory_iterator(dataDirectory);
	std::filesystem::recursive_directory_iterator end;

	for (it; it != end; ++it)
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
				core::Log(ELogType::Debug, "Resource not found in cache with name {} - creating a resource for it now", fileName);

				const std::string relativePath = std::filesystem::relative(absolutePath, dataDirectory).string();

				// Create standard resource data
				IResource::ResourceData data = { fileName, relativePath };
				EResourceType type = CheckResourceType(fileName);
				SendDataToFactory(type, data);
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
		const std::string resourceFileName = it.second->GetResourceName();

		// Item wasn't cached previously, save it to the resource map
		if (!m_CachedResourcesFromFile.contains(resourceFileName))
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

void sad::ResourceManager::SendDataToFactory(const EResourceType& resourceType, const IResource::ResourceData& resourceData)
{
	switch (resourceType)
	{
	case EResourceType::Model:
		ResourceFactory::CreateResource<RenderableResource>(resourceData);
		break;
	case EResourceType::Texture:
		ResourceFactory::CreateResource<rad::Texture>(resourceData);
		break;
	case EResourceType::Shader:
		ResourceFactory::CreateResource<rad::Shader>(resourceData);
		break;
	case EResourceType::None:
		core::Log(ELogType::Warn, 
			"{} is being processed as a resource but it does not have a valid type, it will be ignored.", 
			resourceData.Name);
		break;
	}
}

sad::ResourceManager::EResourceType sad::ResourceManager::CheckResourceType(const std::string& fileName)
{
	const std::filesystem::path filePath = std::filesystem::path(fileName);
	const std::string ext = filePath.extension().string();

	using String = core::StringUtils;

	if (String::Equals(ext, ".fbx"))
		return EResourceType::Model;
	
	if (String::Equals(ext, ".png") || String::Equals(ext, ".jpg"))
		return EResourceType::Texture;

	if (String::Equals(ext, ".glsl") || String::Equals(ext, ".shader"))
		return EResourceType::Shader;

	return EResourceType::None;
}
