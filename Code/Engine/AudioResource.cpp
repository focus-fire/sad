#include "sadpch.h"

#include "AudioResource.h"

sad::AudioResource::AudioResource(const IResource::ResourceData& resourceData, EAudioType audioType)
	: IResource(resourceData)
	, AudioType(audioType)
	, Volume(0)
{
	// TODO: Implement AudioManager::LoadAudioFile logic here with SDL_Mixer initialization code...
}