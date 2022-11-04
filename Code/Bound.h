#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace sad 
{
	class Bound 
	{
	public:
		/*
		* Find enity and use transform data 
		to get position to create bounds
		*/
		Bound();
		~Bound();

		void SetPosition(glm::vec3 position) { m_Position = position; }
		void SetScale(glm::vec3 scale) { m_Scale = scale; }

		glm::vec3 GetPosition() { return m_Position; }
		glm::vec3 GetScale() { return m_Scale; }

		static void Update();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
	};
}