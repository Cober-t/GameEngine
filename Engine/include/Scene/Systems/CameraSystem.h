#ifndef CAMERA_SYSTEM_H
#define CAMERA_SYSTEM_h


#include "Core/Core.h"
#include "Render/Camera/EditorCamera.h"
#include "Render/Camera/GameCamera.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
// #include "Events/EventHandler.h"


namespace Cober {

	class CB_API CameraSystem : public System
	{
	public:
		CameraSystem();
		~CameraSystem();

		void Start(Scene* scene);

		void Update(Unique<Timestep>& ts, Ref<Camera>& defaultCamera, Scene* scene);
		void OnEvent(Event& event, const Ref<Camera>& camera);
		// void UpdateRuntime(Unique<Timestep>& ts, Ref<Camera>& defaultCamera, Scene* scene);
		// void UpdateSimulation(Unique<Timestep>& ts, const Ref<Camera>& defaultCamera, Scene* scene);

		//void OnEvent(Unique<EventHandler>& eventHandler);
	private:
	};	
}

#endif