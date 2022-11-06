#pragma once

#include <glm/glm.hpp>

#include <Core/Memory.h>

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/VertexAttribute.h"
#include "Renderer/Shader.h"

namespace sad
{
	class LineRenderer
	{
	public:
		explicit LineRenderer(glm::vec3 from, glm::vec3 to, glm::vec3 color);
		~LineRenderer();

		rad::VertexArray* GetVertexArray() const { return m_VertexArray; }
		rad::Shader* GetShader() const { return m_Shader; }
		unsigned int GetVertexCount() const { return m_VertexCount; }

	private:
		rad::VertexArray* m_VertexArray;
		rad::VertexBuffer* m_VertexBuffer;
		rad::VertexAttributeContainer* m_VertexAttributeContainer;
		rad::Shader* m_Shader;
		unsigned int m_VertexCount;
	};
}
