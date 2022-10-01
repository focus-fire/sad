#include <spdlog/spdlog.h>

#include <Core/Log.h>
#include <Game/Application.h>
#include <Engine/Application.h>

int main(int argc, char* argv[])
{
    core::InitializeLogging();
    core::Log(ELogType::Info, "this engine is sad.");
    core::Log(ELogType::Assert, "raised assert");

    sad::Game::Application* app = new sad::Game::Application();
    app->Start();
    delete app;

    core::Log(ELogType::Info, "this engine is dead.");
    core::KillLogging();

    return 0;
}