#ifndef PHYSICS_2D
#define PHYSICS_2D

#include <b2_world.h>
#include <b2_contact.h>

#include "Scene/ECS.h"
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
        static void Init(Scene* scene);
        static void DebugDraw();
        static void Step();
        
        static void Update(Scene* scene);
        static void SetBodyType(Entity& entity, BodyType type);
        static void InitEntity(Entity& entity);

        inline static void SetDrawDebug(b2Draw* debugDraw) { m_PhysicsWorld->SetDebugDraw(debugDraw); };
        inline static b2Body* CreateBody(b2BodyDef body) { return m_PhysicsWorld->CreateBody(&body); }
        inline static void DestroyBody(Entity entity);

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
        static void InitEntityPhysics(Entity& entity);
        static void CreateWorld();
        
    private:
        static b2World* m_PhysicsWorld;
        static bool m_DebugActive;
        static PhysicsSettings* m_PhysicsSettings;
        static std::vector<Entity> m_EntitiesToInitPhysics;
        static std::vector<b2Body*> m_BodiesToBeDestroyed;
    };
}

#endif