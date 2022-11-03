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

    AudioManager* audioManager = new AudioManager("Arabic_Nokia.wav", 1, AudioManager::WAV);
    audioManager->play();

    AudioManager* audioManager_2 = new AudioManager("lol.mp3", 1, AudioManager::MP3);
    audioManager_2->play();

    pog::Application* app = new pog::Application();
    app->Start();
    delete app;

    core::Log(ELogType::Info, "this engine is dead.");
    core::KillLogging();

    return 0;
}