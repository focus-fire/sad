#include "sadpch.h"

#include "Application.h"

pog::Application::Application()
	: sad::Application()
{
	// Game can be initialized here
}

pog::Application::~Application()
{
	// Game is destroyed here
}

void pog::Application::Update(float dt)
{
	sad::Application::Update(dt);

	// Game specific update logic goes here
}

void pog::Application::Teardown()
{
	sad::Application::Teardown();

	// Game specific teardown logic goes here
}
