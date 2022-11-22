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
		void* Offset;

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

		/**
		 * @brief Appends a float attribute to the container and incremements the stride
		*/
		void AddFloatAttribute(unsigned int count);

		/**
		 * @brief Retrieves a list of the vertex attributes accumulated in the container
		*/
		std::vector<VertexAttribute> GetVertexAttributes() const { return m_VertexAttributes; }

		/**
		 * @brief Retrieves the accumulated stride amongst all vertex attributes
		*/
		unsigned int GetStride() const { return m_Stride; }

	private:
		std::vector<VertexAttribute> m_VertexAttributes;
		unsigned int m_Stride;
	};

	class MeshVertexAttributeContainer
	{
	public:
		MeshVertexAttributeContainer();

		/**
		 * @brief Appends a MeshVertex attribute to the container, these types specify an offset
		*/
		void AddMeshVertexAttribute(unsigned int count, void* offset);

		/**
		 * @brief Retrieves a list of the vertex attributes accumulated in the container
		*/
		std::vector<VertexAttribute> GetVertexAttributes() const { return m_VertexAttributes; }

	private:
		std::vector<VertexAttribute> m_VertexAttributes;
	};

}
