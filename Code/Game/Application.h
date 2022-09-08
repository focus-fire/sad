#pragma once

#include <Engine/Application.h>

namespace sad
{
    namespace Game 
    {
        class Application final : public sad::Application
        {
        public:
            Application();
            ~Application() override;
        };
    }
}