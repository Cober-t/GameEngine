#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_h


#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Render/Camera/GameCamera.h"
#include "Render/Render2D.h"
#include "Render/RenderGlobals.h"
#include "Scene/Scene.h"
// #include "Events/EventHandler.h"


namespace Cober {

	class RenderSystem : public System {
	public:
		RenderSystem() = default;
		RenderSystem(Scene* scene);
		~RenderSystem();

		void Start();

		void Update(Timestep ts, const Ref<GameCamera>& camera);

		virtual std::vector<Entity> GetSystemEntities() const override;

		//void OnEvent(Unique<EventHandler>& eventHandler);
		
	private:
		Scene* m_Scene = nullptr;
	};	
}

#endif