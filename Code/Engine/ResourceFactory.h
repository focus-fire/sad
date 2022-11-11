#pragma once

#include <filesystem>
#include <type_traits>

#include <Core/Memory.h>
#include <Core/FileUtils.h>

#include "Resource.h"
#include "ResourceManager.h"
#include "RenderableResource.h"
#include "AudioResource.h"
#include "Renderer/TextureResource.h"
#include "Renderer/ShaderResource.h"

namespace sad
{
	/**
	 * @brief Factory class used by the ResourceManager to provision the creation of specific resources
	*/
	class ResourceFactory
	{
	public:
		/**
		 * @brief Factory method to create and construct Resources
		 * @tparam ResourceType Class type inheriting from Resource
		 * @param data Required default set of data for serialization of Resources
		*/
		template<class ResourceType>
		static void CreateResource(const Resource::ResourceData& data) = delete;

		/**
		 * @brief Factory construction for RenderableResources 
		*/
		template<>
		static void CreateResource<RenderableResource>(const Resource::ResourceData& data)
		{
			core::Pointer<RenderableResource> renderableResource = core::CreatePointer<RenderableResource>(data);
			ResourceManager::AddResource<RenderableResource>(renderableResource);
			core::Log(ELogType::Trace, "[ResourceFactory] Created a RenderableResource with name {}", data.Name);
		}

		/**
		 * @brief Factory construction for AudioResources 
		*/
		template<>
		static void CreateResource<AudioResource>(const Resource::ResourceData& data)
		{
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);
			const std::filesystem::path filePath = std::filesystem::path(absolutePath);
			const std::string extension = filePath.extension().string();

			// Note: If more then .wav or .mp3 audio types are added, these will have to be updated
			//       At the moment, this at least ensures that all audio resources will be created with a type by default
			using AudioType = AudioResource::EAudioType;
			AudioType type = core::StringUtils::Equals(extension, ".wav") ? AudioType::WAV : AudioType::MP3;

			core::Pointer<AudioResource> audioResource = core::CreatePointer<AudioResource>(data, type);
			ResourceManager::AddResource<AudioResource>(audioResource);
			core::Log(ELogType::Trace, "[ResourceFactory] Created a AudioResource with name {}", data.Name);
		}

		/**
		 * @brief Factory construction for TextureResources
		*/
		template<>
		static void CreateResource<rad::TextureResource>(const Resource::ResourceData& data)
		{
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);

			core::Pointer<rad::TextureResource> textureResource = core::CreatePointer<rad::TextureResource>(data, absolutePath);
			ResourceManager::AddResource<rad::TextureResource>(textureResource);
			core::Log(ELogType::Trace, "[ResourceFactory] Created a TextureResource with name {}", data.Name);
		}

		/**
		 * @brief Factory construction for ShaderResources 
		*/
		template<>
		static void CreateResource<rad::ShaderResource>(const Resource::ResourceData& data)
		{ 
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);

			core::Pointer<rad::ShaderResource> shaderResource = core::CreatePointer<rad::ShaderResource>(data, absolutePath);
			ResourceManager::AddResource<rad::ShaderResource>(shaderResource);
			core::Log(ELogType::Trace, "[ResourceFactory] Created a ShaderResource with name {}", data.Name);
		}
	};
}
