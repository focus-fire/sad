#pragma once

#include <Engine/Application.h>
#include <Engine/RenderableResource.h>

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
		// Temporary Variables for Test Level
<<<<<<< HEAD
		sad::RenderableResource* m_CubeResource;
		sad::RenderableResource::Geometry m_CubeGeometry;

		sad::ecs::Entity m_FirstCubeEntity;
		sad::ecs::Entity m_SecondCubeEntity;

		std::mutex m_GameMutex;

		float m_CubeTranslate;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
=======
		// Go to Docs folder and use ApplicationFileDefault.md to put back the test case
>>>>>>> dev
	};
}