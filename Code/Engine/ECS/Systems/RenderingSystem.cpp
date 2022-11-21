#include "sadpch.h"

#include "RenderingSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Application.h>
#include <Engine/Renderer/RenderBuddy.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Game/GameCamera.h>
#include <Game/EditorCamera.h>

void sad::ecs::RenderingSystem::Draw(EntityWorld& world)
{
	RenderIndexables(world);
}

void sad::ecs::RenderingSystem::RenderIndexables(EntityWorld& world)
{
	//GameCamera camera = GameCamera();
	auto view = world.view<const RenderableObjectComponent, const TransformComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent] : view.each())
	{
		core::Pointer<RenderableObject> renderable = renderableObjectComponent.m_RenderableObject;
		rad::VertexArray* vertexArray = renderable->GetVertexArray();
		rad::IndexBuffer* indexBuffer = renderable->GetIndexBuffer();
		rad::ShaderResource* shader = renderable->GetShader();

		if (sad::GameCamera::isActive)
		{
			glm::mat4 mvpMatrix = sad::GameCamera::GetViewProjectionMatrix() * transformComponent.m_Transform->GetTransformMatrix();
			shader->Bind();

			shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));
			rad::RenderBuddy::DrawIndexed(vertexArray, indexBuffer);
		}
		if (sad::EditorCamera::isActive)
		{
			glm::mat4 mvpMatrix = sad::EditorCamera::GetViewProjectionMatrix() * transformComponent.m_Transform->GetTransformMatrix();
			shader->Bind();

			shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));
			rad::RenderBuddy::DrawIndexed(vertexArray, indexBuffer);
		}
		/*glm::mat4 mvpMatrix = sad::GameCamera::GetViewProjectionMatrix() * transformComponent.m_Transform->GetTransformMatrix();

		shader->Bind();

		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));
		rad::RenderBuddy::DrawIndexed(vertexArray, indexBuffer);*/
	}
}
