#include "sadpch.h"

#include "PlayerController.h"

#include <SDL2/SDL.h>

#include "ECS/Registry.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/PlayerControllerComponent.h"

#include "InputManager.h"
#include "Transform.h"
#include "AudioManager.h"

sad::PlayerController::PlayerController() 
{
	AudioManager::GetInstance().loadAudioFile("footstep", "xd.wav", 10, WAV);
	AudioManager::GetInstance().loadAudioFile("jump", "jump.wav", 10, WAV);
}

sad::PlayerController::~PlayerController() {}

void sad::PlayerController::Update()
{
	if (InputManager::GetInstance().GetAxis(SDL_CONTROLLER_AXIS_LEFTX) != 0.0f);

	// Movespeed, to later get on config instead.
	float movespeed = 0.025f;
	
	sad::ecs::EntityWorld& world = sad::ecs::Registry::GetEntityWorld();
	auto view = world.view<const sad::ecs::PlayerControllerComponent, const sad::ecs::TransformComponent>();
	for (auto [controllerComponent, transformComponent] : view.each())
	{
		bool usingController = false; // bool that disables movement keys when joystick being used to prevent double speed

		// Handles forward/back movement using W and S + controller left joystick.
		if (abs(InputManager::GetInstance().GetAxis(SDL_CONTROLLER_AXIS_LEFTY)) > InputManager::GetInstance().ControllerDeadZone)
		{
			usingController = true;
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -InputManager::GetInstance().GetAxis(SDL_CONTROLLER_AXIS_LEFTY) * movespeed));
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_W) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, 1.0f * movespeed));
			AudioManager::GetInstance().playSFX("footstep");
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_S) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -1.0f * movespeed));
			AudioManager::GetInstance().playSFX("footstep");
		}

		// Handles left/right movement using A and D + controller left joystick.
		if (abs(InputManager::GetInstance().GetAxis(SDL_CONTROLLER_AXIS_LEFTX)) > InputManager::GetInstance().ControllerDeadZone)
		{
			usingController = true;
			transformComponent.m_Transform->Translate(glm::vec3(-InputManager::GetInstance().GetAxis(SDL_CONTROLLER_AXIS_LEFTX) * movespeed, 0.0f, 0.0f));
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_A) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(1.0f * movespeed, 0.0f, 0.0f));
			AudioManager::GetInstance().playSFX("footstep");
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_D) && !usingController)
		{
			transformComponent.m_Transform->Translate(glm::vec3(-1.0f * movespeed, 0.0f, 0.0f));
			AudioManager::GetInstance().playSFX("footstep");
		}

		// Handles up/down flight using SPACE and LSHIFT.
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_SPACE) || InputManager::GetInstance().GetButton(SDL_CONTROLLER_BUTTON_A))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
			AudioManager::GetInstance().playSFX("jump");
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LSHIFT) || InputManager::GetInstance().GetButton(SDL_CONTROLLER_BUTTON_B))
		{
			transformComponent.m_Transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
		}

		// Handles left/right rotation using LEFT and RIGHT arrow keys.
		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_LEFT) || InputManager::GetInstance().GetButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (InputManager::GetInstance().GetKey(SDL_SCANCODE_RIGHT) || InputManager::GetInstance().GetButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
		{
			transformComponent.m_Transform->Rotate(glm::vec3(0.0f, -1.0f, 0.0f));
		}
	}
	
}


