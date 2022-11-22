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
	* @brief Handles Editor Camera rotation and position. Also takes in keyboard and mouse input to calculate rotation and movement 
	*/

	static class EditorCamera : public Camera
	{
	public:

		EditorCamera();

		/*
		* @brief Updates editor camera every frame in the game loop
		*/
		void Update() override;
		
		/*
		* @brief Handles WASD input and state for the Editor Camera to update the camera's position
		*/
		void CurrentCameraState() override;

	private:
		static int wasdState;
		static bool walking;
		static float walkDirection;
		sad::InputManager& input;
	};
}
