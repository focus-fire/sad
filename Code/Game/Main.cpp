#include "sadpch.h"

#include <spdlog/spdlog.h>

#include <Game/Application.h>
#include <Engine/Application.h>

int main(int argc, char* argv[])
{
    core::InitializeLogging();
    core::Log(ELogType::Info, "this engine is sad.");

    sad::Game::Application* app = new sad::Game::Application();
    app->Start();
    delete app;

    core::Log(ELogType::Info, "this engine is dead.");
    core::KillLogging();

    return 0;
}