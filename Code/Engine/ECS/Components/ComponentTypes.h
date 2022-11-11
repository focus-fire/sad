#pragma once

#include "TransformComponent.h"
#include "BoundComponent.h"
#include "RenderableResourceComponent.h"
#include "ControllerComponent.h"

#define SERIALIZED_COMPONENT_TYPES sad::ecs::TransformComponent,\
	sad::ecs::BoundComponent,\
	sad::ecs::RenderableResourceComponent,\
	sad::ecs::PlayerControllerComponent,\
	sad::ecs::EditorControllerComponent