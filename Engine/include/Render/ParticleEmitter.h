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
        static void Update(Unique<Timestep>& ts, Entity& entity);
        static void Render();
        
        static void Emit(Scene* scene);
        static void Emit(ParticleEmitterComponent& particleEmitter);

        static void CleanUpParticlePool(Unique<Timestep>& ts, ParticleEmitterComponent& particleEmitter);
        static void ForceCleanUPParticlePool();

    private:
        static std::vector<Particle> m_ParticlePool;
    };
}