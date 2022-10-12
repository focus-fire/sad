#include "sadpch.h"

#include <SDL2/SDL.h>

#include "InputManager.h"
#include "Transform.h"
#include "PlayerController.h"

sad::PlayerController::~PlayerController() {}

void sad::PlayerController::Update()
{
	// Movespeed, to later get on config instead.
	float movespeed = 0.025f;

	// Handles forward/back movement using W and S.
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_W))
	{
		m_Transform->Translate(glm::vec3(0.0f, 0.0f, 1.0f * movespeed));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_S))
	{
		m_Transform->Translate(glm::vec3(0.0f, 0.0f, -1.0f * movespeed));
	}

	// Handles left/right movement using A and D.
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_A))
	{
		m_Transform->Translate(glm::vec3(1.0f * movespeed, 0.0f, 0.0f));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_D))
	{
		m_Transform->Translate(glm::vec3(-1.0f * movespeed, 0.0f, 0.0f));
	}

	// Handles up/down flight using SPACE and LSHIFT.
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_SPACE))
	{
		m_Transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LSHIFT))
	{
		m_Transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
	}

	// Handles left/right rotation using LEFT and RIGHT arrow keys.
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LEFT))
	{
		m_Transform->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_RIGHT))
	{
		m_Transform->Rotate(glm::vec3(0.0f, -1.0f, 0.0f));
	}
}


