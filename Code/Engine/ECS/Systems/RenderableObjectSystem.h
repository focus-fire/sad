#pragma once

#include <Engine/ECS/Registry.h>

namespace sad::ecs
{
	/**
	 * @brief Manages the creation of RenderableObjects by detecting when RenderableResources exist on entities within the engine. 
	 *		  Once RenderableObjects are created, each RenderableResource is marked as 'clean'. 
	 * @note If the geometric makeup for a RenderableObject has to be altered RenderableResources should be marked as 'dirty'. 	
	*/
	class RenderableObjectSystem
	{
	public:
		static void Update(EntityWorld& world);
	};
}
