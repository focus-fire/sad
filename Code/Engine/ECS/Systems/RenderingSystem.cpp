#include "sadpch.h"

#include "RenderingSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Application.h>
#include <Engine/Renderer/RenderBuddy.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/LineRendererComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>

void sad::ecs::RenderingSystem::Draw(EntityWorld& world)
{
	RenderIndexables(world);

	RenderLines(world);
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

		// TODO: Retrieve the view projection matrix from the Camera 
		glm::mat4 mvpMatrix = sad::Application::GetViewProjectionMatrix() * transformComponent.m_Transform->GetTransformMatrix();

		shader->Bind();

		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));
		rad::RenderBuddy::DrawIndexed(vertexArray, indexBuffer);
	}
}

void sad::ecs::RenderingSystem::RenderLines(EntityWorld& world)
{
	auto view = world.view<const LineRendererComponent>();
	for (auto [entity, lineRendererComponent] : view.each())
	{
		core::Pointer<LineRenderer> lineRenderer = lineRendererComponent.m_LineRenderer;
		rad::ShaderResource* shader = lineRenderer->GetShader();

		// TODO: Retrieve the view projection matrix from the Camera
		glm::mat4 vpMatrix = sad::Application::GetViewProjectionMatrix();

		shader->Bind();
		shader->SetUniformMatrix4fv("u_VpMatrix", glm::value_ptr(vpMatrix));
		rad::RenderBuddy::DrawLines(lineRenderer->GetVertexArray(), lineRenderer->GetVertexCount());
	}
}