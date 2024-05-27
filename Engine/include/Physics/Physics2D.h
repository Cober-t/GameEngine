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


    class CB_API Physics2D
    {
    public:
        static void Init();
        static void DebugDraw();
        static void Step();

        inline static void SetDrawDebug(b2Draw* debugDraw) { m_PhysicsWorld->SetDebugDraw(debugDraw); };
        inline static b2Body* CreateBody(b2BodyDef body) { return m_PhysicsWorld->CreateBody(&body); }

        inline static b2World& PhysicsWorld() {return *m_PhysicsWorld; }
        inline static PhysicsSettings& GetSettings() {return *m_PhysicsSettings; }

        static void ApplyForceX(b2Body* body, float x);
        static void ApplyForceY(b2Body* body, float y);
        static void ApplyForce(b2Body* body,float x, float y);
        static void ApplyImpulseX(b2Body* body,float x);
        static void ApplyImpulseY(b2Body* body, float y);
        static void ApplyImpulse(b2Body* body, float x, float y);
        static void Move(b2Body* body, float x, float y);

    private:
        static void CreateWorld(const b2World* physicsWorld);
        
    private:
        static b2World* m_PhysicsWorld;
        static bool m_DebugActive;
        static PhysicsSettings* m_PhysicsSettings;
    };
}

#endif