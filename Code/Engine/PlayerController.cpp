#include "sadpch.h"

#include "PlayerController.h"

#include <SDL2/SDL.h>

#include "ECS/Registry.h"
#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/PlayerControllerComponent.h"
#include "ECS/Components/EditorControllerComponent.h"

#include "InputManager.h"
#include "Transform.h"

sad::PlayerController::PlayerController() {}
sad::PlayerController::~PlayerController() {}

void sad::PlayerController::PlayerControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed)
{
	bool usingController = false; // bool that disables movement keys when joystick being used to prevent double speed

	// Handles forward/back movement using W and S + controller left joystick.
	if (abs(input.GetLeftAxis("Vertical")) > input.ControllerDeadZone)
	{
		usingController = true;
		transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -input.GetLeftAxis("Vertical") * movespeed));
	}

	if (input.GetKey(sad::KeyCode::W) && !usingController)
	{
		transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, 1.0f * movespeed));
	}

	if (input.GetKey(sad::KeyCode::S) && !usingController)
	{
		transformComponent.m_Transform->Translate(glm::vec3(0.0f, 0.0f, -1.0f * movespeed));
	}

	// Handles left/right movement using A and D + controller left joystick.
	if (abs(input.GetLeftAxis("Horizontal")) > input.ControllerDeadZone)
	{
		usingController = true;
		transformComponent.m_Transform->Translate(glm::vec3(-input.GetLeftAxis("Horizontal") * movespeed, 0.0f, 0.0f));
	}

	if (input.GetKey(sad::KeyCode::A) && !usingController)
	{
		transformComponent.m_Transform->Translate(glm::vec3(1.0f * movespeed, 0.0f, 0.0f));
	}

	if (input.GetKey(sad::KeyCode::D) && !usingController)
	{
		transformComponent.m_Transform->Translate(glm::vec3(-1.0f * movespeed, 0.0f, 0.0f));
	}

	// Handles left/right rotation using LEFT and RIGHT arrow keys.
	if (input.GetKey(sad::KeyCode::LeftArrow) || input.GetButton(sad::ControllerButton::L1))
	{
		transformComponent.m_Transform->Rotate(glm::vec3(0.0f, 1.0f * 0.25f, 0.0f));
	}

	if (input.GetKey(sad::KeyCode::RightArrow) || input.GetButton(sad::ControllerButton::R1))
	{
		transformComponent.m_Transform->Rotate(glm::vec3(0.0f, -1.0f * 0.25f, 0.0f));
	}
}

void sad::PlayerController::EditorControls(InputManager& input, const ecs::TransformComponent& transformComponent, float movespeed)
{
	// Handles up/down flight using SPACE and LSHIFT.
	if (input.GetKey(sad::KeyCode::Space) || input.GetButton(sad::ControllerButton::South))
	{
		transformComponent.m_Transform->Translate(glm::vec3(0.0f, 1.0f * movespeed, 0.0f));
	}

	if (input.GetKey(sad::KeyCode::LShift) || input.GetButton(sad::ControllerButton::East))
	{
		transformComponent.m_Transform->Translate(glm::vec3(0.0f, -1.0f * movespeed, 0.0f));
	}
}

void sad::PlayerController::Update()
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


