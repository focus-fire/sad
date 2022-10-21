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
	// Movespeed, to later get on config instead.
	float movespeed = 0.025f;
	
	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	auto view = world.view<const sad::ecs::PlayerControllerComponent, const sad::ecs::TransformComponent>();
	for (auto [controllerComponent, transformComponent] : view.each())
	{
		// Handles forward/back movement using W and S.
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_W))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, 1.0f * movespeed));
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_S))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -1.0f * movespeed));
		}

		// Handles left/right movement using A and D.
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_A))
		{
			transformComponent.m_Transform->Translate(glm::vec3(1.0f * movespeed, 0.0f, 0.0f));
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_D))
		{
			transformComponent.m_Transform->Translate(glm::vec3(-1.0f * movespeed, 0.0f, 0.0f));
		}

		// Handles up/down flight using SPACE and LSHIFT.
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_SPACE))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LSHIFT))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
		}

		// Handles left/right rotation using LEFT and RIGHT arrow keys.
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LEFT))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_RIGHT))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, -1.0f, 0.0f));
		}
	}
	
}


