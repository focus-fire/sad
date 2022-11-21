#include "sadpch.h"

#include "EditorCamera.h"
#include "Engine/Application.h"
#include "glm/gtx/string_cast.hpp"
#include "backends/imgui_impl_glfw.h"
#include "Game/GameCamera.h"

#define GLM_ENABLE_EXPERIMENTAL

glm::vec3 sad::EditorCamera::cameraPosition;
glm::vec3 sad::EditorCamera::cameraEulers;
float sad::EditorCamera::walkDirection;
bool sad::EditorCamera::walking;
int sad::EditorCamera::wasdState;
bool sad::EditorCamera::isActive;


sad::EditorCamera::EditorCamera()
{
	cameraPosition = { 0.0f, 0.0f, -5.0f };
	cameraEulers = { 0.0f, 90.0f, 0.0f };
	walkDirection = cameraEulers.z;
	bool walking = false;
}

sad::EditorCamera::~EditorCamera(){}


void sad::EditorCamera::Update()
{
	if (sad::EditorCamera::isActive)
	{
		sad::EditorCamera::CameraState();
		InputManager& input = InputManager::GetInstance();

		double mouse_x, mouse_y;
		mouse_x = input.GetMousePosition().x;
		mouse_y = input.GetMousePosition().y;

		ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
		input.SetMousePosition(800.0, 450.0);

		float delta_x{ static_cast<float>(mouse_x - 800.0) };
		sad::EditorCamera::cameraEulers.y -= delta_x;

		float delta_y{ static_cast<float>(mouse_y - 450.0) };
		sad::EditorCamera::cameraEulers.x = std::max(std::min(sad::EditorCamera::cameraEulers.x + delta_y, 180.0f), 0.0f);
	}
}

glm::mat4 sad::EditorCamera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(60.0f), sad::Application::s_MainWindow->GetAspectRatio(), 0.1f, 20.0f);
}

glm::mat4 sad::EditorCamera::GetViewMatrix()
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

glm::mat4 sad::EditorCamera::GetViewProjectionMatrix()
{
	glm::mat4 projectionMatrix = GetProjectionMatrix();
	glm::mat4 viewMatrix = GetViewMatrix();
	return projectionMatrix * viewMatrix;
}

void sad::EditorCamera::CameraState()
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
		core::Log(ELogType::Info, "### Editor Camera Active ###");
		sad::EditorCamera::cameraPosition += 0.01f * glm::vec3{
			glm::cos(glm::radians(sad::EditorCamera::walkDirection)),
			0.0f,
			glm::sin(glm::radians(sad::EditorCamera::walkDirection))
		};
	}
}

void sad::EditorCamera::ToggleActive()
{
	if (sad::EditorCamera::isActive)
	{
		sad::EditorCamera::isActive = false;
		sad::GameCamera::isActive = true;
	}
	else {
		sad::GameCamera::isActive = true;
		sad::EditorCamera::isActive = false;

	}
}