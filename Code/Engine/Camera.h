#pragma once

#include<glm/glm.hpp>

namespace sad
{

	/*
	* @brief Handles matrix math for the Camera
	*/
	class Camera
	{

	public:
		virtual void Update() = 0;

		virtual void CurrentCameraState() = 0;

		static void Start();

		/*
		* @brief Returns the view matrix (lookAt matrix) after calculating forward vector
		*/
		static glm::mat4 GetViewMatrix();

		/*
		* @brief Returns projection matrix. Contains camera perspective information
		*/
		static glm::mat4 GetProjectionMatrix();

		/*
		* @brief Returns the calculation of the View and Projection matrices
		*/
		static glm::mat4 GetViewProjectionMatrix();

		/*
		* @brief Unbind mouse from screen
		*/
		static void ToggleMouseState();

		/*
		* @brief Stores the Camera position
		*/
		static glm::vec3 cameraPosition;

		/*
		* @brief Stores the camera's pitch (vertical rotation) and yaw (horizontal rotation)
		*/
		static glm::vec3 cameraEulers;

		static bool isActive;

	};
}