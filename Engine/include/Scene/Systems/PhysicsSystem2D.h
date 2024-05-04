#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
// #include "Events/Event.h"

#include <b2_world.h>
#include <b2_body.h>
#include <b2_fixture.h>
#include <b2_edge_shape.h>
#include <b2_circle_shape.h>
#include <b2_polygon_shape.h>
#include <b2_draw.h>
#include <b2_contact.h>

#include <Render/Primitives/Line.h>
#include <Render/Debug2DPhysics.h>


namespace Cober {

	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
		
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	};


	class PhysicsSystem2D : public System 
    {
	public:
		PhysicsSystem2D() = default;
		PhysicsSystem2D(Scene* scene);
		~PhysicsSystem2D();

		void Start();

		void InitEntityPhysics(Entity entity);

		void Update(Unique<Timestep>& ts);
        
        virtual std::vector<Entity> GetSystemEntities() const override;

		//void OnEvent(Event& event);
	
	private:
		b2World* m_PhysicsWorld = nullptr;
		Scene* m_Scene = nullptr;
	};

	static ContactListener contactListener;
}

#endif