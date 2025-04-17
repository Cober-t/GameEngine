#include <pch.h>
#include "Scene/Systems/AudioSystem.h"


namespace Cober {

	AudioSystem::AudioSystem()
	{
		LOG_INFO("Audio System Added to Registry!!");
	}


	AudioSystem::~AudioSystem()
	{
        Audio::Exit();
		LOG_INFO("Audio System Removed from Registry");
	}


	void AudioSystem::Start(Scene* scene)
	{
        Audio::Init();

		auto view = scene->GetAllEntitiesWith<AudioComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );

			if (std::filesystem::exists(entity.GetComponent<AudioComponent>().audioPath))
				Audio::LoadSound(entity.GetComponent<AudioComponent>().audioName);
		}
		LOG_INFO("Audio System Started!!");
	}

	void AudioSystem::Update(Scene* scene)
	{
		auto view = scene->GetAllEntitiesWith<AudioComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );

			if (std::filesystem::exists(entity.GetComponent<AudioComponent>().audioPath))
			{
				// Only TEST
				// Audio::PlaySound(entity.GetComponent<AudioComponent>().audioName);
			}
		}
	}
}