#include "sadpch.h"
#include "AudioManager.h"

AudioManager::AudioManager(const std::string &path, int volume, AudioType type)
{
    core::Log(ELogType::Debug, "Audio Path: {}", path.c_str());
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0)
    {
        core::Log(ELogType::Debug, "Couldn't open audio sample: {}", Mix_GetError());
    }
    else
    {
        if (type == MP3) 
        {
            m_Music = Mix_LoadMUS(path.c_str());
            if (m_Music != NULL)
            {
                core::Log(ELogType::Debug, "Audio sample loaded!");
                Mix_VolumeMusic(volume);
            }
            else
            {
                core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
            }
        }

        if (type == WAV)
        {
            m_Sound = Mix_LoadWAV(path.c_str());
            if (m_Sound != NULL)
            {
                core::Log(ELogType::Debug, "Audio sample loaded!");
                Mix_VolumeChunk(m_Sound, volume);
            }
            else
            {
                core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
            }
        }
    }
}

// -1 here means we let SDL_mixer pick the first channel that is free
// If no channel is free it'll return an err code.
void AudioManager::play() 
{
    if (m_Music != NULL) Mix_PlayMusic(m_Music, 0);
    if (m_Sound != NULL) Mix_PlayChannel(-1, m_Sound, 0);
}

void AudioManager::play(int times) 
{
    if (m_Music != NULL) Mix_PlayMusic(m_Music, times - 1);
    if (m_Sound != NULL) Mix_PlayChannel(-1, m_Sound, times - 1);
}

void AudioManager::playLoop()
{
    if (m_Music != NULL) Mix_PlayMusic(m_Music, -1);
    if (m_Sound != NULL) Mix_PlayChannel(-1, m_Sound, -1);
}

void AudioManager::set_volume(int volume) 
{
    if (m_Music != NULL) Mix_VolumeMusic(volume);
    if (m_Sound != NULL) Mix_VolumeChunk(m_Sound, volume);
}
