#include "sadpch.h"

#include "Editor.h"

#include <glm/gtc/type_ptr.hpp>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl3.h>
#include <imterm/terminal.hpp>
#include <ImGuizmo.h>
#include <entt/entt.hpp>

#include <Engine/Application.h>
#include <Engine/Scripting/ScriptingEngine.h>
#include <Engine/ECS/Components/NameComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/RenderableObjectComponent.h>

#include <Game/Time.h>
#include <Game/EditorCamera.h>
#include <Game/GameCamera.h>

namespace 
{
	static ImGuizmo::OPERATION m_CurrentGizmoOperation = ImGuizmo::OPERATION(ImGuizmo::TRANSLATE);
}

cap::Editor::Editor()
	: m_CurrentLevelContext(nullptr)
	, m_DebugTerminal(new cap::DebugTerminal())
	, m_GameWindowFlags(ImGuiWindowFlags_None)
	, m_ShowGameWindow(true)
	, m_GameWindowTitle("Default - sadEngine")
	, m_IsEditorInPlayMode(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	m_GameWindowWidth = static_cast<float>(sad::Application::s_MainWindow->GetWidth()) / 1.25f;
	m_GameWindowHeight = static_cast<float>(sad::Application::s_MainWindow->GetHeight()) / 1.25f;
}

cap::Editor::~Editor()
{
	delete m_DebugTerminal;
}

void cap::Editor::Start()
{
	ImGui_ImplSDL2_InitForOpenGL(sad::Application::s_MainWindow->GetSDLWindow(), sad::Application::s_MainWindow->GetGLContext());
	ImGui_ImplOpenGL3_Init("#version 150");

	m_DebugTerminal->Start();
}

void cap::Editor::CatchSDLEvents(const SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void cap::Editor::Clear()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void cap::Editor::RenderGameWindow(unsigned int frameBufferTextureId)
{
	// Update game window title with current level name
	// if (!m_CurrentLevelContext)
	// {
	// 	m_CurrentLevelContext = sad::cs::ScriptingEngine::GetCurrentLevelInstance();
	// }
	// else
	// {
	// 	m_GameWindowTitle = m_CurrentLevelContext->LevelName + " - sadEngine";
	// }

	bool showGameWindow = true;

	// Set the window size once when the window opens
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.83f, 0.83f, 0.83f));

	ImGui::Begin(m_GameWindowTitle.c_str(), &showGameWindow, m_GameWindowFlags);
	ImGui::PopStyleColor();

	ImGui::SetWindowSize(ImVec2(m_GameWindowWidth / 1.25, m_GameWindowHeight / 1.25), ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(50.0, 25.0), ImGuiCond_Once);

	// Pass frameBuffer texture to be rendered in window
	ImVec2 availableSize = ImGui::GetContentRegionAvail();
	ImGui::Image(INT_TO_VOIDP(frameBufferTextureId), availableSize, ImVec2(0, 1), ImVec2(1, 0));

	// TODO: Abstract into interaction mode class
	// Rendering gizmos must be completed while framebuffer is still being rendered
	// This allows the grid and all gizmos to properly appear in the game window
	if (sad::Application::s_EngineState->GetEngineMode() == sad::EEngineMode::Editor)
	{
		int entityId = 0;
		//sad::EditorCamera::ToggleActive();
		auto view = sad::ecs::Registry::GetEntityWorld().view<sad::ecs::TransformComponent, const sad::ecs::RenderableObjectComponent>();
		for (auto [entity, transformComponent, renderableObject] : view.each())
		{
			ImGuizmo::SetID(entityId);

			sad::Transform* transform = transformComponent.m_Transform.get();
			std::vector<glm::vec3> manipulatedTransform = std::vector<glm::vec3>(3);
			float* model = glm::value_ptr(transform->GetMutableTransformMatrix());

			manipulatedTransform = RenderGizmos(model, m_LastGizmoUsed == entityId);

			// Reapply transformations caused by gizmo manipulation
			transform->SetPosition(manipulatedTransform[0]);
			transform->SetRotation(glm::radians(manipulatedTransform[1]));
			transform->SetScale(manipulatedTransform[2]);

			if (ImGuizmo::IsUsing())
				m_LastGizmoUsed = entityId;
			
			entityId++;
		}
	}

	ImGui::End();
}

