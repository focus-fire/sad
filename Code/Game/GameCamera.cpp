#include "sadpch.h"

#include "GameCamera.h"

sad::GameCamera::GameCamera(sad::Window window)
	: m_MainWindow(window)
{ }

sad::GameCamera::~GameCamera()
{ }

void sad::GameCamera::CalculateViewProjectionMatrix(sad::Window window)
{
	glm::mat4 rotationMatrix = glm::mat4(m_Rotation);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), window.GetAspectRatio(), 1.0f, 20.0f);

	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 0.0f, -3.0f), // Camera position
		glm::vec3(0.0f, -0.5f, 0.0f), // 'Looks At' this point
		glm::vec3(0.0f, 1.0f, 0.0f)   // Indicates that positive y is 'Up' 
	);
}

//void sad::Transform::Translate(glm::vec3 translation)
//{
//	m_Position += translation;
//}

void sad::Transform::Rotate(glm::quat rotation)
{
	m_Rotation *= rotation;
}

void sad::Transform::Rotate(glm::vec3 rotation)
{
	rotation = glm::radians(rotation);
	m_Rotation *= glm::quat(rotation);
}

void sad::Transform::Rotate(glm::vec3 axis, float angle)
{
	angle = glm::radians(angle);
	m_Rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), angle, axis);
}

//void sad::Transform::Scale(glm::vec3 scale)
//{
//	m_Scale *= scale;
//}
