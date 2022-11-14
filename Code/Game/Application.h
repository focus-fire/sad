#pragma once

#include <Core/Memory.h>

#include <Engine/Application.h>
#include <Engine/RenderableResource.h>
#include <Engine/AudioResource.h>

#include <Engine/ECS/Entity.h>

namespace pog
{
	class Application final : public sad::Application
	{
	public:
		explicit Application();
		~Application() override;

		void Start() override;

		void Update(float dt) override;
		void Teardown() override;

	private:
		std::mutex m_GameMutex;
		// Temporary Variables for Test Level
		sad::RenderableResource* m_CubeResource;
		sad::RenderableResource::Geometry m_CubeGeometry;

		//sad::ecs::Entity m_FirstCubeEntity;
		//sad::ecs::Entity m_SecondCubeEntity;
		//sad::ecs::Entity m_GameCamera;

		float m_CubeTranslate;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;

		sad::AudioResource* m_CollisionSoundEffect;
	};
}