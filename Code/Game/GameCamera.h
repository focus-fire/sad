#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Engine/ECS/Components/TransformComponent.h"
#include "Engine/InputManager.h"
#include "Engine/Window.h"
#include "Engine/Camera.h"

namespace sad
{
	/*
	* @brief Handles Game Camera rotation and position.
	*/

	static class GameCamera : public Camera
	{
	public:

		GameCamera();

		/*
		* @brief Updates game camera every frame in the game loop
		*/
		void Update() override;

		/*
		* @brief Handles camera position based on Player object and also applies rotation to player object
		*/
		void CurrentCameraState() override;

	private:
		sad::InputManager& input;
	};
}
