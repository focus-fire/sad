#pragma once

#include <utility>
#include <unordered_map>
#include <unordered_set>

#include <Core/Guid.h>

#include "IResource.h"

namespace sad
{
	/**
	 * @brief Determines the allocation, storage, and management of resources throughout the engine
	*/
	class ResourceManager
	{
	private:
		ResourceManager()
			: m_ResourceLookup()
			, m_CachedResources()
		{ }

	private:
		const std::string c_ResourceFilePath = core::FileUtils::GetPathInsideDataDirectory("Resources.sad.meta");
		const std::string c_ResourceFileHeader = "GUID,Name,Path";

		const std::string c_ShaderFileExtensions[2] = { ".glsl", ".shader" };
		const std::string c_TextureFileExtensions[2] = { ".png", ".jpg" };
		const std::string c_RenderableFileExtensions[1] = { ".fbx" };

		/**
		 * @brief Main lookup for instantiated resources
		*/
		std::unordered_map<core::Guid, core::Pointer<IResource>> m_ResourceLookup;

		/**
		 * @brief Local cache for resources that have been imported via MImport().
		*/
		std::unordered_set<std::string> m_CachedResources;

	public:
		ResourceManager(ResourceManager& other) = delete;

		static ResourceManager& GetInstance();

		/**
		 * @brief Singleton access for MImport() 
		*/
		static void Import();

	private:
		/**
		 * @brief Performs an import on resources in the Resources.sad.meta file as well as new resources in the './Data' directory 
		*/
		void MImport();

		/**
		 * @brief Helper method that imports resources from the Resources.sad.meta file and stores them into m_CachedResources
		 * @note Paths for all resources are checked on the local filesystem, if a resource that is previously cached is not found - it will be ignored.  
		*/
		void ImportResources();

		/**
		 * @brief Recurses through all files in the './Data' directory of the project in search of un-cached resources.
		 *		  Loads resources that weren't cached on previous import.
		*/
		void FindResourcesInDataDirectory();

		/**
		 * @brief Exports individual resources to the Resources.sad.meta file if they weren't cached during the ImportResources() step 
		*/
		void ExportUncachedResources();

		/**
		 * @brief Rewrites the Resources.sad.meta file with all resources stored in the local lookup
		*/
		void ExportAllResources();
	};
}
