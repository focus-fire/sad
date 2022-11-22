#pragma once

#include <Engine/ECS/Registry.h>

namespace sad::ecs
{
	/**
	 * @brief Performs rendering/drawing on various types of entities within the engine 
	*/
	class RenderingSystem
	{
	public:
		static void Draw(EntityWorld& world);

	private:
		/**
		 * @brief Performs drawing on entities containing a RenderablePrimitive
		*/
		static void RenderPrimitives(EntityWorld& world);

		/**
		 * @brief Performs drawing on entities containing a RenderableModel
		*/
		static void RenderModels(EntityWorld& world);
	};
}
