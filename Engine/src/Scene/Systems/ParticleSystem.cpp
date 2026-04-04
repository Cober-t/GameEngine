#include <pch.h>

#include "Render/ParticleEmitter.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
#include "Scene/Systems/ParticleSystem.h"

namespace Cober 
{
	// --------------------------------------------------------------------------------------

	ParticleSystem::ParticleSystem()
	{
		ParticleEmitter::ForceCleanUPParticlePool();
		LOG_INFO("Particle System Added to Registry!!");
	}

	// --------------------------------------------------------------------------------------

	ParticleSystem::~ParticleSystem()
	{
		ParticleEmitter::ForceCleanUPParticlePool();
		LOG_INFO("Particle System Removed from Registry");
	}

	// --------------------------------------------------------------------------------------

	void ParticleSystem::Start(Scene* scene)
	{
        auto particleGroup = scene->GetAllEntitiesWith<ParticleEmitterComponent>();

        for (auto& entityHandle : particleGroup)
        {
            auto& particleEmitter = particleGroup.get<ParticleEmitterComponent>(entityHandle);
            particleEmitter.InitDefaultParticle();
        }

		LOG_INFO("Particle System Started!!");
	}
    
	// --------------------------------------------------------------------------------------

	void ParticleSystem::Update(const Timestep& ts, Scene* scene)
	{
		auto particleGroup = scene->GetAllEntitiesWith<ParticleEmitterComponent>();

        for (auto& entityHandle : particleGroup) 
		{
            Entity entity(entityHandle, scene );
            ParticleEmitter::Update(ts, entity);
		}
		ParticleEmitter::Render();
	}
};