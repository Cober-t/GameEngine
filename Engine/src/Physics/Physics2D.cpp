#include <pch.h>

#include "Physics/Physics2D.h"
#include "Scene/Systems/ScriptSystem.h"
#include "Core/EngineApp.h"

namespace Cober {

    b2World* Physics2D::m_PhysicsWorld = nullptr;
    bool Physics2D::m_DebugActive = false;

    void Physics2D::Init()
    {
        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

        CreateWorld(m_PhysicsWorld);
        
        m_PhysicsWorld->SetContactListener(new ContactListener());

        if (EngineApp::Get().IsDebugMode())
		{
			m_PhysicsWorld->SetDebugDraw(reinterpret_cast<b2Draw*>(&Debug2DPhysics::Get()));
            m_DebugActive = true;
		}
    }


    void Physics2D::CreateWorld(const b2World* physicsWorld)
    {
        m_PhysicsWorld = new b2World({ 0.0f, -9.8f });
    }


    void Physics2D::Step(float timeStep, int velocityIterations, int positionIterations)
    {
        m_PhysicsWorld->Step(timeStep, velocityIterations, positionIterations);
    }


    void Physics2D::DebugDraw()
    {
		if (EngineApp::Get().IsDebugMode())
        {
            if (!m_DebugActive)
            {
                m_DebugActive = true;
                m_PhysicsWorld->SetDebugDraw(reinterpret_cast<b2Draw*>(&Debug2DPhysics::Get()));
            }

            m_PhysicsWorld->DebugDraw();
        }
        else
            m_DebugActive = false;
    }


    void ContactListener::BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
        uintptr_t indexBodyA = fixtureA->GetBody()->GetUserData().pointer;
		if (indexBodyA)
		{
            // LOG_WARNING("BEGIN CONTACT! {0}", reinterpret_cast<Entity*>(indexBodyA)->GetName());
		}
	
		b2Fixture* fixtureB = contact->GetFixtureB();
        uintptr_t indexBodyB = fixtureB->GetBody()->GetUserData().pointer;
		if (indexBodyB)
		{
            // LOG_WARNING("BEGIN CONTACT: {0}", reinterpret_cast<Entity*>(indexBodyB)->GetName());
		}

        NativeScriptFn::NotifyBeginContact(reinterpret_cast<Entity*>(indexBodyA), reinterpret_cast<Entity*>(indexBodyB));
	}


	void ContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		uintptr_t indexBodyA = fixtureA->GetBody()->GetUserData().pointer;
		if (indexBodyA)
		{
            // LOG_WARNING("END CONTACT: {0}", reinterpret_cast<Entity*>(indexBodyA)->GetName());
		}
	
		b2Fixture* fixtureB = contact->GetFixtureB();
		uintptr_t indexBodyB = fixtureB->GetBody()->GetUserData().pointer;
		if (indexBodyB)
		{
            // LOG_WARNING("END CONTACT: {0}", reinterpret_cast<Entity*>(indexBodyB)->GetName());
		}
	}


	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		// TODO: Implement me
	}


	void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		// TODO: Implement me
	}
}