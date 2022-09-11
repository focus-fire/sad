#include <spdlog/spdlog.h>

#include <Game/Application.h>
#include <Engine/Application.h>

int main(int argc, char* argv[])
{
    spdlog::info("this engine is sad.");

    sad::Game::Application* app = new sad::Game::Application();
    // Initialize game loop
    delete app;

    spdlog::info("this engine shutdown successfully.");

    return 0;
}