#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "ECS/Components/TransformComponent.h"
#include "ECS/Components/PlayerControllerComponent.h"
#include "Engine/InputManager.h"
#include "Window.h"

namespace sad
{
	class GameCamera
	{
	public:
		explicit GameCamera(sad::Window window);

		~GameCamera();

		/**
		 * @brief Retrieves a matrix representing the current transform
		 * @return Model matrix with calculated translation, rotation, and scale matrices for the transform
		*/
		sad::Window window;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		void CalculateViewProjectionMatrix(sad::Window window);
		glm::mat4 GetViewProjectionMatrix() { return projectionMatrix * viewMatrix; };

		void SetRotation(glm::quat rotation) { m_Rotation = rotation; }


		/**
		 * @brief Performs a rotation on the current transform by a quaternion
		 * @param rotation Quaternion to rotate by
		*/
		void Rotate(glm::quat rotation);

		/**
		 * @brief Performs a rotation on the current transform by a Vector3
		 * @param rotation Vector3 to rotate by
		*/
		void Rotate(glm::vec3 rotation);

		/**
		 * @brief Performs a rotation on the current transform by a given axis and angle
		 * @param axis Vector3 representing the active axis(') to rotate against
		 * @param angle Float representing the angle in degrees to rotate by
		*/
		void Rotate(glm::vec3 axis, float angle);

	private:
		glm::quat m_Rotation;
		sad::Window m_MainWindow;
	};
}
