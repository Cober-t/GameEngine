#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H
#endif

#include <unordered_map>
#include <vector>

#include "Core/Utils.h"
#include "Scene/Components.h"

class Render2D;

namespace Cober {

    class CB_API ParticleEmitter
    {
    public:
        static bool Update(Unique<Timestep>& ts, Entity& entity, ParticleEmitterComponent& particleEmitter);
        static void Render(Entity& entity);
        static void Emit(Scene* scene);
        static void Emit(ParticleEmitterComponent& particleEmitter);

        static Particle& GenParticle(ParticleEmitterComponent& particleEmitter);
        static void RemoveParticle(Particle& newParticle, ParticleEmitterComponent& particleEmitter);
        static void CleanUpParticlePool(ParticleEmitterComponent& particleEmitter);
    };
}