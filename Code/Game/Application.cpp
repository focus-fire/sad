#include "sadpch.h"

#include "Application.h"

cry::Application::Application()
	: sad::Application()
{
	// Game can be initialized here
}

cry::Application::~Application()
{
	// Game is destroyed here
}

void cry::Application::Teardown()
{
	// Game is shut down here
}
