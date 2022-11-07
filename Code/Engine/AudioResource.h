#pragma once

#include "IResource.h"

namespace sad
{
	class AudioResource final : public IResource
	{
	public:
		enum class EAudioType
		{
			WAV,
			MP3
		};

		explicit AudioResource(const IResource::ResourceData& resourceData, EAudioType audioType);

		EAudioType AudioType;

		// TODO: Should audio resources hold volumes?
		int Volume;
	};
}
