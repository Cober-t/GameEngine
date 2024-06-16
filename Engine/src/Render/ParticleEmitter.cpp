#include <pch.h>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "Render/ParticleEmitter.h"

namespace Cober
{
    bool ParticleEmitter::Update(Unique<Timestep>& ts, Entity& entity, ParticleEmitterComponent& particleEmitter)
    {
        if (particleEmitter.active == false)
            return false;

        if (particleEmitter.loop == false)
        {
            if (particleEmitter.lifeRemaining >= 0.0f)
            {
                particleEmitter.lifeRemaining -= ts->GetConsistentTimer();
            }
            else if (particleEmitter.lifeRemaining < 0.0f)
            {
                particleEmitter.active == false;
            }
        }

        for (auto& particleMap : particleEmitter.particlePool)
        {
            auto& particle = particleMap.second;

            if (particle.lifeRemaining < 0.0f && particle.active)
            {   
                ParticleEmitter::RemoveParticle(particle, particleEmitter);
                continue;
            }
            particle.lifeRemaining -= ts->GetConsistentTimer();
            particle.position += particle.velocity * (float)ts->GetDeltaTime();
            particle.rotation += particleEmitter.rotation / 10 * (float)ts->GetDeltaTime();
        }

        particleEmitter.position.x = entity.GetComponent<TransformComponent>().position.x;
        particleEmitter.position.y = entity.GetComponent<TransformComponent>().position.y;

        return true;
    }
    

    void ParticleEmitter::Render(Entity& entity)
    {
        auto& particleEmitter = entity.GetComponent<ParticleEmitterComponent>();

        for (auto& particleMap : particleEmitter.particlePool)
        {
            auto& particle = particleMap.second;
            // Fade away particle
            float life = particle.lifeRemaining / particle.lifeTime;
            glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);
            color.a = color.a * life;

            float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.position.x, particle.position.y, 0.0f })
                * glm::rotate(glm::mat4(1.0f), particle.rotation, { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
    
            if (entity.HasComponent<Render2DComponent>() && entity.GetComponent<Render2DComponent>().texture)
                Render2D::DrawQuad(transform, color, entity.GetComponent<Render2DComponent>().subTexture);
            else
                Render2D::DrawQuad(transform, color);
        }
    }


    void ParticleEmitter::Emit(Scene* scene)
    {
        auto view = scene->GetAllEntitiesWith<ParticleEmitterComponent>();

        for (auto& entt : view)
        {
            Entity entity = Entity((entt::entity)entt, scene );
            Emit(entity.GetComponent<ParticleEmitterComponent>());
        }
    }


    void ParticleEmitter::Emit(ParticleEmitterComponent& particleEmitter)
    {
        if (particleEmitter.active == false || particleEmitter.loop == false && particleEmitter.lifeRemaining < 0.0f)
            return;
            
        for (int i = 0; i < particleEmitter.rate; i++)
        {
            Particle& particle = GenParticle(particleEmitter);

            particle.position = particleEmitter.position;
            if (particleEmitter.positionVariation.x != 0)
            {
                float variation = Random::Value(0, abs(particleEmitter.positionVariation.x)) / 10 / 2;
                if (Random::Value(0, 1) == 0)
                    variation *= -1;
                particle.position.x += variation;
            }
            if (particleEmitter.positionVariation.y != 0)
            {
                float variation = Random::Value(0, abs(particleEmitter.positionVariation.y)) / 10 / 2;
                if (Random::Value(0, 1) == 0)
                    variation *= -1;
                particle.position.y += variation;
            }

            particle.velocity = particleEmitter.velocity;
            particle.velocity.x += particleEmitter.velocityVariation.x * (Random::Value(0, 10) / 10 - 0.5f);
            particle.velocity.y += particleEmitter.velocityVariation.y * (Random::Value(0, 10) / 10 - 0.5f);

            particle.colorBegin = particleEmitter.colorBegin;
            particle.colorEnd = particleEmitter.colorEnd;

            particle.lifeTime = particleEmitter.lifeTime;
            particle.lifeRemaining = particleEmitter.lifeTime;

            particle.sizeBegin = particleEmitter.sizeBegin + particleEmitter.sizeVariation * ((float)Random::Value(0, 10) / 10 - 0.5f);
            particle.sizeEnd = particleEmitter.sizeEnd;

            particle.loop = particleEmitter.loop;
            particle.active = true;
        }
    }


    Particle& ParticleEmitter::GenParticle(ParticleEmitterComponent& particleEmitter)
    {
        int newIndex = particleEmitter.particlePool.size();
        if (particleEmitter.freeIndices.size() > 0)
        {
            newIndex = particleEmitter.freeIndices.front();
            particleEmitter.freeIndices.erase(particleEmitter.freeIndices.begin());
        }
        Particle particle;
        particleEmitter.particlePool[newIndex] = particle;
        particleEmitter.particlePool[newIndex].index = newIndex;
        return particleEmitter.particlePool[newIndex];
    }


    void ParticleEmitter::RemoveParticle(Particle& newParticle, ParticleEmitterComponent& particleEmitter)
    {
        newParticle.active = false;
        particleEmitter.particlesToBeRemoved.push_back(newParticle);
    }


    void ParticleEmitter::CleanUpParticlePool(ParticleEmitterComponent& particleEmitter)
    {
        for (auto& particle : particleEmitter.particlesToBeRemoved)
        {
            particleEmitter.freeIndices.push_back(particle.index);
            particleEmitter.particlePool.erase(particle.index);
        }
        
        particleEmitter.particlesToBeRemoved.clear();
    }
}