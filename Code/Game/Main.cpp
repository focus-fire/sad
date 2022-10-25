#include "sadpch.h"

#include <spdlog/spdlog.h>

#define SDL_MAIN_HANDLED

#include <Game/Application.h>
#include <Engine/Application.h>

#include <Engine/JsonManager.h>

int main(int argc, char* argv[])
{
    core::InitializeLogging();
    core::Log(ELogType::Info, "this engine is sad.");

    pog::Application* app = new pog::Application();
    app->Start();
    delete app;

    core::Log(ELogType::Info, "this engine is dead.");
    core::KillLogging();

    return 0;
}