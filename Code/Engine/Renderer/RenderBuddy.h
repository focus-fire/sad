#pragma once

#include <glm/glm.hpp>

namespace sad::rad
{
	/**
	 * @brief Might as well render peepos, instead it performs debug rendering
	*/
	class RenderBuddy
	{
	private:
		RenderBuddy() = delete;
		RenderBuddy(RenderBuddy& lhs) = delete;

	public:
		/**
		 * @brief Renders a flat debug line
		 * @param from Position where the line starts rendering
		 * @param to Position where the line finishes rendering
		 * @param color Solid color to render the line
		*/
		static void DrawLine(glm::vec3 from, glm::vec3 to, glm::vec3 color);

		static void DrawBox();

		static void DrawSphere();
	};
}
