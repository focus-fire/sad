#pragma once

#include <glm/glm.hpp>

#include "Renderer.h"

namespace sad::rad
{
	/**
	 * @brief That little pal that renders things
	*/
	class RenderBuddy
	{
	private:
		RenderBuddy() = delete;
		RenderBuddy(RenderBuddy& lhs) = delete;

	public:
		static void Start() { s_Renderer.Start(); }

		static void Clear() { s_Renderer.Clear(); }

		static void ClearColor(const glm::vec4& color = glm::vec4(0.5f)) { s_Renderer.ClearColor(color); }

		static void BindFrameBuffer() { s_Renderer.BindFrameBuffer(); }

		static void UnbindFrameBuffer() { s_Renderer.UnbindFrameBuffer(); }

		static unsigned int GetFrameBufferTexture() { return s_Renderer.GetFrameBufferTexture(); }

		/**
		 * @brief Draws geometry bound by an index buffer
		 * @param vertexArray VAO wrapper for the geometry in GL
		 * @param indexBuffer IBO wrapper for the geometry in GL
		*/
		static void DrawIndexed(VertexArray* vertexArray, IndexBuffer* indexBuffer) { s_Renderer.DrawIndexed(vertexArray, indexBuffer); }

		/**
		 * @brief Draws lines bound to the passed index array
		 * @param vertexArray VAO wrapper for the line geometry in GL
		 * @param vertexCount Number of vertices required for the bound geometry to render
		*/
		static void DrawLines(VertexArray* vertexArray, unsigned int vertexCount) { s_Renderer.DrawLines(vertexArray, vertexCount); }

		/**
		 * @brief Draws a mesh bound by an index buffer
		 * @param mesh Mesh wrapper for the geometry of a RenderableModel
		*/
		static void DrawMesh(rad::Mesh* mesh) { s_Renderer.DrawMesh(mesh); }

		/**
		 * @brief Renders a flat debug line
		 * @param from Position where the line starts rendering
		 * @param to Position where the line finishes rendering
		 * @param color Solid RGB color to render the line
		*/
		static void DrawDebugLine(glm::vec3 from, glm::vec3 to, glm::vec4 color);

		/**
		 * @brief Renders a solid debug bounding box 
		 * @param min The minimum extent of the box/bound
		 * @param max The maximum extent of the box/bound
		 * @param color Solid RGB color to render the box
		*/
		static void DrawDebugBox(glm::vec3 min, glm::vec3 max, glm::vec4 color);

	private:
		static Renderer s_Renderer;
	};
}
