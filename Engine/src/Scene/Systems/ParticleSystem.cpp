#include <pch.h>

#include "Render/ParticleEmitter.h"
#include "Scene/Systems/ParticleSystem.h"


namespace Cober {

	ParticleSystem::ParticleSystem()
	{
		ParticleEmitter::ForceCleanUPParticlePool();
		LOG_INFO("Particle System Added to Registry!!");
	}


	ParticleSystem::~ParticleSystem()
	{
		ParticleEmitter::ForceCleanUPParticlePool();
		LOG_INFO("Particle System Removed from Registry");
	}


	void ParticleSystem::Start(Scene* scene)
	{
        auto view = scene->GetAllEntitiesWith<ParticleEmitterComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );

            auto& particleEmitter = entity.GetComponent<ParticleEmitterComponent>();
            particleEmitter.InitDefaultParticle();
        }

		LOG_INFO("Particle System Started!!");
	}
    

	void ParticleSystem::Update(Unique<Timestep>& ts, Scene* scene)
	{
		auto view = scene->GetAllEntitiesWith<ParticleEmitterComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );

            ParticleEmitter::Update(ts, entity);

			ParticleEmitter::Render();
		}
	}
};