#pragma once

#include <Engine/Application.h>

namespace pog
{
	class Application final : public sad::Application
	{
	public:
		explicit Application();
		~Application() override;

		void Update(float dt) override;
		void Teardown() override;
	};
}