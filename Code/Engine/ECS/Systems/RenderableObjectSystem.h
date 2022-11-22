#pragma once

#include <Engine/ECS/Registry.h>

namespace sad::ecs
{
	/**
	 * @brief Manages the creation of various objects by detecting when a ModelResource or a PrimitievResouce exists on entities within the engine.
	 *		  Once either a RenderableModel or a Primitive are created, each ModelResource is marked as 'clean'. 
	 * @note If the geometric makeup for a RenderablePrimitive has to be altered RenderableResources should be marked as 'dirty'. 	
	*/
	class RenderableObjectSystem
	{
	public:
		static void Update(EntityWorld& world);

	private:
		/**
		 * @brief Turns ModelResources into RenderableModels 
		*/
		static void CreateRenderableModels(EntityWorld& world);

		/**
		 * @brief Turns PrimitiveResources into RenderablePrimitives 
		*/
		static void CreateRenderablePrimitives(EntityWorld& world);
	};
}
