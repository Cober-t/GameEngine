#include <pch.h>
#include "Scene/Systems/AudioSystem.h"
#include "Audio/Audio.h"

#include "Scene/ECS.h"
#include "Scene/Scene.h"

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
		// CB_PROFILE_FUNCTION();

        auto& registry = *scene->GetRegistry();
        auto group = registry.group<>(entt::get<AudioComponent>);

        for (auto entityHandle : group)
        {
            auto& audio = group.get<AudioComponent>(entityHandle);

            // runtime logic only
            // if (audio.playOnStart && !audio.started) { ... }
        }
	}
}