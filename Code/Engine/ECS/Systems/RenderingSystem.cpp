#include "sadpch.h"

#include "RenderingSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Application.h>
#include <Engine/Renderer/RenderBuddy.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/Camera.h>

void sad::ecs::RenderingSystem::Draw(EntityWorld& world)
{
	RenderIndexables(world);
}

void sad::ecs::RenderingSystem::RenderIndexables(EntityWorld& world)
{
	auto view = world.view<const RenderableObjectComponent, const TransformComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent] : view.each())
	{
		core::Pointer<RenderableObject> renderable = renderableObjectComponent.m_RenderableObject;
		rad::VertexArray* vertexArray = renderable->GetVertexArray();
		rad::IndexBuffer* indexBuffer = renderable->GetIndexBuffer();
		rad::ShaderResource* shader = renderable->GetShader();

		Camera* currentCamera = sad::rad::RenderBuddy::GetCameraInstance();

		glm::mat4 mvpMatrix = currentCamera->GetViewProjectionMatrix() * transformComponent.m_Transform->GetTransformMatrix();

		shader->Bind();

		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));
		rad::RenderBuddy::DrawIndexed(vertexArray, indexBuffer);
	}
}
