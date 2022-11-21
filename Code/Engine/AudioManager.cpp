#include "sadpch.h"

#include "AudioManager.h"

sad::AudioManager& sad::AudioManager::GetInstance()
{
    static AudioManager instance;
    return instance;
}

// -1 here means we let SDL_mixer pick the first channel that is free
// If no channel is free it'll return an err code.
void sad::AudioManager::PlaySFX(AudioResource* audioResource)
{
    if (Mix_Playing(-1) == 0)
    {
        Mix_PlayChannel(-1, audioResource->MixChunk, 0);
    }
}

// times = 0 for infinite loop
void sad::AudioManager::PlayMusic(AudioResource* audioResource, int times) 
{
    if (Mix_Playing(-1) == 0)
    {
        Mix_PlayMusic(audioResource->MixMusic, times - 1);
    }
}

void sad::AudioManager::SetVolume(AudioResource* audioResource, int volume)
{
    if (audioResource->AudioType == AudioResource::EAudioType::WAV)
    {
        Mix_VolumeChunk(audioResource->MixChunk, volume);
    }

    if (audioResource->AudioType == AudioResource::EAudioType::MP3)
    {
        Mix_VolumeMusic(volume);
    }
}