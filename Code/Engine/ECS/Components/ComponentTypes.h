#pragma once

/* Non-Serialized */

#include "GuidComponent.h"
#include "RenderableObjectComponent.h"

/* Serialized */

#include "NameComponent.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"
#include "BoundComponent.h"
#include "RenderableResourceComponent.h"
#include "ControllerComponent.h"
#include "PointLightComponent.h"

#define SERIALIZED_COMPONENT_TYPES \
	sad::ecs::NameComponent,\
	sad::ecs::ScriptComponent,\
	sad::ecs::TransformComponent,\
	sad::ecs::BoundComponent,\
	sad::ecs::PrimitiveResourceComponent,\
	sad::ecs::ModelResourceComponent,\
	sad::ecs::PlayerControllerComponent,\
	sad::ecs::EditorControllerComponent,\
	sad::ecs::PointLightComponent
