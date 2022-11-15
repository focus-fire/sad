#pragma once

#include <filesystem>
#include <type_traits>

#include <Core/Memory.h>
#include <Core/FileUtils.h>

#include "Resource.h"
#include "ResourceManager.h"
#include "LevelResource.h"
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
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.DataPath);
			const std::filesystem::path filePath = std::filesystem::path(absolutePath);
			const std::string extension = filePath.extension().string();

			// Note: If more then .wav or .mp3 audio types are added, these will have to be updated
			//		 This at least ensures that audio files are marked as .wav, .mp3, or None
			AudioResource::EAudioType type = AudioResource::EAudioType::None;
			if (core::StringUtils::Equals(extension, ".wav"))
			{
				type = AudioResource::EAudioType::WAV;
			}
			else if (core::StringUtils::Equals(extension, ".mp3"))
			{
				type = AudioResource::EAudioType::MP3;
			}

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
			using ETextureType = rad::TextureResource::ETextureType;
			ETextureType textureType = ETextureType::Normal;

			// Check if the texture file contains a 'skybox' suffix
			// These textures are marked as being part of a cube map
			// ie: 'Top.skybox.png
			if (data.Name.find(".skybox.") != std::string::npos)
				textureType = ETextureType::CubeMap;

			// Create a standard image/texture resource
			core::Pointer<rad::TextureResource> textureResource = core::CreatePointer<rad::TextureResource>(data, textureType);

			ResourceManager::AddResource<rad::TextureResource>(textureResource);
			core::Log(ELogType::Trace, "[ResourceFactory] Created a TextureResource with name {}", data.Name);
		}

		/**
		 * @brief Factory construction for ShaderResources 
		*/
		template<>
		static void CreateResource<rad::ShaderResource>(const Resource::ResourceData& data)
		{ 
			core::Pointer<rad::ShaderResource> shaderResource = core::CreatePointer<rad::ShaderResource>(data);
			ResourceManager::AddResource<rad::ShaderResource>(shaderResource);
			core::Log(ELogType::Trace, "[ResourceFactory] Created a ShaderResource with name {}", data.Name);
		}

		template<>
		static void CreateResource<LevelResource>(const Resource::ResourceData& data)
		{
			core::Pointer<LevelResource> levelResource = core::CreatePointer<LevelResource>(data);
			ResourceManager::AddResource<LevelResource>(levelResource);
			core::Log(ELogType::Trace, "[ResourceFactory] Created a LevelResource with name {}", data.Name);
		}
	};
}
