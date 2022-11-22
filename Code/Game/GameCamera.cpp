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

float sad::GameCamera::walkDirection;
bool sad::GameCamera::walking;
int sad::GameCamera::wasdState;

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

    // Capture player state
    walkDirection = -cameraEulers.y;
    wasdState = 0;
    walking = false;

    // Handles forward/backward movement using W and S
    if (input.GetKey(sad::KeyCode::W))
    {
        wasdState += 1;
    }

    if (input.GetKey(sad::KeyCode::S))
    {
        wasdState += 4;
    }

    // Handles left/right movement using A and D
    if (input.GetKey(sad::KeyCode::A))
    {
        wasdState += 2;
    }

    if (input.GetKey(sad::KeyCode::D))
    {
        wasdState += 8;
    }

    // TODO: Have to move this somwhere else to handle pointer position and visibility
    if (input.GetKey(sad::KeyCode::Escape))
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
        SDL_WarpMouseInWindow(NULL, NULL, NULL);
    }
    // Interpret WASD state
    switch (wasdState)
    {
    case 1:
    case 11:
        // Forwards
        walking = true;
        walkDirection += 90;
        break;
    case 3:
        // Left-forwards
        walking = true;
        walkDirection += 45;
        break;
    case 2:
    case 7:
        // Left
        walking = true;
        break;
    case 6:
        // Left-backwards
        walking = true;
        walkDirection += 315;
        break;
    case 4:
    case 14:
        // Backwards
        walking = true;
        walkDirection += 270;
        break;
    case 12:
        // Right-backwards
        walking = true;
        walkDirection += 225;
        break;
    case 8:
    case 13:
        // Right
        walking = true;
        walkDirection += 180;
        break;
    case 9:
        // Right-forwards
        walking = true;
        walkDirection += 135;
    }

    if (walking)
    {
        // Update the camera position based on direction vector. 0.01f is the constant movespeed
        cameraPosition += 0.01f * glm::vec3{
            glm::cos(glm::radians(walkDirection)),
            0.0,
            glm::sin(glm::radians(walkDirection))
        };
    }

	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	// Get all entities with renderableObject, transform and scripting Components
	auto view = world.view<const ecs::RenderableObjectComponent, const ecs::TransformComponent, const ecs::ScriptComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent, scriptingComponent] : view.each())
	{
		std::string entityScriptClassName = scriptingComponent.m_ClassName;

		// Update the game camera position based on the player object's transform and apply rotation to the player object based on camera's yaw
		if (entityScriptClassName == "Player")
		{
			transformComponent.m_Transform->SetPosition(sad::GameCamera::cameraPosition);
            transformComponent.m_Transform->SetRotation(glm::toQuat(glm::orientate3(cameraForward)));
		}
	}
}
