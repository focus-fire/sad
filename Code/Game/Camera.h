#pragma once
#include<glm/glm.hpp>

namespace sad
{
	class Camera
	{

	public:
		void ToggleCamera();
		void Update();

	private:
		
		static int wasdState;
		static bool walking;
		static float walkDirection;
		static glm::vec3 cameraPosition;
		static glm::vec3 cameraEulers;
		static bool isActive;
	};
}