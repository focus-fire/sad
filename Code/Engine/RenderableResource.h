#pragma once

#include <entt/entt.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <Engine/Renderer/Mesh.h>
#include <Engine/Renderer/ShaderResource.h>

#include "Resource.h"
#include "Model.h"

namespace sad
{
	class RenderableResource final : public Resource
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

		RenderableResource();
		~RenderableResource();

		explicit RenderableResource(const Resource::ResourceData& resourceData, const Geometry& geometry);
		explicit RenderableResource(const Resource::ResourceData& resourceData);

		const Geometry GetGeometry() const { return m_Geometry; }

	private:
		Geometry m_Geometry;
	};
}
