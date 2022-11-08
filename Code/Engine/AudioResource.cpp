#include "sadpch.h"

#include "AudioResource.h"


sad::AudioResource::AudioResource(const IResource::ResourceData& resourceData, EAudioType audioType)
	: IResource(resourceData)
	, AudioType(audioType)
	, DefaultVolume(10)
{
    const std::string absolutePath = core::FileUtils::GetPathInsideDataDirectory(resourceData.FilePath);
	// TODO: Implement AudioManager::LoadAudioFile logic here with SDL_Mixer initialization code...
    if (audioType == EAudioType::MP3)
    {
        MixMusic = Mix_LoadMUS(absolutePath.c_str());
        if (MixMusic != NULL)
        {
            core::Log(ELogType::Debug, "Audio sample loaded!");
            Mix_VolumeMusic(DefaultVolume);
        }
        else
        {
            core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
        }
    }

    if (audioType == EAudioType::WAV)
    {
        MixChunk = Mix_LoadWAV(absolutePath.c_str());
        if (MixChunk != NULL)
        {
            core::Log(ELogType::Debug, "Audio sample loaded!");
            Mix_VolumeChunk(MixChunk, DefaultVolume);
        }
        else
        {
            core::Log(ELogType::Debug, "Couldn't load audio sample: {}", Mix_GetError());
        }
    }
}