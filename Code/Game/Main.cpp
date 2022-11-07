#include "sadpch.h"

#include <spdlog/spdlog.h>

#define SDL_MAIN_HANDLED

#include <Game/Application.h>
#include <Engine/Application.h>

int main(int argc, char* argv[])
{
    core::InitializeLogging();
    core::Log(ELogType::Info, "this engine is sad.");

    pog::Application* app = new pog::Application();
    app->EngineStart();
    delete app;

    core::Log(ELogType::Info, "this engine is dead.");
    core::KillLogging();

    return 0;
}

#ifdef _SAD_WINDOWS
int WinMain(_In_ HINSTANCE hInstance, 
    _In_opt_ HINSTANCE hPrevInstance, 
    _In_ LPSTR lpCmdLine, 
    _In_ int nShowCmd)
{
    return main(__argc, __argv);
}
#endif
