#include "sadpch.h"

#include "Game/GameCamera.h"
#include "Engine/Application.h"
#include "glm/gtx/string_cast.hpp"
#define GLM_ENABLE_EXPERIMENTAL

glm::vec3 sad::GameCamera::cameraVector;

sad::GameCamera::GameCamera()
{ 
	cameraVector = glm::vec3(0.0f, 0.0f, -5.0f);
}

sad::GameCamera::~GameCamera()
{ }

glm::mat4 sad::GameCamera::GetViewProjectionMatrix()
{
	//glm::mat4 rotationMatrix = glm::mat4(m_Rotation);
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), sad::Application::s_MainWindow->GetAspectRatio(), 1.0f, 20.0f);
	//core::Log(ELogType::Debug, "Camera vector: {}", glm::to_string(cameraVector));

	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(cameraVector.x, 0.0f, -5.0f), // Camera position
		glm::vec3(cameraVector.x-3.0f, -0.5f, 2.0f), // 'Looks At' this point
		glm::vec3(0.0f, 1.0f, 0.0f)   // Indicates that positive y is 'Up' 
	);

	return projectionMatrix * viewMatrix;
}

void sad::GameCamera::SetViewMatrix(glm::vec3 vector) {
	cameraVector += vector;
}

void sad::GameCamera::RotateViewMatrix(glm::vec3 vector) {

}

//void sad::Transform::Translate(glm::vec3 translation)
//{
//	m_Position += translation;
//}

//void sad::Transform::Rotate(glm::quat rotation)
//{
//	m_Rotation *= rotation;
//}
//
//void sad::Transform::Rotate(glm::vec3 rotation)
//{
//	rotation = glm::radians(rotation);
//	m_Rotation *= glm::quat(rotation);
//}
//
//void sad::Transform::Rotate(glm::vec3 axis, float angle)
//{
//	angle = glm::radians(angle);
//	m_Rotation *= glm::rotate(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), angle, axis);
//}

//void sad::Transform::Scale(glm::vec3 scale)
//{
//	m_Scale *= scale;
//}
