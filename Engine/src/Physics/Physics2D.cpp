#include <pch.h>

#include "Physics/Physics2D.h"
#include "Scene/Systems/ScriptSystem.h"
#include "Core/EngineApp.h"

namespace Cober {

    bool Physics2D::m_DebugActive = false;
    b2World* Physics2D::m_PhysicsWorld = nullptr;
    PhysicsSettings* Physics2D::m_PhysicsSettings = new PhysicsSettings();

    void Physics2D::Init()
    {
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
        m_PhysicsWorld = new b2World({ 0.0f, m_PhysicsSettings->Gravity });
    }


    void Physics2D::Step()
    {
        m_PhysicsWorld->Step(m_PhysicsSettings->TimeStep, m_PhysicsSettings->VelocityIterations, m_PhysicsSettings->PositionIterations);
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


    void Physics2D::ApplyForceX(b2Body* body, float x)
    {
        body->ApplyForce(b2Vec2(x, 0), body->GetWorldCenter(), true);
    }

    void Physics2D::ApplyForceY(b2Body* body, float y)
    {
        body->ApplyForce(b2Vec2(0, y), body->GetWorldCenter(), true);
    }

    void Physics2D::ApplyForce(b2Body* body,float x, float y)
    {
        body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter(), true);
    }

    void Physics2D::ApplyImpulseX(b2Body* body,float x)
    {
        body->ApplyLinearImpulse(b2Vec2(x, 0), body->GetWorldCenter(), true);
    }

    void Physics2D::ApplyImpulseY(b2Body* body, float y)
    {
        body->ApplyLinearImpulse(b2Vec2(0, y), body->GetWorldCenter(), true);
    }
    
    void Physics2D::ApplyImpulse(b2Body* body, float x, float y)
    {
        body->ApplyLinearImpulse(b2Vec2(x, y), body->GetWorldCenter(), true);
    }

    void Physics2D::SetTransform(b2Body* body, float x, float y, float angle)
    {
        body->SetTransform(b2Vec2(x, y), angle);
    }
}