#pragma once

#include <glm/glm.hpp>

#include <Core/Memory.h>

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexAttribute.h"
#include "Renderer/ShaderResource.h"

namespace sad
{
	struct LineRenderer
	{
	public:
		explicit LineRenderer(glm::vec3 from, glm::vec3 to, glm::vec4 color);
		~LineRenderer();

		rad::VertexArray* VertexArray;
		rad::VertexBuffer* VertexBuffer;
		rad::VertexAttributeContainer* VertexAttributeContainer;
		rad::ShaderResource* Shader;
		unsigned int VertexCount;
	};
}
