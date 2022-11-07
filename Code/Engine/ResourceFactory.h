#pragma once

#include <filesystem>
#include <type_traits>

#include <Core/Memory.h>
#include <Core/FileUtils.h>

#include "IResource.h"
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
		 * @brief Factory method to create and construct IResources
		 * @tparam ResourceType Class type inheriting from IResource
		 * @param data Required default set of data for serialization of IResources
		*/
		template<class ResourceType>
		static void CreateResource(const IResource::ResourceData& data) = delete;

		/**
		 * @brief Factory construction for RenderableResources 
		*/
		template<>
		static void CreateResource<RenderableResource>(const IResource::ResourceData& data)
		{
			core::Pointer<RenderableResource> renderableResource = core::CreatePointer<RenderableResource>(data);
			SAD_ASSERT(renderableResource, "Failed to initialize pointer to a RenderableResource");
			core::Log(ELogType::Trace, "[ResourceFactory] Created a RenderableResource with name {}", data.Name);

			ResourceManager::AddResource<RenderableResource>(renderableResource);
		}

		/**
		 * @brief Factory construction for AudioResources 
		*/
		template<>
		static void CreateResource<AudioResource>(const IResource::ResourceData& data)
		{
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);
			const std::filesystem::path filePath = std::filesystem::path(absolutePath);
			const std::string extension = filePath.extension().string();

			// Note: If more then .wav or .mp3 audio types are added, these will have to be updated
			//       At the moment, this at least ensures that all audio resources will be created with a type by default
			using AudioType = AudioResource::EAudioType;
			AudioType type = core::StringUtils::Equals(extension, ".wav") ? AudioType::WAV : AudioType::MP3;

			core::Pointer<AudioResource> audioResource = core::CreatePointer<AudioResource>(data, type);
			SAD_ASSERT(audioResource, "Failed to initialize pointer to a AudioResource");
			core::Log(ELogType::Trace, "[ResourceFactory] Created a AudioResource with name {}", data.Name);

			ResourceManager::AddResource<AudioResource>(audioResource);
		}

		/**
		 * @brief Factory construction for TextureResources
		*/
		template<>
		static void CreateResource<rad::TextureResource>(const IResource::ResourceData& data)
		{
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);

			core::Pointer<rad::TextureResource> textureResource = core::CreatePointer<rad::TextureResource>(data, absolutePath);
			SAD_ASSERT(textureResource, "Failed to initialize pointer to a TextureResource");
			core::Log(ELogType::Trace, "[ResourceFactory] Created a TextureResource with name {}", data.Name);

			ResourceManager::AddResource<rad::TextureResource>(textureResource);
		}

		/**
		 * @brief Factory construction for ShaderResources 
		*/
		template<>
		static void CreateResource<rad::ShaderResource>(const IResource::ResourceData& data)
		{ 
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);

			core::Pointer<rad::ShaderResource> shaderResource = core::CreatePointer<rad::ShaderResource>(data, absolutePath);
			SAD_ASSERT(shaderResource, "Failed to initialize pointer to a ShaderResource");
			core::Log(ELogType::Trace, "[ResourceFactory] Created a ShaderResource with name {}", data.Name);

			ResourceManager::AddResource<rad::ShaderResource>(shaderResource);
		}
	};
}
