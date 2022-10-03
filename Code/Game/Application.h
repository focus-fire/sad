#pragma once

#include <Engine/Application.h>

namespace cry
{
	class Application final : public sad::Application
	{
	public:
		Application();
		~Application() override;

		void Teardown() override;
	};
}