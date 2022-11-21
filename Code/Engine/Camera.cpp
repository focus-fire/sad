#include "sadpch.h"

#include "Engine/Application.h"
#include "Engine/Camera.h"
#include "glm/gtx/string_cast.hpp"
#include "backends/imgui_impl_glfw.h"

#define GLM_ENABLE_EXPERIMENTAL

glm::vec3 sad::Camera::cameraPosition;
glm::vec3 sad::Camera::cameraEulers;


glm::mat4 sad::Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(60.0f), sad::Application::s_MainWindow->GetAspectRatio(), 0.1f, 20.0f);
}

glm::mat4 sad::Camera::GetViewMatrix()
{
	glm::vec3 forwards{
		glm::sin(glm::radians(cameraEulers.y)) * glm::cos(glm::radians(cameraEulers.z)),
		-glm::sin(glm::radians(cameraEulers.x)),
		glm::cos(glm::radians(cameraEulers.y))
	};

	glm::vec3 globalUp{ 0.0f, 1.0f, 0.0f };

	glm::vec3 right{ glm::cross(forwards, globalUp) };

	glm::vec3 up{ glm::cross(right, forwards) };

	return glm::lookAt(cameraPosition, cameraPosition + forwards, up);
}

glm::mat4 sad::Camera::GetViewProjectionMatrix()
{
	glm::mat4 projectionMatrix = GetProjectionMatrix();
	glm::mat4 viewMatrix = GetViewMatrix();
	return projectionMatrix * viewMatrix;
}
