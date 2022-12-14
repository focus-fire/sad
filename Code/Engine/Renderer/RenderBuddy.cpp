#include "sadpch.h"

#include "RenderBuddy.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Engine/Application.h>
#include <Engine/LineRenderer.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Registry.h>
#include <Engine/Camera.h>

#include "ShaderResource.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

sad::rad::Renderer sad::rad::RenderBuddy::s_Renderer = sad::rad::Renderer();
sad::Camera* sad::rad::RenderBuddy::s_CurrentCamera = nullptr;

void sad::rad::RenderBuddy::DrawDebugLine(glm::vec3 from, glm::vec3 to, glm::vec4 color)
{
	LineRenderer lineRenderer = LineRenderer(from, to, color);

	// TODO: Retrieve the view projection matrix from the Camera
	glm::mat4 vpMatrix = sad::Camera::GetViewProjectionMatrix();

	lineRenderer.Shader->Bind();
	lineRenderer.Shader->SetUniformMatrix4fv("u_VpMatrix", glm::value_ptr(vpMatrix));
	rad::RenderBuddy::DrawLines(lineRenderer.VertexArray, lineRenderer.VertexCount);
}

void sad::rad::RenderBuddy::DrawDebugBox(glm::vec3 min, glm::vec3 max, glm::vec4 color)
{
    glm::vec3 points[8];

	// Initialize a bounding box vector with the minimum and maximum extents
	glm::vec3 boundingBox[2];
	boundingBox[0] = min;
	boundingBox[1] = max;

	// Create each point relative to the minimum and/or maximum of the bounding box
	for (int i = 0; i < 8; ++i)
	{
		points[i][0] = boundingBox[(i ^ (i >> 1)) & 1][0];
		points[i][1] = boundingBox[(i >> 1) & 1][1];
		points[i][2] = boundingBox[(i >> 2) & 1][2];
	}

	// Builds each line of the box using some nifty indexing tricks
	// Using a bitwise and is equivalent to mod4 without the computational expense of a modulo operation
	for (int i = 0; i < 4; ++i)
	{
		DrawDebugLine(std::move(points[i]), std::move(points[(i + 3) & 3]), std::move(color));
		DrawDebugLine(std::move(points[4 + i]), std::move(points[4 + ((i + 1) & 3)]), std::move(color));
		DrawDebugLine(std::move(points[i]), std::move(points[4 + i]), std::move(color));
	}
}
