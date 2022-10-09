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

		explicit RenderableResource(const Geometry& geometry);

		const Geometry GetGeometry() const { return m_Geometry; }

	private:
		Geometry m_Geometry;
	};
}