std::vector<glm::vec3> cap::Editor::RenderGizmos(float* modelMatrix, bool transformDecomposition)
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

	ImGui::Begin("Transform", 0);
	ImGui::SetWindowPos(ImVec2(1150.0f, 115.0f), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(250.0f, 125.0f), ImGuiCond_Once);

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
	m_GameWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : ImGuiWindowFlags_None;

	if (sad::GameCamera::isActive)
	{
		glm::mat4 viewMatrix = sad::GameCamera::GetViewMatrix();
		glm::mat4 projectionMatrix = sad::GameCamera::GetProjectionMatrix();
		//glm::mat4 viewMatrix = sad::GameCamera::GetViewMatrix();
		//glm::mat4 projectionMatrix = sad::GameCamera::GetProjectionMatrix();
		ImGuizmo::DrawGrid(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), glm::value_ptr(glm::mat4(1.0f)), 100.0f);
		ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), m_CurrentGizmoOperation, currentGizmoMode, modelMatrix, NULL, NULL, NULL, NULL);
		ImGuizmo::ViewManipulate(glm::value_ptr(viewMatrix), 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
	}
	else if (sad::EditorCamera::isActive)
	{
		glm::mat4 viewMatrix = sad::EditorCamera::GetViewMatrix();
		glm::mat4 projectionMatrix = sad::EditorCamera::GetProjectionMatrix();
		//glm::mat4 viewMatrix = sad::GameCamera::GetViewMatrix();
		//glm::mat4 projectionMatrix = sad::GameCamera::GetProjectionMatrix();
		ImGuizmo::DrawGrid(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), glm::value_ptr(glm::mat4(1.0f)), 100.0f);
		ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), m_CurrentGizmoOperation, currentGizmoMode, modelMatrix, NULL, NULL, NULL, NULL);
		ImGuizmo::ViewManipulate(glm::value_ptr(viewMatrix), 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
	}


	// Re-decompose new model matrix again once operation is complete
	float finalTranslation[3], finalRotation[3], finalScale[3];
	ImGuizmo::DecomposeMatrixToComponents(modelMatrix, finalTranslation, finalRotation, finalScale);

	return { glm::make_vec3(finalTranslation), glm::make_vec3(finalRotation), glm::make_vec3(finalScale) };
}

void cap::Editor::Render()
{
	m_DebugTerminal->Render();

	const char* currentMode = m_IsEditorInPlayMode ? "Pause" : "Play";
	pog::Time::TimeScale = m_IsEditorInPlayMode ? 1.0f : 0.0f;

	ImGui::Begin("Action Panel");
	ImGui::SetWindowPos(ImVec2(1150.0f, 35.0f), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(125.0f, 65.0f), ImGuiCond_Once);
	if (ButtonCenteredOnLine(currentMode))
	{
		m_IsEditorInPlayMode = !m_IsEditorInPlayMode;
		sad::EditorCamera::ToggleActive();
		core::SignalEvent("OnToggleEngineMode");
	}
	ImGui::End();

	// Really scuffed way to list entities, included mainly for debugging
	// Cycles through available names and lists them in the editor
	ImGui::Begin("Scuffed Entity List");
	ImGui::SetWindowPos(ImVec2(1150.0f, 265.0f), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(200.0f, 225.0f), ImGuiCond_Once);
	auto view = sad::ecs::Registry::GetEntityWorld().view<sad::ecs::NameComponent>();
	for (auto [entity, name] : view.each())
		ImGui::Text(name.m_Name.c_str());
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool cap::Editor::ButtonCenteredOnLine(const char* label, float alignment /* = 0.5f */)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label);
}

void cap::Editor::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
