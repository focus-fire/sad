#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <Core/Log.h>

int main(int argc, char* argv[])
{
	// Setup
	core::InitializeLogging();

	Catch::Session session;
	session.applyCommandLine(argc, argv);
	int result = session.run();

	// Teardown 
	core::KillLogging();

	return result;
}
