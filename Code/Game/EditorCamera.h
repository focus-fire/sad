#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/InputManager.h"
#include "Engine/Window.h"

namespace sad
{
	static class EditorCamera
	{
	public:
		explicit EditorCamera();

		~EditorCamera();

		sad::EditorCamera& GetInstance() { return *this; };
		const sad::EditorCamera& GetInstance() const { return *this; };
		static void Update();

		static glm::mat4 GetViewMatrix();

		static glm::mat4 GetProjectionMatrix();

		static glm::mat4 GetViewProjectionMatrix();

		static void ToggleActive();

		static void CameraState();

		static int wasdState;
		static bool walking;
		static float walkDirection;
		static glm::vec3 cameraPosition;
		static glm::vec3 cameraEulers;
		static bool isActive;
	private:

	};
}
