#include <pch.h>
#include "Scene/Systems/AudioSystem.h"
#include "Audio/Audio.h"

#include "Scene/ECS.h"
#include "Scene/Scene.h"

namespace Cober 
{
	// --------------------------------------------------------------------------------------

	AudioSystem::AudioSystem()
	{
		LOG_INFO("Audio System Added to Registry!!");
	}

	// --------------------------------------------------------------------------------------

	AudioSystem::~AudioSystem()
	{
        Audio::Exit();
		LOG_INFO("Audio System Removed from Registry");
	}

	// --------------------------------------------------------------------------------------

	void AudioSystem::Start(Scene* scene)
	{
        Audio::Init();

		auto audioGroup = scene->GetAllEntitiesWith<AudioComponent>();

        for (auto& entityHandle : audioGroup)
        {
            auto& audio = audioGroup.get<AudioComponent>(entityHandle);

			if (!std::filesystem::exists(audio.audioPath)) 
			{
				LOG_CORE_WARNING("Audio file not found: {0}", audio.audioPath.string());
			}
			Audio::LoadSound(audio.audioName);
		}

		LOG_INFO("Audio System Started!!");
	}

	// --------------------------------------------------------------------------------------

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

	// --------------------------------------------------------------------------------------
}