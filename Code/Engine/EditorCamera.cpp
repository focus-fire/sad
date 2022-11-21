#include "sadpch.h"

#include "backends/imgui_impl_glfw.h"

#include "Engine/Application.h"
#include "Game/GameCamera.h"

#include "EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL

float sad::EditorCamera::walkDirection;
bool sad::EditorCamera::walking;
int sad::EditorCamera::wasdState;


sad::EditorCamera::EditorCamera()
{
	cameraEulers = { 0.0f, 90.0f, 0.0f };
	bool walking = false;
}

sad::EditorCamera::~EditorCamera(){}


void sad::EditorCamera::Update()
{
	sad::EditorCamera::CurrentCameraState();
	InputManager& input = InputManager::GetInstance();

	double mouse_x, mouse_y;
	mouse_x = input.GetMousePosition().x;
	mouse_y = input.GetMousePosition().y;

	ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
	input.SetMousePosition(800.0, 450.0);

	float delta_x{ static_cast<float>(mouse_x - 800.0) };
	cameraEulers.y -= delta_x * 0.06f;

	float delta_y{ static_cast<float>(mouse_y - 450.0) };
	cameraEulers.x = std::max(std::min(cameraEulers.x + delta_y * 0.05f, 90.0f), -90.0f);
	
}

void sad::EditorCamera::CurrentCameraState()
{

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

	if (input.GetKey(sad::KeyCode::Escape))
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		SDL_WarpMouseInWindow(NULL, NULL, NULL);
	}

	// Interpret wasd state
	switch (wasdState)
	{
	case 1:
	case 11:
		// Forwards
		walking = true;
		walkDirection += 90;
		break;
	case 3:
		//left-forwards
		walking = true;
		walkDirection += 45;
		break;
	case 2:
	case 7:
		//left
		walking = true;
		break;
	case 6:
		//left-backwards
		walking = true;
		walkDirection += 315;
		break;
	case 4:
	case 14:
		//backwards
		walking = true;
		walkDirection += 270;
		break;
	case 12:
		//right-backwards
		walking = true;
		walkDirection += 225;
		break;
	case 8:
	case 13:
		//right
		walking = true;
		walkDirection += 180;
		break;
	case 9:
		//right-forwards
		walking = true;
		walkDirection += 135;
	}

	if (walking)
	{
		cameraPosition += 0.01f * glm::vec3{
			glm::cos(glm::radians(walkDirection)),
			-glm::sin(glm::radians(cameraEulers.x)),
			glm::sin(glm::radians(walkDirection))
		};
	}
}
