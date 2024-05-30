#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_h


#include "Core/Core.h"
#include "Render/Camera/GameCamera.h"
#include "Render/Render2D.h"
#include "Render/RenderGlobals.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
// #include "Events/EventHandler.h"


namespace Cober {

	class CB_API RenderSystem : public System
	{
	public:
		RenderSystem();
		~RenderSystem();

		void Start();

		void Update(const Ref<Camera>& camera, Scene* scene);

		//void OnEvent(Unique<EventHandler>& eventHandler);
		
	private:
	};	
}

#endif