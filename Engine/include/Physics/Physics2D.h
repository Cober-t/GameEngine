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

    enum class BodyChangesType
    {
        MOVE = 0,
        APPLY_LINEAR_IMPULSE,
        APPLY_FORCE,
    };

    struct BodyValues
    {
        b2Body* body = nullptr;
        float x = 0.0f;
        float y = 0.0f;
        b2Vec2 impulse = b2Vec2(0, 0);
        b2Vec2 force = b2Vec2(0, 0);
        b2Vec2 velocity = b2Vec2(0, 0);
    };

    class CB_API Physics2D
    {
    public:
        static void Init(Scene* scene);
        static void DebugDraw(Scene* scene);
        static void Step();
        
        static void Update(Scene* scene);
        static void SetBodyType(Entity& entity, BodyType type);
        static void InitEntity(Entity& entity);

        inline static void SetDrawDebug(b2Draw* debugDraw) { m_PhysicsWorld->SetDebugDraw(debugDraw); };
        inline static b2Body* CreateBody(b2BodyDef body) { return m_PhysicsWorld->CreateBody(&body); }
        inline static void DestroyBody(Entity entity);

        inline static int32 GetBodyCount() { return m_PhysicsWorld ? m_PhysicsWorld->GetBodyCount() : 0; };

        inline static b2World& PhysicsWorld() {return *m_PhysicsWorld; }
        static void CleanUp();

        // Move to Scripting System
        static void ApplyForceX(b2Body* body, float x);
        static void ApplyForceY(b2Body* body, float y);
        static void ApplyForce(b2Body* body,float x, float y);
        static void ApplyImpulseX(b2Body* body,float x);
        static void ApplyImpulseY(b2Body* body, float y);
        static void ApplyImpulse(b2Body* body, float x, float y);
        static void Move(b2Body* body, float x, float y);
        static void MoveTo(Entity& entity, float x, float y);

    private:
        static void InitEntityPhysics(Entity& entity);
        static void ApplyBodyChanges();
        static void CreateWorld();
        
    private:
        static b2World* m_PhysicsWorld;
        static bool m_DebugActive;
        static std::vector<Entity> m_EntitiesToInitPhysics;
        static std::vector<b2Body*> m_BodiesToBeDestroyed;
        static std::vector<std::pair<BodyChangesType, BodyValues>> m_ApplyBodyChangesPool;
    };
}

#endif