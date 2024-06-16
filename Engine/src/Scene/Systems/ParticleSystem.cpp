#include <pch.h>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "Scene/Systems/ParticleSystem.h"


namespace Cober {

	ParticleSystem::ParticleSystem()
	{
		LOG_INFO("Particle System Added to Registry!!");
	}


	ParticleSystem::~ParticleSystem()
	{
		LOG_INFO("Particle System Removed from Registry");
	}


	void ParticleSystem::Start(Scene* scene)
	{
        auto view = scene->GetAllEntitiesWith<ParticleEmitterComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );

            auto& particleEmitter = entity.GetComponent<ParticleEmitterComponent>();
            particleEmitter.InitParticlesPool();
        }

		LOG_INFO("Particle System Started!!");
	}
    

	void ParticleSystem::Update(Unique<Timestep>& ts, Scene* scene)
	{
		auto view = scene->GetAllEntitiesWith<ParticleEmitterComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );

            auto& particleEmitter =  entity.GetComponent<ParticleEmitterComponent>();
            if (Input::IsKeyPressed(KeyCode::P))
            {
                // Emit Particles of the pool
                particleEmitter.InitParticlesPool();
                for (auto& particle : particleEmitter.particlePool)
                {
                    Emit(ts, particle, particleEmitter);
                }
            }

            // UPDATE
            if (particleEmitter.active == false)
                continue;

            if (particleEmitter.loop == false)
            {
                if (particleEmitter.lifeRemaining >= 0.0f)
                {
                    particleEmitter.lifeRemaining -= ts->GetConsistentTimer();
                }
                else if (particleEmitter.lifeRemaining < 0.0f)
                {
                    particleEmitter.active == false;
                    continue;
                }
            }

            // RENDER
            for (auto& particle : particleEmitter.particlePool)
            {
                if (particle.lifeRemaining < 0.0f)
                {
                    particleEmitter.RemoveParticle();
                    continue;
                }
                particle.lifeRemaining -= ts->GetConsistentTimer();
                particle.position += particle.velocity * (float)ts->GetDeltaTime();
                particle.rotation += 0.01 * (float)ts->GetDeltaTime();

                // Fade away particle
                float life = particle.lifeRemaining / particle.lifeTime;
                glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
                color.a = color.a * life;

                float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, 0.0f })
                    * glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f })
                    * glm::scale(glm::mat4(1.0f), { size, size, 1.0f });

                Render2D::DrawQuad(transform, color);
            }
		}
	}


	void ParticleSystem::Emit(Unique<Timestep>& ts, Particle& particle,  ParticleEmitterComponent& particleEmitter)
    {
        float randomX = Random::Value(0, 10);
        float randomY = Random::Value(0, 10);
        particle.velocity.x = particleEmitter.velocityVariation.x * (randomX / 10 - 0.5f);
        particle.velocity.y = particleEmitter.velocityVariation.y * (randomY / 10 - 0.5f);

        particle.colorBegin = particleEmitter.colorBegin;
        particle.colorEnd = particleEmitter.colorEnd;

        particle.lifeTime = particleEmitter.lifeTime;
	    particle.lifeRemaining = particleEmitter.lifeTime;

        particle.sizeBegin = particleEmitter.sizeBegin + particleEmitter.sizeVariation * ((float)Random::Value(0, 10) / 10 - 0.5f);
        particle.sizeEnd = particleEmitter.sizeEnd;
    }
};