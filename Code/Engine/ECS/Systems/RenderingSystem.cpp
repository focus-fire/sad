#include "sadpch.h"

#include "RenderingSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Application.h>
#include <Engine/Renderer/RenderBuddy.h>
#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/LineRendererComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>

void sad::ecs::RenderingSystem::Draw()
{
	EntityWorld& world = Registry::GetEntityWorld();

	auto view = world.view<const RenderableObjectComponent, const TransformComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent] : view.each())
	{
		RenderableObjectComponent renderable = renderableObjectComponent;

		sad::rad::VertexArray* vertexArray = renderable.m_RenderableObject->GetVertexArray();
		sad::rad::IndexBuffer* indexBuffer = renderable.m_RenderableObject->GetIndexBuffer();
		sad::rad::Shader* shader = renderable.m_RenderableObject->GetShader();

		// TODO: Retrieve the view projection matrix from the Camera 
		glm::mat4 mvpMatrix = sad::Application::GetViewProjectionMatrix() * transformComponent.m_Transform->GetTransformMatrix();

		shader->Bind();
		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));
		rad::RenderBuddy::DrawIndexed(vertexArray, indexBuffer);
	}

	auto lineView = world.view<const LineRendererComponent>();
	for (auto [entity, lineRendererComponent] : lineView.each())
	{
		Pointer<LineRenderer> lineRenderer = lineRendererComponent.m_LineRenderer;

		lineRenderer->m_Shader->Bind();
		lineRenderer->m_Shader->SetUniformMatrix4fv("u_VpMatrix", glm::value_ptr(sad::Application::GetViewProjectionMatrix()));

		rad::RenderBuddy::DrawLines(lineRenderer->m_VertexArray, lineRenderer->m_VertexCount);
	}
}