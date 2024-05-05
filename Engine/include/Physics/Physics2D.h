#ifndef PHYSICS_2D
#define PHYSICS_2D

#include <b2_world.h>
#include <b2_contact.h>

#include "Physics/Debug2DPhysics.h"

namespace Cober {

    class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
		
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	};


    class Physics2D
    {
    public:

        static void Init();
        static void DebugDraw();
        static void Step(float timeStep, int velocityIterations, int positionIterations);

        inline static void SetDrawDebug(b2Draw* debugDraw) { m_PhysicsWorld->SetDebugDraw(debugDraw); };
        inline static b2Body* CreateBody(b2BodyDef body) { return m_PhysicsWorld->CreateBody(&body); }

        inline static b2World& PhysicsWorld() {return *m_PhysicsWorld; }

    private:
        static void CreateWorld(const b2World* physicsWorld);
        
    private:
        static b2World* m_PhysicsWorld;
        static bool m_DebugActive;
    };
}

#endif