#include "sadpch.h"

#include "GameCamera.h"
#include "Engine/Application.h"
#include "glm/gtx/string_cast.hpp"
#include "backends/imgui_impl_glfw.h"

#define GLM_ENABLE_EXPERIMENTAL

glm::vec3 sad::GameCamera::cameraPosition;
glm::vec3 sad::GameCamera::cameraEulers;
float sad::GameCamera::walkDirection;
bool sad::GameCamera::walking;
int sad::GameCamera::wasdState;


sad::GameCamera::GameCamera()
{ 
	cameraPosition = { 0.0f, 0.0f, -5.0f };
	cameraEulers = { 0.0f, 90.0f, 0.0f };
	walkDirection = cameraEulers.z;
	bool walking = false;
	ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	//rotateVector = glm::vec3(0.0f, 0.0f, 0.0f);
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
	sad::GameCamera::cameraEulers.y -= delta_x;

	float delta_y{ static_cast<float>(mouse_y - 450.0) };
	sad::GameCamera::cameraEulers.x = std::max(std::min(sad::GameCamera::cameraEulers.x + delta_y, 180.0f), 0.0f);
}

glm::mat4 sad::GameCamera::GetProjectionMatrix() 
{
	return glm::perspective(glm::radians(60.0f), sad::Application::s_MainWindow->GetAspectRatio(), 1.0f, 20.0f);
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

//void sad::Transform::Translate(glm::vec3 translation)
//{
//	m_Position += translation;
//}

//void sad::Transform::Rotate(glm::quat rotation)
//{
//	m_Rotation *= rotation;
//}
//
//void sad::Transform::Rotate(glm::vec3 rotation)
//{
//	rotation = glm::radians(rotation);
//	m_Rotation *= glm::quat(rotation);
//}
//
//void sad::Transform::Rotate(glm::vec3 axis, float angle)
//{
//	angle = glm::radians(angle);
//	m_Rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), angle, axis);
//}

//void sad::Transform::Scale(glm::vec3 scale)
//{
//	m_Scale *= scale;
//}
