#pragma once

#include <string>
#include <memory>
#include <SDL2/SDL_mixer.h>

class AudioManager {
public:
    enum AudioType { MP3, WAV };
    AudioManager(const std::string &path, int volume, AudioType type);
    void play();
    void playLoop();
    void play(int times);
    void set_volume(int volume);

private:
    Mix_Music *m_Music = NULL;
    Mix_Chunk *m_Sound = NULL;
};