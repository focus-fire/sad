#include "sadpch.h"

#include "GameCamera.h"
#include "Engine/Application.h"
#include "glm/gtx/string_cast.hpp"
#include "backends/imgui_impl_glfw.h"
#include "Engine/EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL

glm::vec3 sad::GameCamera::gameCameraEulers;
bool sad::GameCamera::isActive;


sad::GameCamera::GameCamera()
{ 
	gameCameraEulers = { 0.0f, 90.0f, 0.0f };
	sad::GameCamera::isActive = true;
}

sad::GameCamera::~GameCamera()
{ }

void sad::GameCamera::Update()
{

	InputManager& input = InputManager::GetInstance();

	double mouse_x, mouse_y;
	mouse_x = input.GetMousePosition().x;
	mouse_y = input.GetMousePosition().y;

	ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
	input.SetMousePosition(800.0, 450.0);

	float delta_x{ static_cast<float>(mouse_x - 800.0) };
	sad::Camera::cameraEulers.y -= delta_x * 0.001f;

	float delta_y{ static_cast<float>(mouse_y - 450.0) };
	sad::Camera::cameraEulers.x = std::max(std::min(sad::Camera::cameraEulers.x + delta_y, 180.0f), 0.0f);

}

void sad::GameCamera::CurrentCameraState()
{

}
