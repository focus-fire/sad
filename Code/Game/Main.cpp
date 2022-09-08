#include <spdlog/spdlog.h>

#include <Game/Application.h>
#include <Engine/Application.h>

int main(void)
{
    spdlog::info("this engine is sad.");

    sad::Game::Application* app = new sad::Game::Application();
    // Initialize game loop
    delete app;

    spdlog::info("this engine shutdown successfully.");

    return 0;
}