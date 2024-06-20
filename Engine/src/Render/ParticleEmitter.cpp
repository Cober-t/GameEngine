#include <pch.h>

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "Render/ParticleEmitter.h"

namespace Cober {

    std::vector<Particle> ParticleEmitter::m_ParticlePool;

    
    void ParticleEmitter::Update(Unique<Timestep>& ts, Entity& entity)
    {
        auto& particleEmitter = entity.GetComponent<ParticleEmitterComponent>();

        if (particleEmitter.active == false)
            return;

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

        CleanUpParticlePool(ts, entity.GetComponent<ParticleEmitterComponent>());

        particleEmitter.position.x = entity.GetComponent<TransformComponent>().position.x;
        particleEmitter.position.y = entity.GetComponent<TransformComponent>().position.y;
    }
    

    void ParticleEmitter::Render()
    {
        for (std::vector<Particle>::reverse_iterator particle = m_ParticlePool.rbegin(); particle != m_ParticlePool.rend(); ++particle)
        {
            // Fade away particle
            float life = particle->lifeRemaining / particle->lifeTime;
            glm::vec4 color = glm::lerp(particle->colorEnd, particle->colorBegin, life);
            color.a = color.a * life;

            float size = glm::lerp(particle->sizeEnd, particle->sizeBegin, life);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle->position.x, particle->position.y, 0.0f })
                * glm::rotate(glm::mat4(1.0f), particle->rotation, { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { size, size, 1.0f });
    
            if (particle->subTexture)
                Render2D::DrawQuad(transform, color, particle->subTexture);
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

        int sizeBeforeAddNewParticles = m_ParticlePool.size();
        m_ParticlePool.resize(m_ParticlePool.size() + particleEmitter.rate);
            
        for (int i = sizeBeforeAddNewParticles; i < m_ParticlePool.size(); i++)
        {
            m_ParticlePool[i].position = particleEmitter.position;
            if (particleEmitter.positionVariation.x != 0)
            {
                float variation = Random::Value(0, abs(particleEmitter.positionVariation.x)) / 10 / 2;
                if (Random::Value(0, 1) == 0)
                    variation *= -1;

                m_ParticlePool[i].position.x += variation;
            }
            if (particleEmitter.positionVariation.y != 0)
            {
                float variation = Random::Value(0, abs(particleEmitter.positionVariation.y)) / 10 / 2;
                if (Random::Value(0, 1) == 0)
                    variation *= -1;
                
                m_ParticlePool[i].position.y += variation;
            }

            m_ParticlePool[i].velocity = particleEmitter.velocity;
            m_ParticlePool[i].velocity.x += particleEmitter.velocityVariation.x * (Random::Value(0, 10) / 10 - 0.5f);
            m_ParticlePool[i].velocity.y += particleEmitter.velocityVariation.y * (Random::Value(0, 10) / 10 - 0.5f);

            m_ParticlePool[i].colorBegin = particleEmitter.colorBegin;
            m_ParticlePool[i].colorEnd = particleEmitter.colorEnd;

            m_ParticlePool[i].lifeTime = particleEmitter.lifeTime;
            m_ParticlePool[i].lifeRemaining = particleEmitter.lifeTime;

            m_ParticlePool[i].sizeBegin = particleEmitter.sizeBegin + particleEmitter.sizeVariation * ((float)Random::Value(0, 10) / 10 - 0.5f);
            m_ParticlePool[i].sizeEnd = particleEmitter.sizeEnd;

            m_ParticlePool[i].loop = particleEmitter.loop;

            if (particleEmitter.texture)
                m_ParticlePool[i].subTexture = particleEmitter.subTexture;
        }
    }


    void ParticleEmitter::CleanUpParticlePool(Unique<Timestep>& ts, ParticleEmitterComponent& particleEmitter)
    {

        int particlesCount = m_ParticlePool.size();
        for (int i = 0; i < m_ParticlePool.size(); i++)
        {
            if (m_ParticlePool[i].lifeRemaining < 0.0f)
            {   
                m_ParticlePool.erase(m_ParticlePool.begin() + i--);
                continue;
            }
            m_ParticlePool[i].lifeRemaining -= ts->GetConsistentTimer();
            m_ParticlePool[i].position += m_ParticlePool[i].velocity * (float)ts->GetDeltaTime();
            m_ParticlePool[i].rotation += particleEmitter.rotation / 10 * (float)ts->GetDeltaTime();
        }
    }


    void ParticleEmitter::ForceCleanUPParticlePool()
    {
        m_ParticlePool.clear();
    }
}