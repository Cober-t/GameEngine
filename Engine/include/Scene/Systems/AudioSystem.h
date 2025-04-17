#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include "Scene/Scene.h"
#include "Audio/Audio.h"


namespace Cober {

	class CB_API AudioSystem : public System 
    {
	public:
		AudioSystem();
		~AudioSystem();

		void Start(Scene* scene);
		void Update(Scene* scene);
        
		//void OnEvent(Event& event);
	
	private:
	};
}

#endif