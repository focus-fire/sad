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
        std::string filename;
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

        static void loadAudioFile(const std::string& filename, const std::string& path, int volume, AudioType type);

        static void playSFX(std::string filename);

        static void playMusic(std::string filename, int times);

        static void set_volume(std::string filename, int volume);

        static AudioManager& GetInstance();

    private:
        AudioManager() {}

        static std::list<AudioFile> m_AudioFiles;

    };
}
