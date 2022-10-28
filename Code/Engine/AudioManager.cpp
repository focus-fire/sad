#include "sadpch.h"
#include "AudioManager.h"

AudioManager::AudioManager(const std::string &path, int volume)
    : chunk(Mix_LoadWAV(path.c_str()), Mix_FreeChunk) {

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        if (!chunk.get()) {
            // LOG("Couldn't load audio sample: ", path);
            core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
        }
    }

    Mix_AllocateChannels(32);
   
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
