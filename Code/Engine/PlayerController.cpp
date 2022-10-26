#include "sadpch.h"

#include "PlayerController.h"

#include <SDL2/SDL.h>

#include "ECS/Registry.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/PlayerControllerComponent.h"

#include "InputManager.h"
#include "Transform.h"

sad::PlayerController::PlayerController() {}

sad::PlayerController::~PlayerController() {}

void sad::PlayerController::Update()
{
	
	InputManager& input = InputManager::GetInstance();

	// Movespeed, to later get on config instead.
	float movespeed = 0.025f;
	
	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	auto view = world.view<const sad::ecs::PlayerControllerComponent, const sad::ecs::TransformComponent>();
	for (auto [controllerComponent, transformComponent] : view.each())
	{
		bool usingController = false; // bool that disables movement keys when joystick being used to prevent double speed

		// Handles forward/back movement using W and S + controller left joystick.
		if (abs(input.GetAxis(SDL_CONTROLLER_AXIS_LEFTY)) > input.ControllerDeadZone)
		{
			usingController = true;
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -input.GetAxis(SDL_CONTROLLER_AXIS_LEFTY) * movespeed));
		}

		if (input.GetKey(SDL_SCANCODE_W) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, 1.0f * movespeed));
		}

		if (input.GetKey(SDL_SCANCODE_S) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -1.0f * movespeed));
		}

		// Handles left/right movement using A and D + controller left joystick.
		if (abs(input.GetAxis(SDL_CONTROLLER_AXIS_LEFTX)) > input.ControllerDeadZone)
		{
			usingController = true;
			transformComponent.m_Transform->Translate(glm::vec3(-input.GetAxis(SDL_CONTROLLER_AXIS_LEFTX) * movespeed, 0.0f, 0.0f));
		}

		if (input.GetKey(SDL_SCANCODE_A) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(1.0f * movespeed, 0.0f, 0.0f));
		}

		if (input.GetKey(SDL_SCANCODE_D) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(-1.0f * movespeed, 0.0f, 0.0f));
		}

		// Handles up/down flight using SPACE and LSHIFT.
		if (input.GetKey(SDL_SCANCODE_SPACE) || input.GetButton(SDL_CONTROLLER_BUTTON_A))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
		}

		if (input.GetKey(SDL_SCANCODE_LSHIFT) || input.GetButton(SDL_CONTROLLER_BUTTON_B))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
		}

		// Handles left/right rotation using LEFT and RIGHT arrow keys.
		if (input.GetKey(SDL_SCANCODE_LEFT) || input.GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (input.GetKey(SDL_SCANCODE_RIGHT) || input.GetButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, -1.0f, 0.0f));
		}


		// Test mouse position
		if (input.GetMouseButtonPressed(SDL_BUTTON_LEFT)) 
		{
			std::string mousePosition = "Mouse Pos: (";
			mousePosition += std::to_string(input.GetMousePosition().x);
			mousePosition += ", ";
			mousePosition += std::to_string(input.GetMousePosition().y);
			mousePosition += ")";

			core::Log(ELogType::Info, mousePosition.c_str());
		}
	}
	
}


