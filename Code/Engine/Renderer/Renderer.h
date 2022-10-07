#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace sad::rad
{
	class Renderer
	{
	public:
		void Clear(float r, float g, float b, float a) const;
		void Draw(VertexArray* va, IndexBuffer* ib, Shader* shader) const;
	};
}
