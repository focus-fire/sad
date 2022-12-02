#include "sadpch.h"

#include "RenderingSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Engine/Application.h>
#include <Engine/RenderableModel.h>

#include <Engine/Renderer/RenderBuddy.h>
#include <Engine/ECS/Components/ComponentTypes.h>
#include <Engine/Camera.h>

void sad::ecs::RenderingSystem::Draw(EntityWorld& world)
{
	RenderPrimitives(world);

	RenderModels(world);
}

void sad::ecs::RenderingSystem::RenderPrimitives(EntityWorld& world)
{
	auto view = world.view<const RenderablePrimitiveComponent, const TransformComponent>();
	for (auto [handle, primitiveComponent, transformComponent] : view.each())
	{
		core::Pointer<RenderablePrimitive> renderable = primitiveComponent.m_Primitive;
		rad::VertexArray* vertexArray = renderable->GetVertexArray();
		rad::IndexBuffer* indexBuffer = renderable->GetIndexBuffer();
		rad::ShaderResource* shader = renderable->GetShader();

		Camera* currentCamera = sad::rad::RenderBuddy::GetCameraInstance();

		glm::mat4 mvpMatrix = currentCamera->GetViewProjectionMatrix() * transformComponent.m_Transform->GetTransformMatrix();

		shader->Bind();
		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));

		rad::RenderBuddy::DrawIndexed(vertexArray, indexBuffer);

		shader->Unbind();
	}
}

void sad::ecs::RenderingSystem::RenderModels(EntityWorld& world)
{
	auto view = world.view<const RenderableModelComponent, const TransformComponent>();
	for (auto [handle, modelComponent, transformComponent] : view.each())
	{
		RenderableModel model = modelComponent.m_RenderableModel;
		Transform* transform = transformComponent.m_Transform.get();

		Camera* currentCamera = sad::rad::RenderBuddy::GetCameraInstance();

		glm::mat4 modelMatrix = transform->GetTransformMatrix();
		glm::mat3 normalMatrix = transform->GetNormalMatrix();
		glm::mat4 modelViewMatrix = currentCamera->GetViewMatrix() * modelMatrix;
		glm::mat4 mvpMatrix = currentCamera->GetViewProjectionMatrix() * modelMatrix;

		// Set shader uniforms
		rad::ShaderResource* shader = model.GetShader();
		shader->Bind();

		// Matrices
		shader->SetUniformMatrix3fv("u_NormalMatrix", glm::value_ptr(normalMatrix));
		shader->SetUniformMatrix4fv("u_ModelViewMatrix", glm::value_ptr(modelViewMatrix));
		shader->SetUniformMatrix4fv("u_MvpMatrix", glm::value_ptr(mvpMatrix));

		glm::vec3 lightPosition = glm::vec3(0.0f, 42.0f, -3.0f);
		shader->SetUniform3fv("u_LightPosition", glm::value_ptr(lightPosition));

		//glm::vec3 cameraPosition = glm::vec3(0.5f, 2.5f, -3.0f);
		shader->SetUniform3fv("u_ViewPosition", glm::value_ptr(currentCamera->cameraPosition));

		std::vector<core::Pointer<rad::Mesh>> modelMeshes = model.GetMeshes();
		for (unsigned int i = 0; i < modelMeshes.size(); i++)
		{
			rad::Mesh* currentMesh = modelMeshes[i].get();
			rad::MeshColor meshColors = currentMesh->Colors;

			shader->SetUniform4f("u_Model.Ambient", meshColors.Ambient.r, meshColors.Ambient.g, meshColors.Ambient.b, meshColors.Ambient.a);
			shader->SetUniform4f("u_Model.Diffuse", meshColors.Diffuse.r, meshColors.Diffuse.g, meshColors.Diffuse.b, meshColors.Diffuse.a);
			shader->SetUniform4f("u_Model.Specular", meshColors.Specular.r, meshColors.Specular.g, meshColors.Specular.b, meshColors.Specular.a);
			shader->SetUniform4f("u_Model.Emissive", meshColors.Emissive.r, meshColors.Emissive.g, meshColors.Emissive.b, meshColors.Emissive.a);

			rad::RenderBuddy::DrawMesh(currentMesh);
		}

		shader->Unbind();
	}
}
