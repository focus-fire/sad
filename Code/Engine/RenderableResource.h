#pragma once

#include <entt/entt.hpp>

namespace sad
{
	class RenderableResource
	{
	public:
		struct Geometry
		{
			const float* Vertices;
			const unsigned int VertexSize;

			const unsigned int* Indices;
			const unsigned int IndexCount;
		};

		explicit RenderableResource(Geometry* geometry);

		Geometry* GetGeometry() const { return m_Geometry; }

	private:
		Geometry* m_Geometry;
	};

	struct RenderableResourceComponent
	{
		RenderableResource* m_RenderableResource;
		bool m_IsResourceDirty;

		RenderableResourceComponent(RenderableResource* renderableResource)
			: m_RenderableResource(renderableResource)
			, m_IsResourceDirty(true)
		{ }
	};
}
