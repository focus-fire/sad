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
	};
}