#pragma once

#include <entt/entt.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <Engine/Renderer/Mesh.h>
#include <Engine/Renderer/ShaderResource.h>

#include "Resource.h"

namespace sad
{
	/**
	 * @brief Holds resource data for models 
	*/
	class ModelResource final : public Resource
	{
	public:
		ModelResource();
		~ModelResource();

		explicit ModelResource(const Resource::ResourceData& resourceData);
	};

	/**
	 * @brief Holds resource data for primitives (ie: Cubes, Spheres, etc.) that have vertices hard-coded in the engine 
	*/
	class PrimitiveResource
	{
	public:
		struct Geometry
		{
			const float* Vertices;
			const unsigned int VertexSize;

			const unsigned int* Indices;
			const unsigned int IndexCount;

			Geometry()
				: Vertices(nullptr)
				, VertexSize(0)
				, Indices(nullptr)
				, IndexCount(0)
			{ }

			explicit Geometry(const float* vertices, const unsigned int vertexSize, const unsigned int* indices, const unsigned int indexCount)
				: Vertices(vertices)
				, VertexSize(vertexSize)
				, Indices(indices)
				, IndexCount(indexCount)
			{ }
		};

		explicit PrimitiveResource(const Geometry& geometry);

		const Geometry GetGeometry() const { return m_Geometry; }

		private:
			Geometry m_Geometry;
	};
}
