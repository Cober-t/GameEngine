#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
// #include "Events/EventHandler.h"

#include <b2World.h>
#include <b2Body.h>
#include <b2Fixture.h>
#include <b2PolygonShape.h>
#include <b2Draw.h>


namespace Cober {

	class DebugSystem;

	class PhysicsSystem2D : public System 
    {
	public:
		PhysicsSystem2D() = default;
		PhysicsSystem2D(Scene* scene);
		~PhysicsSystem2D();

		void Start();

		void InitEntityPhysics(Entity entity);

		void Update(Timestep ts);
        
        virtual std::vector<Entity> GetSystemEntities() const override;

		//void OnEvent(Unique<EventHandler>& eventHandler);
	
	private:
		b2World* m_PhysicsWorld = nullptr;
		Scene* m_Scene = nullptr;
	};
}

#endif