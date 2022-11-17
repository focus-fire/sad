#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/InputManager.h"
#include "Engine/Window.h"

namespace sad
{
	static class GameCamera
	{
	public:
		explicit GameCamera();

		~GameCamera();

		static void Update();

		static glm::mat4 GetViewMatrix();

		static glm::mat4 GetProjectionMatrix();

		static glm::mat4 GetViewProjectionMatrix();

		static void SetViewMatrix(glm::vec3 vector);

		static void RotateViewMatrix(glm::vec3 vector);

		static int wasdState;
		static bool walking;
		static float walkDirection;
		static glm::vec3 cameraPosition;
		static glm::vec3 cameraEulers;

	private:

	};
}
