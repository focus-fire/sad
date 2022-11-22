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
#include <Engine/Camera.h>
#include <Engine/LevelManager.h>

#include <Game/Time.h>

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

const float rightColumnWidth = 250.0f;
const float windowTopEdge = 0.0f;
const float windowLeftEdge = 0.0f;
const float gameWindowX = windowLeftEdge;
const float gameWindowY = windowTopEdge;
const float gameWindowRightEdge = 1025.0f;
const float playWindowX = gameWindowRightEdge;
const float playWindowY = windowTopEdge;
const float playWindowHeight = 65.0f;
const float listWindowX = gameWindowRightEdge;
const float listWindowY = playWindowHeight;
const float listWindowHeight = 390.0f;
const float transformWindowX = gameWindowRightEdge;
const float transformWindowY = listWindowY + listWindowHeight;
const float transformWindowHeight = 125.0f;


void cap::Editor::RenderGameWindow(unsigned int frameBufferTextureId)
{
	bool showGameWindow = true;

	// Set the window size once when the window opens
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.83f, 0.83f, 0.83f));

	ImGui::Begin(m_GameWindowTitle.c_str(), &showGameWindow, m_GameWindowFlags);
	ImGui::PopStyleColor();

    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow))
	{
		core::SignalEvent("OnInputUnlock");
	}
	else
	{
		core::SignalEvent("OnInputLock");
	}

	ImGui::SetWindowSize(ImVec2(m_GameWindowWidth / 1.25, m_GameWindowHeight / 1.25), ImGuiCond_Always);
	ImGui::SetWindowPos(ImVec2(gameWindowX, gameWindowY), ImGuiCond_Once);

	// Pass frameBuffer texture to be rendered in window
	ImVec2 availableSize = ImGui::GetContentRegionAvail();
	ImGui::Image(INT_TO_VOIDP(frameBufferTextureId), availableSize, ImVec2(0, 1), ImVec2(1, 0));

	// TODO: Abstract into interaction mode class
	// Rendering gizmos must be completed while framebuffer is still being rendered
	// This allows the grid and all gizmos to properly appear in the game window
	if (sad::Application::s_EngineState->GetEngineMode() == sad::EEngineMode::Editor)
	{
		int entityId = 0;
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
	ImGui::SetWindowPos(ImVec2(transformWindowX, transformWindowY), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(rightColumnWidth, transformWindowHeight), ImGuiCond_Once);

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

	glm::mat4 viewMatrix = sad::Camera::GetViewMatrix();
	glm::mat4 projectionMatrix = sad::Camera::GetProjectionMatrix();
	ImGuizmo::DrawGrid(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), glm::value_ptr(glm::mat4(1.0f)), 100.0f);
	ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), m_CurrentGizmoOperation, currentGizmoMode, modelMatrix, NULL, NULL, NULL, NULL);
	ImGuizmo::ViewManipulate(glm::value_ptr(viewMatrix), 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);


	// Re-decompose new model matrix again once operation is complete
	float finalTranslation[3], finalRotation[3], finalScale[3];
	ImGuizmo::DecomposeMatrixToComponents(modelMatrix, finalTranslation, finalRotation, finalScale);

	return { glm::make_vec3(finalTranslation), glm::make_vec3(finalRotation), glm::make_vec3(finalScale) };
}

void cap::Editor::Render()
{
	m_DebugTerminal->Render();
	pog::Time::TimeScale = m_IsEditorInPlayMode ? 1.0f : 0.0f;
	PanelAndButton();
	EditorControls();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cap::Editor::PanelAndButton()
{
	const char* currentMode = m_IsEditorInPlayMode ? "Pause" : "Play";
	ImGui::Begin("Action Panel");
	ImGui::SetWindowPos(ImVec2(playWindowX, playWindowY), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(rightColumnWidth, playWindowHeight), ImGuiCond_Once);
	if (ButtonCenteredOnLine(currentMode))
	{
		m_IsEditorInPlayMode = !m_IsEditorInPlayMode;
		core::SignalEvent("OnToggleEngineMode");
	}
	if (ButtonCenteredOnLine("Stop") && m_IsEditorInPlayMode)
	{
		m_IsEditorInPlayMode = !m_IsEditorInPlayMode;
		core::SignalEvent("OnToggleEngineMode");
		core::SignalEvent("ResetLevel");
	}
	if (ButtonCenteredOnLine("Save") && !m_IsEditorInPlayMode)
	{
		sad::LevelManager::ExportLevel();
	}
	ImGui::End();

	// Really scuffed way to list entities, included mainly for debugging
	// Cycles through available names and lists them in the editor
	ImGui::Begin("Scuffed Entity List");
	ImGui::SetWindowPos(ImVec2(listWindowX, listWindowY), ImGuiCond_Once);
	ImGui::SetWindowSize(ImVec2(rightColumnWidth, listWindowHeight), ImGuiCond_Once);
	auto view = sad::ecs::Registry::GetEntityWorld().view<sad::ecs::NameComponent>();
	for (auto [entity, name] : view.each())
		ImGui::Text(name.m_Name.c_str());
	ImGui::End();
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

/**
 * @brief 
 * Controls
 * ctrl+s : Save state
 * ctrl+b : Switch editor mode
*/
void cap::Editor::EditorControls()
{
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
	{
		if (!m_IsEditorInPlayMode && ImGui::IsKeyPressed(ImGuiKey_S))
		{
			sad::LevelManager::ExportLevel();
			m_IsEditorInPlayMode = false;
			core::Log(ELogType::Trace, "Saved");
		}

		if (ImGui::IsKeyPressed(ImGuiKey_P))
		{
			m_IsEditorInPlayMode = !m_IsEditorInPlayMode;
			core::SignalEvent("OnToggleEngineMode");
		}
	}
}

void cap::Editor::Teardown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
