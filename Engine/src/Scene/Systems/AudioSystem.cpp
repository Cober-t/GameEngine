#include <pch.h>
#include "Scene/Systems/AudioSystem.h"


namespace Cober {

	AudioSystem::AudioSystem()
	{
		LOG_INFO("Audio System Added to Registry!!");
	}


	AudioSystem::~AudioSystem()
	{
        Audio::Stop();
		LOG_INFO("Audio System Removed from Registry");
	}


	void AudioSystem::Start(Scene* scene)
	{
        Audio::Init();
	}

	void AudioSystem::Update(Scene* scene)
	{
        Audio::Update();
	}
}