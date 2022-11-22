#include "sadpch.h"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "backends/imgui_impl_glfw.h"

#include "Engine/Application.h"
#include <Engine/ECS/Components/RenderableObjectComponent.h>
#include <Engine/ECS/Components/TransformComponent.h>
#include <Engine/ECS/Components/ScriptComponent.h>

#include "GameCamera.h"


#define GLM_ENABLE_EXPERIMENTAL

sad::GameCamera::GameCamera() : input (InputManager::GetInstance()) {}

void sad::GameCamera::Update()
{

	// Change mouse/camera state
	if (input.GetKey(sad::KeyCode::Escape))
	{
		ToggleMouseState();
	}

	if (isActive)
	{
		// Get current camera state (position) from Player object
		sad::GameCamera::CurrentCameraState();
		double mouse_x, mouse_y;
		mouse_x = input.GetMousePosition().x;
		mouse_y = input.GetMousePosition().y;

		ImGui::SetCursorPos(ImVec2(800.0f, 450.0f));
		input.SetMousePosition(800.0, 450.0);

		// Application shouldnt be accessed in here :/
		SDL_WarpMouseInWindow(sad::Application::s_MainWindow->GetSDLWindow(), 800, 450);

		// Calculate and update camera yaw (horizontal rotation) for the camera and keep rotation between 0 <=> 360
		float delta_x{ static_cast<float>(mouse_x - 800.0) };
		cameraEulers.y -= delta_x * 0.05f;
		if (cameraEulers.y <= 0)
		{
			cameraEulers.y = 360.0f;
		}
		else if (cameraEulers.y >= 360.0f)
		{
			cameraEulers.y = 0.0f;
		}

		// Calculate and update camera pitch (vertical rotation), keeps it between 90 <=> -90
		float delta_y{ static_cast<float>(mouse_y - 450.0) };
		cameraEulers.x = std::max(std::min(cameraEulers.x + delta_y * 0.05f, 45.0f), -45.0f);
	}

}

void sad::GameCamera::CurrentCameraState()
{
	ecs::EntityWorld& world = ecs::Registry::GetEntityWorld();

	// Get all entities with renderableObject, transform and scripting Components
	auto view = world.view<const ecs::RenderableObjectComponent, const ecs::TransformComponent, const ecs::ScriptComponent>();
	for (auto [entity, renderableObjectComponent, transformComponent, scriptingComponent] : view.each())
	{
		std::string entityScriptClassName = scriptingComponent.m_ClassName;

		// Update the game camera position based on the player object's transform and apply rotation to the player object based on camera's yaw
		if (entityScriptClassName == "Player")
		{
			sad::GameCamera::cameraPosition = transformComponent.m_Transform->GetPosition();
			transformComponent.m_Transform->SetRotation(glm::toQuat(glm::orientate3(glm::vec3(0, 0, cameraEulers.y * 0.1f))));
		}
	}
}
