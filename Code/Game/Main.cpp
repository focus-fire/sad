#include "sadpch.h"

#include <SDL2/SDL_mixer.h>

#include <Engine/AudioManager.h>

#include <spdlog/spdlog.h>

#define SDL_MAIN_HANDLED

#include <Game/Application.h>
#include <Engine/Application.h>

int main(int argc, char* argv[])
{
    core::InitializeLogging();
    core::Log(ELogType::Info, "this engine is sad.");

    AudioManager* audioManager = new AudioManager("C:\\Users\\keith\\Documents\\GitHub\\sad\\Code\\Arabic_Nokia.wav", 100);
    audioManager->play();

    pog::Application* app = new pog::Application();
    app->Start();
    delete app;

    core::Log(ELogType::Info, "this engine is dead.");
    core::KillLogging();

    return 0;
}