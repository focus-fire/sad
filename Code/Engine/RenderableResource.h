#pragma once

#include <entt/entt.hpp>

#include "Resource.h"

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

			const float* Normals;

			Geometry()
				: Vertices(nullptr)
				, VertexSize(0)
				, Indices(nullptr)
				, IndexCount(0)
				// , Normals(nullptr)
			{ }

			explicit Geometry(const float* vertices, const unsigned int vertexSize, 
				const unsigned int* indices, const unsigned int indexCount)//,
				// const float* normals)
				: Vertices(vertices)
				, VertexSize(vertexSize)
				, Indices(indices)
				, IndexCount(indexCount)
				// , Normals(normals)
			{ }
		};

		RenderableResource();
		~RenderableResource();

		explicit RenderableResource(const Resource::ResourceData& resourceData, const Geometry& geometry);

		// TODO: Remove this constructor when 3D models can be parsed - the AddResource for RenderableResource will have to change as well.
		explicit RenderableResource(const Resource::ResourceData& resourceData)
			: Resource(resourceData)
		{ }

		const Geometry GetGeometry() const { return m_Geometry; }

	private:
		Geometry m_Geometry;
	};
}
