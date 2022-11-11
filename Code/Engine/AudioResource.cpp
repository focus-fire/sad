#include "sadpch.h"

#include "AudioResource.h"

sad::AudioResource::AudioResource()
    : Resource(Resource::Null())
    , AudioType(EAudioType::None)
    , DefaultVolume(0)
    , MixChunk(nullptr)
    , MixMusic(nullptr)
{ }

sad::AudioResource::~AudioResource()
{ }

sad::AudioResource::AudioResource(const Resource::ResourceData& resourceData, EAudioType audioType)
	: Resource(resourceData)
	, AudioType(audioType)
	, DefaultVolume(10)
    , MixChunk(nullptr)
    , MixMusic(nullptr)
{
    const std::string absolutePath = resourceData.AbsolutePath;

    if (audioType == EAudioType::MP3)
    {
        MixMusic = Mix_LoadMUS(absolutePath.c_str());
        if (MixMusic != NULL)
        {
            Mix_VolumeMusic(DefaultVolume);
        }
        else
        {
            core::Log(ELogType::Debug, "Failed to load audio sample: {}", Mix_GetError());
        }
    }

    if (audioType == EAudioType::WAV)
    {
        MixChunk = Mix_LoadWAV(absolutePath.c_str());
        if (MixChunk != NULL)
        {
            Mix_VolumeChunk(MixChunk, DefaultVolume);
        }
        else
        {
            core::Log(ELogType::Debug, "Failed to load audio sample: {}", Mix_GetError());
        }
    }
}