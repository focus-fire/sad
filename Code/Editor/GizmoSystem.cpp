#include "sadpch.h"

#include "GizmoSystem.h"

#include <glm/gtc/type_ptr.hpp>

#include <ImGuizmo.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>

#include <Engine/Camera.h>
#include <Engine/Application.h>
#include <Engine/Renderer/RenderBuddy.h>
#include <Engine/ECS/Registry.h>
#include <Engine/ECS/Components/NameComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>

ImGuiWindowFlags cap::GizmoSystem::s_GameWindowFlags; 

const float c_TransformWindowY = 65.0f + 390.0f;
const float c_TransformWindowHeight = 125.0f;
const float c_RightColumnWidth = 250.0f;

namespace 
{
	static ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::OPERATION(ImGuizmo::TRANSLATE);
}

void cap::GizmoSystem::Render()
{
	int entityId = 0;

	// All renderable objects are tagged with a 'GuidComponent'
	// This means that gizmos will attach to all primitives and models in the level 
	auto view = sad::ecs::Registry::GetEntityWorld().view<sad::ecs::TransformComponent, const sad::ecs::GuidComponent>();
	for (auto [entity, transformComponent, guid] : view.each())
	{
		ImGuizmo::SetID(entityId);

		sad::Transform* transform = transformComponent.m_Transform.get();
		std::vector<glm::vec3> manipulatedTransform = std::vector<glm::vec3>(3);
		float* model = glm::value_ptr(transform->GetMutableTransformMatrix());

		manipulatedTransform = UpdateGizmos(model, m_LastGizmoUsed == entityId);

		// Reapply transformations caused by gizmo manipulation
		transform->SetPosition(manipulatedTransform[0]);
		transform->SetRotation(glm::radians(manipulatedTransform[1]));
		transform->SetScale(manipulatedTransform[2]);

		if (ImGuizmo::IsUsing())
			m_LastGizmoUsed = entityId;
		
		entityId++;
	}
}

std::vector<glm::vec3> cap::GizmoSystem::UpdateGizmos(float* modelMatrix, bool transformDecomposition)
{
	static ImGuizmo::MODE currentGizmoMode(ImGuizmo::LOCAL);

	// Allow ImGuizmo to setup framebuffer from ImGui drawlist
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::Enable(true);

	// Allow ImGuizmo screen space to match the space allocated by ImGui
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 windowPos = ImGui::GetWindowPos();
	ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

	float viewManipulateRight = windowPos.x + ImGui::GetWindowWidth();
	float viewManipulateTop = windowPos.y;

	ImGui::Begin("Inspector", 0);
	float transformWindowX = sad::Application::s_MainWindow->GetWidth() - c_RightColumnWidth;
	ImGui::SetWindowPos(ImVec2(transformWindowX, c_TransformWindowY), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(c_RightColumnWidth, c_TransformWindowHeight), ImGuiCond_Always);

	// Check for hotkey to update gizmo operation for particular object
	if (transformDecomposition)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R))
			m_CurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_E)) 
			m_CurrentGizmoOperation = ImGuizmo::SCALE;

		// Allow radio button to update for gizmo operation 
		if (ImGui::RadioButton("Translate", m_CurrentGizmoOperation == ImGuizmo::TRANSLATE))
			m_CurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", m_CurrentGizmoOperation == ImGuizmo::ROTATE))
			m_CurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", m_CurrentGizmoOperation == ImGuizmo::SCALE))
			m_CurrentGizmoOperation = ImGuizmo::SCALE;

		// Calculate guizmo edits on a given model matrix...
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(modelMatrix, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Translate", matrixTranslation);
		ImGui::InputFloat3("Rotate", matrixRotation);
		ImGui::InputFloat3("Scale", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, modelMatrix);
	}

	ImGui::End();

	// Freeze game window if gizmo movement is detected
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	s_GameWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : ImGuiWindowFlags_None;

	sad::Camera* currentCamera = sad::rad::RenderBuddy::GetCameraInstance();
	glm::mat4 viewMatrix = currentCamera->GetViewMatrix();
	glm::mat4 projectionMatrix = currentCamera->GetProjectionMatrix();
	ImGuizmo::DrawGrid(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), glm::value_ptr(glm::mat4(1.0f)), 100.0f);
	ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), m_CurrentGizmoOperation, currentGizmoMode, modelMatrix, NULL, NULL, NULL, NULL);
	ImGuizmo::ViewManipulate(glm::value_ptr(viewMatrix), 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

	// Re-decompose new model matrix again once operation is complete
	float finalTranslation[3], finalRotation[3], finalScale[3];
	ImGuizmo::DecomposeMatrixToComponents(modelMatrix, finalTranslation, finalRotation, finalScale);

	return { glm::make_vec3(finalTranslation), glm::make_vec3(finalRotation), glm::make_vec3(finalScale) };
}
