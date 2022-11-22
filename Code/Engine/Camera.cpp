#include "sadpch.h"

#include "Engine/Application.h"
#include "Engine/Camera.h"

#define GLM_ENABLE_EXPERIMENTAL

glm::vec3 sad::Camera::cameraPosition;
glm::vec3 sad::Camera::cameraEulers;
bool sad::Camera::isActive;

void sad::Camera::Start()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	isActive = true;
}

glm::mat4 sad::Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(60.0f), sad::Application::s_MainWindow->GetAspectRatio(), 0.01f, 20.0f);
}

glm::mat4 sad::Camera::GetViewMatrix()
{
	if (isActive)
	{
		// Get forward direction vector from rotation orientation
		glm::vec3 forwards{
			glm::sin(glm::radians(cameraEulers.y)) * glm::cos(glm::radians(cameraEulers.z)),
			-glm::sin(glm::radians(cameraEulers.x)),
			glm::cos(glm::radians(cameraEulers.y))
		};

		// Store the global up vector, this is used to ensure that up is always up
		glm::vec3 globalUp{ 0.0f, 1.0f, 0.0f };

		// Calculate the cross product to get the right direction vector (relative to forward and global up)
		glm::vec3 right{ glm::cross(forwards, globalUp) };

		// Calculate the new "up" vector by doing cross product of the right and forwards vectors
		glm::vec3 up{ glm::cross(right, forwards) };

		// Return the appropriate lookAt vector. (Camera position, Camera forward direction, Camera's "up" direction).
		return glm::lookAt(cameraPosition, cameraPosition + forwards, up);
	}
}

glm::mat4 sad::Camera::GetViewProjectionMatrix()
{
	glm::mat4 projectionMatrix = GetProjectionMatrix();
	glm::mat4 viewMatrix = GetViewMatrix();
	return projectionMatrix * viewMatrix;
}

void sad::Camera::UnbindMouse()
{
	// Probably don't need to grab the instance multiple times
	InputManager& input = InputManager::GetInstance();

	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_WarpMouseInWindow(NULL, NULL, NULL);  // Required for First-person camera movement

}
