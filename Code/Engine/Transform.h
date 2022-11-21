#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace sad
{
	class Transform
	{
	public:
		explicit Transform(glm::vec3 position = glm::vec3(0.0f),
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
			glm::vec3 scale = glm::vec3(1.0f));

		explicit Transform(glm::vec3 position,
			glm::vec3 rotation,
			glm::vec3 scale = glm::vec3(1.0f));

		/**
		 * @brief Retrieves a matrix representing the current transform  
		 * @return Model matrix with calculated translation, rotation, and scale matrices for the transform 
		*/
		glm::mat4 GetTransformMatrix();
		glm::mat4& GetMutableTransformMatrix();
		glm::mat3 GetNormalMatrix();

		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetRotation(glm::quat rotation) { m_Rotation = rotation; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }

		glm::vec3 GetPosition() { return m_Position; }
		glm::quat GetRotation() { return m_Rotation; }
		glm::vec3 GetScale() { return m_Scale; }

		/**
		 * @brief Performs a translation on the transform by a given vector
		 * @param translation Vector3 to translate by
		*/
		void Translate(glm::vec3 translation);

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

		/**
		 * @brief Scales the transform by a provided Vector3 
		 * @param scale Vector3 representing the amount to scale a transform on each axis 
		*/
		void Scale(glm::vec3 scale);

	private:
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;

		glm::mat4 m_TransformMatrix;
	};
}
