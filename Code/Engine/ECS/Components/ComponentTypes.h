#pragma once

/* Non-Serialized */

#include "NameComponent.h"
#include "GuidComponent.h"
#include "LineRendererComponent.h"
#include "RenderableObjectComponent.h"
#include "RenderableResourceComponent.h"

/* Serialized */

#include "TransformComponent.h"
#include "BoundComponent.h"
#include "RenderableResourceComponent.h"
#include "ControllerComponent.h"

#define SERIALIZED_COMPONENT_TYPES sad::ecs::NameComponent,\
	sad::ecs::TransformComponent,\
	sad::ecs::BoundComponent,\
	sad::ecs::RenderableResourceComponent,\
	sad::ecs::PlayerControllerComponent,\
	sad::ecs::EditorControllerComponent
