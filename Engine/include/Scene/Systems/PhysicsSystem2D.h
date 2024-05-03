#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
// #include "Events/Event.h"

#include <b2World.h>
#include <b2Body.h>
#include <b2Fixture.h>
#include <b2PolygonShape.h>
#include <b2Draw.h>
#include <b2Contact.h>
#include <Render/Primitives/Line.h>


namespace Cober {

	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override;

		void EndContact(b2Contact* contact) override;

		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	};


	class DebugPhysics : public b2Draw
	{
	public:
		void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {};
		void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {};
		void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {};
		void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {};
		void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {};
		void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
		{ 
			Primitive::Line::Draw(glm::vec3(0.0f), glm::vec3(3.0f), glm::vec4(0.0f, 1.0, 0.0f, 1.0f), -1);
		};
		void DrawTransform(const b2Transform& xf) {};
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
		DebugPhysics* m_DebugPhysics;
	};

	static ContactListener contactListener;
}

#endif