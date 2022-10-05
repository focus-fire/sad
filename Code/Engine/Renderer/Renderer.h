#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

namespace sad::rad
{
	class Renderer
	{
	public:
		void Clear(float r, float g, float b, float a) const;
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	};
}
