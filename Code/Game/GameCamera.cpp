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
	// Get current camera state (position) from Player object
	sad::GameCamera::CurrentCameraState();

	// Get the input manager instance to track mouse position
	// Probably don't need to grab the instance every update
	InputManager& input = InputManager::GetInstance();

	double mouse_x, mouse_y;
	mouse_x = input.GetMousePosition().x;
	mouse_y = input.GetMousePosition().y;

	ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
	input.SetMousePosition(800.0, 450.0);

	// TODO: Remove tempSensitivityHandle declaration and calls below after mouse pointer handling is in place
	// Temporary sensitivity constant for assisting with troubleshooting and testing game
	float tempSensitivityHandle = 0.005f;

	// Calculate and update camera yaw (horizontal rotation) for the camera and keep rotation between 0 <=> 360
	float delta_x{ static_cast<float>(mouse_x - 800.0) };
	cameraEulers.y -= delta_x * 0.001f;
	if (cameraEulers.y <= 0)
	{
		cameraEulers.y = 360.0f;
	}
	else if (cameraEulers.y >= 360.0f)
	{
		cameraEulers.y = 0.0f;
	}

	// Calculate and update camera pitch (vertical rotation), keeps it between 90 <=> -90
	float delta_y{ static_cast<float>(mouse_y - 450.0) };
	cameraEulers.x = std::max(std::min(cameraEulers.x + delta_y * 0.03f * tempSensitivityHandle, 45.0f), -45.0f);

}

void sad::GameCamera::CurrentCameraState()
{
	// TODO: Probably don't need to grab the instance every update
	InputManager& input = InputManager::GetInstance();

	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	// Get all entities with renderableObject, transform and scripting Components
	auto view = world.view<const ecs::RenderableObjectComponent, const ecs::TransformComponent, const ecs::ScriptComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent, scriptingComponent] : view.each())
	{
		std::string entityScriptClassName = scriptingComponent.m_ClassName;

		// Update the game camera position based on the player object's transform and apply rotation to the player object based on camera's yaw
		if (entityScriptClassName == "Player")
		{
			sad::GameCamera::cameraPosition = transformComponent.m_Transform->GetPosition();
			transformComponent.m_Transform->SetRotation(glm::toQuat(glm::orientate3(glm::vec3(0, 0, cameraEulers.y * 0.1f))));
		}
	}
}
