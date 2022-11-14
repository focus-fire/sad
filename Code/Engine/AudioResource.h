#pragma once

#include "Resource.h"

#include <SDL2/SDL_mixer.h>

namespace sad
{
	/**
	 * @brief Representation for an audio file in the engine
	*/
	class AudioResource final : public Resource
	{
	public:
		/**
		 * @brief Denotes the type of audio file conatined by the AudioResource
		*/
		enum class EAudioType
		{
			None,
			WAV,
			MP3
		};

		AudioResource();
		~AudioResource();

		explicit AudioResource(const Resource::ResourceData& resourceData, EAudioType audioType);

		int DefaultVolume;
		EAudioType AudioType;
		Mix_Chunk* MixChunk;
		Mix_Music* MixMusic;
	};
}
