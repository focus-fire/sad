#include "sadpch.h"

#include "backends/imgui_impl_glfw.h"

#include "Engine/Application.h"
#include "EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL

float sad::EditorCamera::walkDirection;
bool sad::EditorCamera::walking;
int sad::EditorCamera::wasdState;

sad::EditorCamera::EditorCamera() : input(InputManager::GetInstance()) {}

void sad::EditorCamera::Update()
{
	// Change mouse/camera state
	if (input.GetKey(sad::KeyCode::Escape))
	{
		ToggleMouseState();
	}

	if (isActive)
	{
		// Get current camera state from input
		sad::EditorCamera::CurrentCameraState();

		double mouse_x, mouse_y;
		mouse_x = input.GetMousePosition().x;
		mouse_y = input.GetMousePosition().y;

		ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
		input.SetMousePosition(800.0, 450.0);

		// Application shouldnt be accessed in here :/
		SDL_WarpMouseInWindow(sad::Application::s_MainWindow->GetSDLWindow(), 800, 450);

		// Calculate and update camera yaw (horizontal rotation) for the camera and keep rotation between 0 <=> 360
		float delta_x{ static_cast<float>(mouse_x - 800.0) };
		cameraEulers.y -= delta_x * 0.05f;
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
		cameraEulers.x = std::max(std::min(cameraEulers.x + delta_y * 0.05f, 90.0f), -90.0f);
	}
	
}

void sad::EditorCamera::CurrentCameraState()
{
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
			-glm::sin(glm::radians(cameraEulers.x)),
			glm::sin(glm::radians(walkDirection))
		};
	}
}