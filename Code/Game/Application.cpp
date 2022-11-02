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

void pog::Application::Teardown()
{
	// Game is shut down here
}
