#include "sadpch.h"

#include "GameCamera.h"
#include "Engine/Application.h"
#include "glm/gtx/string_cast.hpp"
#include "backends/imgui_impl_glfw.h"
#include "Game/EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL

glm::vec3 sad::GameCamera::cameraPosition;
glm::vec3 sad::GameCamera::cameraEulers;
float sad::GameCamera::walkDirection;
bool sad::GameCamera::walking;
int sad::GameCamera::wasdState;
bool sad::GameCamera::isActive;


sad::GameCamera::GameCamera()
{ 
	cameraPosition = { 0.0f, 0.0f, -5.0f };
	cameraEulers = { 0.0f, 90.0f, 0.0f };
	walkDirection = cameraEulers.z;
	bool walking = false;
}

sad::GameCamera::~GameCamera()
{ }

void sad::GameCamera::Update()
{
	if (sad::GameCamera::isActive) 
	{
		sad::GameCamera::CameraState();
		InputManager& input = InputManager::GetInstance();

		double mouse_x, mouse_y;
		mouse_x = input.GetMousePosition().x;
		mouse_y = input.GetMousePosition().y;

		ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
		input.SetMousePosition(800.0, 450.0);

		float delta_x{ static_cast<float>(mouse_x - 800.0) };
		sad::GameCamera::cameraEulers.y -= delta_x * 0.001f;

		float delta_y{ static_cast<float>(mouse_y - 450.0) };
		sad::GameCamera::cameraEulers.x = std::max(std::min(sad::GameCamera::cameraEulers.x + delta_y, 180.0f), 0.0f);
	}
}

glm::mat4 sad::GameCamera::GetProjectionMatrix() 
{
	return glm::perspective(glm::radians(60.0f), sad::Application::s_MainWindow->GetAspectRatio(), 0.1f, 20.0f);
}

glm::mat4 sad::GameCamera::GetViewMatrix() 
{
	glm::vec3 forwards{
		glm::sin(glm::radians(cameraEulers.y)) * glm::cos(glm::radians(cameraEulers.z)),
		glm::sin(glm::radians(cameraEulers.y)) * glm::sin(glm::radians(cameraEulers.z)),
		glm::cos(glm::radians(cameraEulers.y))
	};

	glm::vec3 globalUp{ 0.0f, 1.0f, 0.0f };

	glm::vec3 right{ glm::cross(forwards, globalUp) };

	glm::vec3 up{ glm::cross(right, forwards) };
	return glm::lookAt(cameraPosition, cameraPosition + forwards, up);
}

glm::mat4 sad::GameCamera::GetViewProjectionMatrix()
{
	glm::mat4 projectionMatrix = GetProjectionMatrix();
	glm::mat4 viewMatrix = GetViewMatrix();
	return projectionMatrix * viewMatrix;
}

void sad::GameCamera::CameraState()
{

	InputManager& input = InputManager::GetInstance();

	// Capture player state
	sad::GameCamera::walkDirection = -sad::GameCamera::cameraEulers.y;
	sad::GameCamera::wasdState = 0;
	sad::GameCamera::walking = false;

	// Handles forward/backward movement using W and S
	if (input.GetKey(sad::KeyCode::W))
	{
		sad::GameCamera::wasdState += 1;
	}

	if (input.GetKey(sad::KeyCode::S))
	{
		sad::GameCamera::wasdState += 4;
	}

	// Handles left/right movement using A and D
	if (input.GetKey(sad::KeyCode::A))
	{
		sad::GameCamera::wasdState += 2;
	}

	if (input.GetKey(sad::KeyCode::D))
	{
		sad::GameCamera::wasdState += 8;
	}


	// Interpret wasd state
	switch (sad::GameCamera::wasdState)
	{
	case 1:
	case 11:
		// Forwards
		sad::GameCamera::walking = true;
		sad::GameCamera::walkDirection += 90;
		break;
	case 3:
		//left-forwards
		sad::GameCamera::walking = true;
		sad::GameCamera::walkDirection += 45;
		break;
	case 2:
	case 7:
		//left
		sad::GameCamera::walking = true;
		break;
	case 6:
		//left-backwards
		sad::GameCamera::walking = true;
		sad::GameCamera::walkDirection += 315;
		break;
	case 4:
	case 14:
		//backwards
		sad::GameCamera::walking = true;
		sad::GameCamera::walkDirection += 270;
		break;
	case 12:
		//right-backwards
		sad::GameCamera::walking = true;
		sad::GameCamera::walkDirection += 225;
		break;
	case 8:
	case 13:
		//right
		sad::GameCamera::walking = true;
		sad::GameCamera::walkDirection += 180;
		break;
	case 9:
		//right-forwards
		sad::GameCamera::walking = true;
		sad::GameCamera::walkDirection += 135;
	}

	if (sad::GameCamera::walking)
	{
		sad::GameCamera::cameraPosition += 0.01f * glm::vec3{
			glm::cos(glm::radians(sad::GameCamera::walkDirection)),
			0.0f,
			glm::sin(glm::radians(sad::GameCamera::walkDirection))
		};
	}
}

void sad::GameCamera::ToggleActive()
{
	if (sad::GameCamera::isActive)
	{
		sad::EditorCamera::isActive = true;
		sad::GameCamera::isActive = false;
	}
	else 
	{
		sad::EditorCamera::isActive = false;
		sad::GameCamera::isActive = true;

	}
}
