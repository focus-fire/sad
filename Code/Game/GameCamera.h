#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/InputManager.h"
#include "Engine/Window.h"
#include "Engine/Camera.h"

namespace sad
{
	static class GameCamera : public Camera
	{
	public:

		void Update() override;

		void CurrentCameraState();

	private:

	};
}
