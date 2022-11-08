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
		// Go to Docs folder and use ApplicationFileDefault.md to put back the test case
	};
}