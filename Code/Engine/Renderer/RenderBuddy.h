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

		static void ClearColor(const glm::vec4& color = glm::vec4(0.5f)) { s_Renderer.ClearColor(color); }

		static void BindFrameBuffer() { s_Renderer.BindFrameBuffer(); }

		static void UnbindFrameBuffer() { s_Renderer.UnbindFrameBuffer(); }

		static unsigned int GetFrameBufferTexture() { return s_Renderer.GetFrameBufferTexture(); }

		static void DrawIndexed(VertexArray* vertexArray, IndexBuffer* indexBuffer) { s_Renderer.DrawIndexed(vertexArray, indexBuffer); }

		static void DrawLines(const VertexArray& vertexArray, unsigned int vertexCount) { s_Renderer.DrawLines(vertexArray, vertexCount); }

		/**
		 * @brief Renders a flat debug line
		 * @param from Position where the line starts rendering
		 * @param to Position where the line finishes rendering
		 * @param color Solid color to render the line
		*/
		static void DrawDebugLine(glm::vec3 from, glm::vec3 to, glm::vec3 color);

		static void DrawDebugBox();

		static void DrawDebugSphere();

	private:
		static Renderer s_Renderer;
	};
}
