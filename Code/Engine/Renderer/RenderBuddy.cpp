#include "sadpch.h"

#include "RenderBuddy.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Engine/Application.h>
#include <Engine/LineRenderer.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Components/LineRendererComponent.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

sad::rad::Renderer sad::rad::RenderBuddy::s_Renderer = sad::rad::Renderer();

void sad::rad::RenderBuddy::DrawDebugLine(glm::vec3 from, glm::vec3 to, glm::vec3 color)
{
	Pointer<LineRenderer> lineRenderer = CreatePointer<LineRenderer>(from, to, color);
	ecs::Entity lineEntity = ecs::Entity();
	lineEntity.AddComponent<ecs::LineRendererComponent>({ lineRenderer });
}

void sad::rad::RenderBuddy::DrawDebugBox()
{

}

void sad::rad::RenderBuddy::DrawDebugSphere()
{

}
