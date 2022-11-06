#pragma once

#include <utility>
#include <unordered_map>
#include <unordered_set>

#include <Core/Log.h>
#include <Core/Assert.h>
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
			, m_CachedResourcesFromFile()
		{ }

		enum class EResourceType
		{
			None,
			Model,
			Audio,
			Texture,
			Shader
		};

	private:
		const std::string c_ResourceFilePath = core::FileUtils::GetPathInsideDataDirectory("Resources.sad.meta");
		const std::string c_ResourceFileHeader = "GUID,Name,Path";

		/**
		 * @brief Main lookup for instantiated resources
		*/
		std::unordered_map<core::Guid, core::Pointer<IResource>> m_ResourceLookup;

		/**
		 * @brief Local cache for resources that have been imported via MImport().
		*/
		std::unordered_set<std::string> m_CachedResourcesFromFile;

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
		 * @brief Imports resources from the Resources.sad.meta file and stores them into m_CachedResources
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

		/**
		 * @brief Evaluates a resource type based on the extension of a file name
		 * @param fileName String containing the file whose type needs to be checked
		 * @return Enum containing data for the type of resource returned
		*/
		EResourceType CheckResourceType(const std::string& fileName);

		/**
		 * @brief Submits a resource and its type information to the ResourceFactory
		 * @param resourceType EResourceType enum dictating which type of resource it is
		 * @param resourceData Mandatory ResourceData struct that initializes the resource
		*/
		void SendDataToFactory(const EResourceType& resourceType, const IResource::ResourceData& resourceData);

	public:
		/**
		 * @brief Type trait ensuring that passed template parameters inherit from IResource
		 * @tparam T typename that inherits from IResource
		*/
		template<typename T>
		using ResourceType = std::enable_if<std::is_base_of<IResource, T>::value>;

		/**
		 * @brief Adds a smart pointer for a resource into the ResourceManager
		 * @tparam T Class type inheriting from IResource
		 * @param resource Reference to the smart pointer containing the resource being inserted 
		*/
		template<class T, typename = ResourceType<T>>
		static void AddResource(const core::Pointer<T>& resource)
		{
			GetInstance().MAddResource<T>(resource);
		}

		/**
		 * @brief Retrieve a resource from the ResourceManager by a valid GUID 
		 * @tparam T Class type inheriting from IResource
		 * @param guid Refernce to a GUID object containing a valid GUID
		 * @return Pointer to the reference being retrieved, nullptr if the resource isn't found
		*/
		template<class T, typename = ResourceType<T>>
		static T* GetResource(const core::Guid& guid)
		{
			return GetInstance().MGetResource<T>(guid);
		}

		/**
		 * @brief Retrieve a resource from the ResourceManager by a valid name
		 * @tparam T Class type inheriting from IResource
		 * @param name Reference to a string containing a valid resource name 
		 * @return Pointer to the reference being retrieved, nullptr if the resource isn't found
		*/
		template<class T, typename = ResourceType<T>>
		static T* GetResource(const std::string& name)
		{
			return GetInstance().MGetResource<T>(name);
		}
		
	private:
		template<class T, typename = ResourceType<T>>
		void MAddResource(const core::Pointer<T>& resource)
		{
			SAD_ASSERT(resource, "Attempting to instert a null resource!")
			m_ResourceLookup.emplace(resource->GetResourceId(), resource);
		}

		template<class T, typename = ResourceType<T>>
		T* MGetResource(const core::Guid& guid)
		{
			SAD_ASSERT(!guid.IsNull(), "Attempting to retrieve a resource with a null GUID!");
			core::Log(ELogType::Trace, "[ResourceManager] Attempting to retrieve resource with GUID {}", guid);

			std::unordered_map<core::Guid, core::Pointer<IResource>>::const_iterator it = m_ResourceLookup.find(guid);
			return it == m_ResourceLookup.end() ? nullptr : dynamic_cast<T*>(it->second);
		}

		template <class T, typename = ResourceType<T>>
		T* MGetResource(const std::string& name)
		{
			SAD_ASSERT(!name.empty(), "Attempting to retrieve a resource with an empty string!");
			core::Log(ELogType::Trace, "[ResourceManager] Attempting to retrieve resource with name {}", name);

			for (auto& it : m_ResourceLookup)
			{
				const std::string& resourceName = it.second->GetResourceName();
				if (core::StringUtils::Equals(name, resourceName))
					return dynamic_cast<T*>(it.second.get());
			}

			return nullptr;
		}
	};
}
