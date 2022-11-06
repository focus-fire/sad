#pragma once

#include <filesystem>
#include <type_traits>

#include <Core/Memory.h>
#include <Core/FileUtils.h>

#include "IResource.h"
#include "ResourceManager.h"
#include "RenderableResource.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

namespace sad
{
	class ResourceFactory
	{
	public:
		template<class T>
		static void CreateResource(const IResource::ResourceData& data) = delete;

		template<>
		static void CreateResource<rad::Shader>(const IResource::ResourceData& data)
		{ 
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);

			core::Pointer<rad::Shader> shaderResource = core::CreatePointer<rad::Shader>(data, absolutePath);

			ResourceManager::AddResource<rad::Shader>(shaderResource);
		}

		template<>
		static void CreateResource<rad::Texture>(const IResource::ResourceData& data)
		{
			const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(data.FilePath);

			core::Pointer<rad::Texture> textureResource = core::CreatePointer<rad::Texture>(data, absolutePath);

			ResourceManager::AddResource<rad::Texture>(textureResource);
		}

		template<>
		static void CreateResource<RenderableResource>(const IResource::ResourceData& data)
		{
			core::Pointer<RenderableResource> renderableResource = core::CreatePointer<RenderableResource>(data);

			ResourceManager::AddResource<RenderableResource>(renderableResource);
		}
	};
}
