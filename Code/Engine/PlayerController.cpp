#include "sadpch.h"
#include <SDL2/SDL.h>
#include "InputManager.h"
#include "Transform.h"
#include "PlayerController.h"

sad::PlayerController::~PlayerController() {}

void sad::PlayerController::Update()
{
	float movespeed = 0.025f;

	// forward/back
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_W))
	{
		transform->Translate(glm::vec3(0.0f, 0.0f, 1.0f * movespeed));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_S))
	{
		transform->Translate(glm::vec3(0.0f, 0.0f, -1.0f * movespeed));
	}

	// left/right
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_A))
	{
		transform->Translate(glm::vec3(1.0f * movespeed, 0.0f, 0.0f));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_D))
	{
		transform->Translate(glm::vec3(-1.0f * movespeed, 0.0f, 0.0f));
	}

	// up/down
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_SPACE))
	{
		transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LSHIFT))
	{
		transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
	}

	// rotate left/right
	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LEFT))
	{
		transform->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (InputManager::GetInstance().GetKey(SDL_SCANCODE_RIGHT))
	{
		transform->Rotate(glm::vec3(0.0f, -1.0f, 0.0f));
	}
}


