#include "sadpch.h"

#include "EditorCamera.h"
#include "Engine/Application.h"
#include "glm/gtx/string_cast.hpp"
#include "backends/imgui_impl_glfw.h"
#include "Game/GameCamera.h"

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
	sad::Camera::cameraEulers.y -= delta_x * 0.06f;

	float delta_y{ static_cast<float>(mouse_y - 450.0) };
	//sad::Camera::cameraEulers.x = std::max(std::min(sad::Camera::cameraEulers.x + delta_y, 90.0f), -10.0f);
	sad::Camera::cameraEulers.x = std::max(std::min(sad::Camera::cameraEulers.x + delta_y * 0.05f, 90.0f), -90.0f);
	
}

void sad::EditorCamera::CurrentCameraState()
{

	InputManager& input = InputManager::GetInstance();

	// Capture player state
	sad::EditorCamera::walkDirection = -sad::EditorCamera::cameraEulers.y;
	sad::EditorCamera::wasdState = 0;
	sad::EditorCamera::walking = false;

	// Handles forward/backward movement using W and S
	if (input.GetKey(sad::KeyCode::W))
	{
		sad::EditorCamera::wasdState += 1;
	}

	if (input.GetKey(sad::KeyCode::S))
	{
		sad::EditorCamera::wasdState += 4;
	}

	// Handles left/right movement using A and D
	if (input.GetKey(sad::KeyCode::A))
	{
		sad::EditorCamera::wasdState += 2;
	}

	if (input.GetKey(sad::KeyCode::D))
	{
		sad::EditorCamera::wasdState += 8;
	}


	// Interpret wasd state
	switch (sad::EditorCamera::wasdState)
	{
	case 1:
	case 11:
		// Forwards
		sad::EditorCamera::walking = true;
		sad::EditorCamera::walkDirection += 90;
		break;
	case 3:
		//left-forwards
		sad::EditorCamera::walking = true;
		sad::EditorCamera::walkDirection += 45;
		break;
	case 2:
	case 7:
		//left
		sad::EditorCamera::walking = true;
		break;
	case 6:
		//left-backwards
		sad::EditorCamera::walking = true;
		sad::EditorCamera::walkDirection += 315;
		break;
	case 4:
	case 14:
		//backwards
		sad::EditorCamera::walking = true;
		sad::EditorCamera::walkDirection += 270;
		break;
	case 12:
		//right-backwards
		sad::EditorCamera::walking = true;
		sad::EditorCamera::walkDirection += 225;
		break;
	case 8:
	case 13:
		//right
		sad::EditorCamera::walking = true;
		sad::EditorCamera::walkDirection += 180;
		break;
	case 9:
		//right-forwards
		sad::EditorCamera::walking = true;
		sad::EditorCamera::walkDirection += 135;
	}

	if (sad::EditorCamera::walking)
	{
		sad::Camera::cameraPosition += 0.01f * glm::vec3{
			glm::cos(glm::radians(sad::EditorCamera::walkDirection)),
			-glm::sin(glm::radians(sad::EditorCamera::cameraEulers.x)),
			glm::sin(glm::radians(sad::EditorCamera::walkDirection))
		};
	}
}
