#pragma once

#include <string>
#include <memory>
#include <list>

#include <SDL2/SDL_mixer.h>

namespace sad 
{
    enum AudioType { MP3, WAV };

    struct AudioFile
    {
        std::string fileName;
        std::string path;
        AudioType type;
        Mix_Chunk* mixChunk;
        Mix_Music* mixMusic;
        int volume;
    };

    class AudioManager 
    {
    public:
        AudioManager(const AudioManager&) = delete;

        static void LoadAudioFile(const std::string& fileName, const std::string& path, int volume, AudioType type);

        static void PlaySFX(std::string fileName);

        static void PlayMusic(std::string fileName, int times);

        static void SetVolume(std::string fileName, int volume);

        static AudioManager& GetInstance();

    private:
        AudioManager() {}

        static std::list<AudioFile> m_AudioFiles;

    };
}
