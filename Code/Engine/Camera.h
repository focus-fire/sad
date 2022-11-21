#pragma once

#include<glm/glm.hpp>

namespace sad
{
	class Camera
	{

	public:
		virtual void Update() = 0;
		static glm::mat4 GetViewMatrix();
		static glm::mat4 GetProjectionMatrix();
		static glm::mat4 GetViewProjectionMatrix();

		static glm::vec3 cameraPosition;
		static glm::vec3 cameraEulers;

	private:

	};
}