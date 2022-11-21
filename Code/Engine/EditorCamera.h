#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/InputManager.h"
#include "Engine/Window.h"
#include "Engine/Camera.h"

namespace sad
{
	static class EditorCamera : public Camera
	{
	public:
		explicit EditorCamera();

		~EditorCamera();

		sad::EditorCamera& GetInstance() { return *this; };
		const sad::EditorCamera& GetInstance() const { return *this; };
		void Update() override;

		static bool isActive;
		
		void CurrentCameraState();


	private:
		static int wasdState;
		static bool walking;
		static float walkDirection;
	};
}
