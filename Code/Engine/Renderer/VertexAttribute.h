#pragma once

#include <vector>

#include <glad/glad.h>

#include <Core/Assert.h>

namespace sad::rad
{
	/**
	 * @brief Layout for a VertexAttribute that can be bound using glVertexAttribArray() 
	*/
	struct VertexAttribute
	{
		unsigned int Count;
		unsigned int Type;
		unsigned char Normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			}
			
			SAD_ASSERT(false, "Type passed to vertex attribute hasn't been added yet, add it to GetSizeOfType");

			return 0;
		}
	};

	/**
	 * @brief Container for holding the various attributes and current stride for a set of VertexAttributes
	*/
	class VertexAttributeContainer
	{
	public:
		VertexAttributeContainer();

		void AddFloatAttribute(unsigned int count);
		void AddMeshVertexAttribute(unsigned int count);

		std::vector<VertexAttribute> GetVertexAttributes() const { return m_VertexAttributes; }
		unsigned int GetStride() const { return m_Stride; }

	private:
		std::vector<VertexAttribute> m_VertexAttributes;
		unsigned int m_Stride;
	};

}
