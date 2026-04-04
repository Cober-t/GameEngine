#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include "Scene/ECS.h"

namespace Cober {

	class Scene;

	class CB_API AudioSystem : public System 
    {
	public:
		AudioSystem();
		~AudioSystem() override;

		void Start(Scene* scene);
		void Update(Scene* scene);
        
		//void OnEvent(Event& event);
	
	private:
	};
}

#endif