#include "sadpch.h"

#include "RenderingSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Application.h>
#include <Engine/Renderer/RenderBuddy.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>

void sad::ecs::RenderingSystem::Draw(EntityWorld& world)
{
	RenderPrimitives(world);

	RenderModels(world);
}

void sad::ecs::RenderingSystem::RenderPrimitives(EntityWorld& world)
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
		shader->Unbind();
	}
}

void sad::ecs::RenderingSystem::RenderModels(EntityWorld& world)
{
	auto view = world.view<const RenderableModelComponent, const TransformComponent>();
	for (auto [entity, renderableModelComponent, transformComponent] : view.each())
	{
		Model model = renderableModelComponent.m_RenderableModel;
		Transform* transform = transformComponent.m_Transform.get();

		// TODO: Retrieve the view projection matrix from the Camera 
		glm::mat4 modelMatrix = transform->GetTransformMatrix();
		glm::mat3 normalMatrix = transform->GetNormalMatrix();
		glm::mat4 modelViewMatrix = sad::Application::GetViewMatrix() * modelMatrix;
		glm::mat4 mvpMatrix = sad::Application::GetViewProjectionMatrix() * modelMatrix;

		// Set shader uniforms
		rad::ShaderResource* shader = model.GetShader();
		shader->Bind();
		shader->SetUniformMatrix3fv("u_NormalMatrix", glm::value_ptr(normalMatrix));
		shader->SetUniformMatrix4fv("u_ModelViewMatrix", glm::value_ptr(modelViewMatrix));
		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));

		std::vector<core::Pointer<rad::Mesh>> modelMeshes = model.GetMeshes();
		for (unsigned int i = 0; i < modelMeshes.size(); i++)
		{
			rad::Mesh* currentMesh = modelMeshes[i].get();

			rad::RenderBuddy::DrawMesh(currentMesh);
		}
		shader->Unbind();
	}
}
