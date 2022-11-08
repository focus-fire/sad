#pragma once

#include <string>
#include <memory>
#include <list>

#include <SDL2/SDL_mixer.h>

#include "AudioResource.h"

namespace sad 
{
    class AudioManager 
    {
    public:
        AudioManager(const AudioManager&) = delete;

        static void PlaySFX(const AudioResource& audioResource);

        static void PlayMusic(const AudioResource& audioResource, int times);

        static void SetVolume(const AudioResource& audioResource, int volume);

        static AudioManager& GetInstance();

    private:
        AudioManager() {}

    };
}
