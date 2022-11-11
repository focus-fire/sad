#pragma once

#include "Resource.h"

#include <SDL2/SDL_mixer.h>

namespace sad
{
	class AudioResource final : public Resource
	{
	public:
		enum class EAudioType
		{
			WAV,
			MP3
		};

		AudioResource() { }
		explicit AudioResource(const Resource::ResourceData& resourceData, EAudioType audioType);

		EAudioType AudioType;

		// TODO: Should audio resources hold volumes?
		int DefaultVolume;
		Mix_Chunk* MixChunk;
		Mix_Music* MixMusic;
	};
}
