#pragma once

/* Non-Serialized */

#include "RenderableObjectComponent.h"

/* Serialized */

#include "GuidComponent.h"
#include "NameComponent.h"
#include "ScriptComponent.h"
#include "TransformComponent.h"
#include "BoundComponent.h"
#include "RenderableResourceComponent.h"
#include "ControllerComponent.h"
#include "PointLightComponent.h"

#define SERIALIZED_COMPONENT_TYPES \
	sad::ecs::GuidComponent,\
	sad::ecs::NameComponent,\
	sad::ecs::ScriptComponent,\
	sad::ecs::TransformComponent,\
	sad::ecs::BoundComponent,\
	sad::ecs::PrimitiveResourceComponent,\
	sad::ecs::ModelResourceComponent,\
	sad::ecs::PlayerControllerComponent,\
	sad::ecs::EditorControllerComponent,\
	sad::ecs::PointLightComponent
