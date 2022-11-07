#pragma once
#include "LineRendererComponent.h"
#include "ControllerComponent.h"
#include "RenderableObjectComponent.h"
#include "RenderableResourceComponent.h"
#include "TransformComponent.h"

#define COMPONENT_TYPES \
sad::ecs::TransformComponent, \
sad::ecs::RenderableResourceComponent, \
sad::ecs::PlayerControllerComponent, \
sad::ecs::EditorControllerComponent