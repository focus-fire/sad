#include "sadpch.h"

#include "RenderBuddy.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <Engine/Application.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/Registry.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

sad::rad::Renderer sad::rad::RenderBuddy::s_Renderer = sad::rad::Renderer();

void sad::rad::RenderBuddy::DrawDebugLine(glm::vec3 from, glm::vec3 to, glm::vec3 color)
{
	float points[12];
	
	points[0] = from.x;
	points[1] = from.y;
	points[2] = from.z;
	points[3] = color.r;
	points[4] = color.g;
	points[5] = color.b;

	points[6] = to.x;
	points[7] = to.y;
	points[8] = to.z;
	points[9] = color.r;
	points[10] = color.g;
	points[11] = color.b;

	// TODO: Change to platform safe path
	Shader shader = Shader("../Data/Shaders/Line.glsl");
	shader.Bind();
	shader.SetUniformMatrix4fv("u_VpMatrix", glm::value_ptr(sad::Application::GetViewProjectionMatrix()));

	VertexArray va = VertexArray();
	va.Bind();

	VertexBuffer vb = VertexBuffer(points, sizeof(points));
	vb.Bind();
	
	VertexAttributeContainer vac = VertexAttributeContainer();
	vac.AddFloatAttribute(3);
	vac.AddFloatAttribute(3);

	va.AddBufferWithAttributes(vb, vac);
	rad::RenderBuddy::DrawLines(std::move(va), 2);

	va.Unbind();
	vb.Unbind();
	shader.Unbind();
}

void sad::rad::RenderBuddy::DrawDebugBox()
{

}

void sad::rad::RenderBuddy::DrawDebugSphere()
{

}
