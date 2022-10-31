#include "sadpch.h"
#include "AudioManager.h"

AudioManager::AudioManager(const std::string &path, int volume)
    : chunk(Mix_LoadWAV(path.c_str()), Mix_FreeChunk) {
    core::Log(ELogType::Debug, "Path Test {}", path.c_str());
    if( Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
        }
    else {
        core::Log(ELogType::Debug, "Audio sample loaded!");
    }
    Mix_Chunk *test = Mix_LoadWAV("Arabic_Nokia.wav");
    if (test == NULL) {
        core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
    }
    else {
        if (Mix_PlayChannel(-1, test, 0) == -1) {
            core::Log(ELogType::Debug, "It Work?: {}", Mix_GetError());
        }
    }
   
    Mix_VolumeChunk(chunk.get(), volume);
}

// -1 here means we let SDL_mixer pick the first channel that is free
// If no channel is free it'll return an err code.
void AudioManager::play() {
    Mix_PlayChannel(-1, chunk.get(), 0);
}

void AudioManager::play(int times) {
    Mix_PlayChannel(-1, chunk.get(), times - 1);
}

void AudioManager::set_volume(int volume) {
    Mix_VolumeChunk(chunk.get(), volume);
}
