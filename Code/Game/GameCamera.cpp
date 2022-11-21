#include "sadpch.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "backends/imgui_impl_glfw.h"

#include "Engine/Application.h"
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/ScriptComponent.h>

#include "GameCamera.h"


#define GLM_ENABLE_EXPERIMENTAL


void sad::GameCamera::Update()
{
	sad::GameCamera::CurrentCameraState();
	InputManager& input = InputManager::GetInstance();

	double mouse_x, mouse_y;
	mouse_x = input.GetMousePosition().x;
	mouse_y = input.GetMousePosition().y;

	ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
	input.SetMousePosition(800.0, 450.0);

	float delta_x{ static_cast<float>(mouse_x - 800.0) };
	cameraEulers.y -= delta_x * 0.001f;

	float delta_y{ static_cast<float>(mouse_y - 450.0) };
	//sad::Camera::cameraEulers.x = std::max(std::min(sad::Camera::cameraEulers.x + delta_y, 180.0f), 0.0f);
	cameraEulers.x = std::max(std::min(cameraEulers.x + delta_y * 0.05f, 45.0f), -45.0f);

}

void sad::GameCamera::CurrentCameraState()
{
	InputManager& input = InputManager::GetInstance();


	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	auto view = world.view<const ecs::RenderableObjectComponent, const ecs::TransformComponent, const ecs::ScriptComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent, scriptingComponent] : view.each())
	{
		std::string entityScriptClassName = scriptingComponent.m_ClassName;
		if (entityScriptClassName == "Player")
		{
			sad::GameCamera::cameraPosition = transformComponent.m_Transform->GetPosition() + glm::vec3(0.0f, 0.0f, -5.0f);
			transformComponent.m_Transform->SetRotation(glm::toQuat(glm::orientate3(glm::vec3(0, 0, cameraEulers.y * 0.1f))));
		}
	}
}
