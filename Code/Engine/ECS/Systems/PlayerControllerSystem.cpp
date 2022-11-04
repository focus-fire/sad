#include "sadpch.h"

#include <SDL2/SDL.h>

#include <Engine/InputManager.h>
#include <Engine/Transform.h>

#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Systems/PlayerControllerSystem.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/PlayerControllerComponent.h>
#include <Engine/ECS/Components/EditorControllerComponent.h>

sad::PlayerControllerSystem::PlayerControllerSystem() {}
sad::PlayerControllerSystem::~PlayerControllerSystem() {}

void sad::PlayerControllerSystem::PlayerControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed)
{
	if (input.HasControllerConnected())
	{
		// Handles vertical left joystick input for forward/back movement
		if (abs(input.GetLeftAxis("Vertical")) > input.ControllerDeadZone)
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -input.GetLeftAxis("Vertical") * movespeed));
		}

		// Handles horizontal left joystick input for left/right movement
		if (abs(input.GetLeftAxis("Horizontal")) > input.ControllerDeadZone)
		{
			transformComponent.m_Transform->Translate(glm::vec3(-input.GetLeftAxis("Horizontal") * movespeed, 0.0f, 0.0f));
		}

		// Handles rotation with left/right shoulder buttons
		if (input.GetButton(sad::ControllerButton::L1))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, 1.0f * 0.25f, 0.0f));
		}

		if (input.GetButton(sad::ControllerButton::R1))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, -1.0f * 0.25f, 0.0f));
		}
	}
	else
	{
		// Handles forward/backward movement using W and S
		if (input.GetKey(sad::KeyCode::W))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, 1.0f * movespeed));
		}

		if (input.GetKey(sad::KeyCode::S))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -1.0f * movespeed));
		}

		// Handles left/right movement using A and D
		if (input.GetKey(sad::KeyCode::A))
		{
			transformComponent.m_Transform->Translate(glm::vec3(1.0f * movespeed, 0.0f, 0.0f));
		}

		if (input.GetKey(sad::KeyCode::D))
		{
			transformComponent.m_Transform->Translate(glm::vec3(-1.0f * movespeed, 0.0f, 0.0f));
		}

		// Handles left/right rotation using LEFT and RIGHT arrow keys.
		if (input.GetKey(sad::KeyCode::LeftArrow))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, 1.0f * 0.25f, 0.0f));
		}

		if (input.GetKey(sad::KeyCode::RightArrow))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, -1.0f * 0.25f, 0.0f));
		}
	}

}

void sad::PlayerControllerSystem::EditorControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed)
{
	if (input.HasControllerConnected())
	{
		// Handles up/down flight using south and east buttons
		if (input.GetButton(sad::ControllerButton::South))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
		}

		if (input.GetButton(sad::ControllerButton::East))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
		}
	}
	else
	{
		// Handles up/down flight using space and left shift.
		if (input.GetKey(sad::KeyCode::Space) || input.GetButton(sad::ControllerButton::South))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
		}

		if (input.GetKey(sad::KeyCode::LShift) || input.GetButton(sad::ControllerButton::East))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
		}
	}
	
}

void sad::PlayerControllerSystem::Update()
{
	InputManager& input = InputManager::GetInstance();

	// Test mouse position
	if (input.GetMouseButtonPressed(SDL_BUTTON_MIDDLE))
	{
		std::string mousePosition = "Mouse Pos: (";
		mousePosition += std::to_string(input.GetMousePosition().x);
		mousePosition += ", ";
		mousePosition += std::to_string(input.GetMousePosition().y);
		mousePosition += ")";

		core::Log(ELogType::Info, mousePosition.c_str());
	}

	float movespeed = 0.005f;

	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	auto playerView = world.view<const sad::ecs::PlayerControllerComponent, const sad::ecs::TransformComponent>();
	auto editorView = world.view<const sad::ecs::EditorControllerComponent, const sad::ecs::TransformComponent>();
	
	for (auto [controllerComponent, transformComponent] : playerView.each())
	{
		PlayerControls(input, transformComponent, movespeed);
	}

	for (auto [controllerComponent, transformComponent] : editorView.each())
	{
		PlayerControls(input, transformComponent, movespeed);
		EditorControls(input, transformComponent, movespeed);
	}
	
}

