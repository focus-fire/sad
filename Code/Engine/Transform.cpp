#include "sadpch.h"

#include "Transform.h"

#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>

sad::Transform::Transform(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
	: m_Position(position)
	, m_Rotation(rotation)
	, m_Scale(scale)
	, m_TransformMatrix()
{ 
}

sad::Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: m_Position(position)
	, m_Rotation(glm::radians(rotation))
	, m_Scale(scale)
	, m_TransformMatrix()
{ 
}

glm::mat4 sad::Transform::GetTransformMatrix()
{ 
	glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), m_Position);
	glm::mat4 rotationMatrix = glm::mat4(m_Rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);

	m_TransformMatrix = translateMatrix * rotationMatrix * scaleMatrix;

	return m_TransformMatrix;
}

glm::mat4& sad::Transform::GetMutableTransformMatrix()
{
	return m_TransformMatrix;
}

glm::mat3 sad::Transform::GetNormalMatrix()
{
	return glm::inverseTranspose(glm::mat3(m_TransformMatrix));
}

void sad::Transform::Translate(glm::vec3 translation)
{ 
	m_Position += translation;
}

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

void sad::Transform::Scale(glm::vec3 scale)
{ 
	m_Scale *= scale;
}
