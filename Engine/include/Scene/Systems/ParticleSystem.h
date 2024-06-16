#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "Scene/Scene.h"


namespace Cober {

	class CB_API ParticleSystem : public System 
    {
	public:
		ParticleSystem();
		~ParticleSystem();

		void Start(Scene* scene);
		void Update(Unique<Timestep>& ts, Scene* scene);

        void Emit(ParticleEmitterComponent& particleEmitter);
	};
}

#endif