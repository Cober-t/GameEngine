#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_h


#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Render/Camera/GameCamera.h"
#include "Render/Render2D.h"
#include "Render/RenderGlobals.h"
// #include "Events/EventHandler.h"


namespace Cober {

	class RenderSystem : public System {
	public:
		RenderSystem();
		~RenderSystem();

		void Start();

		void Update(Timestep ts, const Ref<GameCamera>& camera);

		//void OnEvent(Unique<EventHandler>& eventHandler);
	};	
}

#endif