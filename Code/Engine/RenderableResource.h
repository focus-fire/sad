#pragma once

#include <entt/entt.hpp>

#include "IResource.h"

namespace sad
{
	class RenderableResource final : public IResource
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

		explicit RenderableResource(const IResource::ResourceData& resourceData, const Geometry& geometry);

		// TODO: Remove this constructor when 3D models can be parsed - the AddResource for RenderableResource will have to change as well.
		//		 When removing this constructor also remove the constructors in Geometry.
		explicit RenderableResource(const IResource::ResourceData& resourceData)
			: IResource(resourceData)
		{ }

		const Geometry GetGeometry() const { return m_Geometry; }

	private:
		Geometry m_Geometry;
	};
}
