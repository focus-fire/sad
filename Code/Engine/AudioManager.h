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

        static AudioManager& GetInstance();

        static void PlaySFX(AudioResource* audioResource);
        static void PlayMusic(AudioResource* audioResource, int times);
        static void SetVolume(AudioResource* audioResource, int volume);


    private:
        AudioManager() {}

    };
}
