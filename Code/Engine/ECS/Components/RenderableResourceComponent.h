#pragma once

#include <nlohmann/json.hpp>

#include <Core/StringUtils.h>

#include <Engine/Resource.h>
#include <Engine/ResourceManager.h>
#include <Engine/RenderableResource.h>
#include <Engine/Renderer/Sample/Cube.h>

namespace sad::ecs
{
	/**
	 * @brief Contains a reference for a particular ModelResourceComponent 
	*/
	struct ModelResourceComponent
	{
		ModelResource* m_Model;
		bool m_IsResourceDirty;
	};

	/**
	 * @brief Component containing a reference for a particular PrimitiveResource 
	 *
	 * @note Most PrimitiveResourceComponent's should be initialized with an m_IsResourceDirty of 'true'.
	 *		 This ensures that they get absolved into a RenderablePrimitive in the next frame.
	*/
	struct PrimitiveResourceComponent
	{
		core::Pointer<PrimitiveResource> m_Primitive;
		bool m_IsResourceDirty;
	};

	/**
	 * @brief Contains name for a sprite attached to a particular RenderableSprite
	 */
	struct SpriteResourceComponent
	{
		std::string m_SpriteName;
		bool m_IsResourceDirty;
	};

	inline void to_json(nlohmann::json& JSON, const sad::ecs::ModelResourceComponent& renderableResource)
	{
		JSON =
		{
			{ "ModelFileName", renderableResource.m_Model->GetResourceFileName()},
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::ModelResourceComponent& renderableResource)
	{
		std::string modelFileName = JSON["ModelFileName"];

		ModelResource* modelResource = ResourceManager::GetResource<ModelResource>(std::move(modelFileName));
		renderableResource.m_Model = modelResource;
		renderableResource.m_IsResourceDirty = true;
	}

	inline void to_json(nlohmann::json& JSON, const sad::ecs::PrimitiveResourceComponent& primitiveResource)
	{
		JSON =
		{
			{ "PrimitiveType", "Cube" },
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::PrimitiveResourceComponent& primitiveResource)
	{
		std::string primitiveType = JSON["PrimitiveType"];

		// Only cube is implemented as a primtive type at the moment
		if (core::StringUtils::Equals(primitiveType, "Cube"))
		{
			PrimitiveResource::Geometry CubeGeometry(CubePoints, sizeof(CubePoints), CubeIndices, CubeIndexCount);
			primitiveResource.m_Primitive = core::CreatePointer<PrimitiveResource>(std::move(CubeGeometry));
			primitiveResource.m_IsResourceDirty = true;
		}
	}

	inline void to_json(nlohmann::json& JSON, const sad::ecs::SpriteResourceComponent& spriteResource)
	{
		JSON =
		{
			{ "SpriteFileName", spriteResource.m_SpriteName },
		};
	}

	inline void from_json(const nlohmann::json& JSON, sad::ecs::SpriteResourceComponent& spriteResource)
	{
		std::string spriteFileName = JSON["SpriteFileName"];

		spriteResource.m_SpriteName = spriteFileName;
		spriteResource.m_IsResourceDirty = true;
	}
}
