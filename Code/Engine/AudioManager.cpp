#include "sadpch.h"
#include "AudioManager.h"

std::list<sad::AudioFile> sad::AudioManager::m_AudioFiles;

void sad::AudioManager::loadAudioFile(const std::string& filename, const std::string& path, int volume, AudioType type)
{
    AudioFile audioFile;
    audioFile.filename = filename;
    audioFile.path = path.c_str();
    audioFile.type = type;
    audioFile.volume = volume;

    core::Log(ELogType::Debug, "Audio Path: {}", path.c_str()); // Debug Path

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0) // Check if audio file can be opened
    {
        core::Log(ELogType::Debug, "Couldn't open audio sample: {}", Mix_GetError());
    }
    else
    {
        if (type == MP3)
        {
            audioFile.mixMusic = Mix_LoadMUS(path.c_str());
            if (audioFile.mixMusic != NULL)
            {
                core::Log(ELogType::Debug, "Audio sample loaded!");
                Mix_VolumeMusic(volume);

                m_AudioFiles.push_back(audioFile);
            }
            else
            {
                core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
            }
        }

        if (type == WAV)
        {
            audioFile.mixChunk = Mix_LoadWAV(path.c_str());
            if (audioFile.mixChunk != NULL)
            {
                core::Log(ELogType::Debug, "Audio sample loaded!");
                Mix_VolumeChunk(audioFile.mixChunk, volume);
                
                m_AudioFiles.push_back(audioFile);
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
void sad::AudioManager::playSFX(std::string filename)
{
    for (std::list<AudioFile>::iterator it = m_AudioFiles.begin(); it != m_AudioFiles.end(); ++it) 
    {
        if (it->filename.compare(filename) == 0)
        {
            if (Mix_Playing(-1) == 0)
            {
                Mix_PlayChannel(-1, it->mixChunk, 0);
            }
            break;
        }
    }
}

// times = 0 for infinite loop
void sad::AudioManager::playMusic(std::string filename, int times) 
{
    for (std::list<AudioFile>::iterator it = m_AudioFiles.begin(); it != m_AudioFiles.end(); ++it)
    {
        if (it->filename.compare(filename) == 0)
        {
            Mix_PlayMusic(it->mixMusic, times - 1);
            break;
        }
    }
}

void sad::AudioManager::set_volume(std::string filename, int volume)
{
    for (std::list<AudioFile>::iterator it = m_AudioFiles.begin(); it != m_AudioFiles.end(); ++it)
    {
        if (it->filename.compare(filename) == 0)
        {
            if (it->type == WAV) Mix_VolumeChunk(it->mixChunk, volume);
            if (it->type == MP3) Mix_VolumeMusic(volume);
            break;
        }
    }
}

sad::AudioManager& sad::AudioManager::GetInstance()
{
    static AudioManager instance;
    return instance;
}