#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

namespace Cober {

	class Scene;

	class CB_API ParticleSystem : public System 
    {
	public:
		ParticleSystem();
		~ParticleSystem() override;

		void Start(Scene* scene);
		void Update(const Timestep& ts, Scene* scene);
	};
}

#endif