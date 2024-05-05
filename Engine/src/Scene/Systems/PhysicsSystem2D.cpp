#include <pch.h>
#include "Scene/Systems/PhysicsSystem2D.h"
#include "Core/EngineApp.h"

namespace Cober {

	PhysicsSystem2D::PhysicsSystem2D(Scene* scene) : m_Scene(scene)
	{
		LOG_INFO("Physics System Added to Registry!!");
	}


	PhysicsSystem2D::~PhysicsSystem2D()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
		LOG_INFO("Physics System Removed from Registry");
	}


	void PhysicsSystem2D::Start()
	{
		Physics2D::Init();

		for (auto& entity : GetSystemEntities())
		{
			InitEntityPhysics(entity);
		}

		LOG_INFO("Physics System Started!!");
	}


	void PhysicsSystem2D::InitEntityPhysics(Entity entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2D>();

		b2BodyDef bodyDef;
		bodyDef.type = (b2BodyType)rb2d.type;
		bodyDef.position.Set(transform.position.x, transform.position.y);
		bodyDef.angle = transform.rotation.z;

		Entity* staticRef = (Entity*)malloc(sizeof(entity));
		*staticRef = entity;
		bodyDef.userData.pointer = (uintptr_t)staticRef;

		b2Body* body = Physics2D::CreateBody(bodyDef);

		body->SetFixedRotation(rb2d.fixedRotation);
		rb2d.runtimeBody = body;

		if (entity.HasComponent<BoxCollider2D>()) 
		{
			auto& boxEntity = entity.GetComponent<BoxCollider2D>();

			boxEntity.shape.SetAsBox((abs(boxEntity.size.x) * abs(transform.scale.x)), 
								(abs(boxEntity.size.y) * abs(transform.scale.y)),
								(b2Vec2(boxEntity.offset.x, boxEntity.offset.y)),
								0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxEntity.shape;
			fixtureDef.density = boxEntity.density;
			fixtureDef.friction = boxEntity.friction;
			fixtureDef.restitution = boxEntity.restitution;
			body->CreateFixture(&fixtureDef);
		}
		else if (entity.HasComponent<CircleCollider2D>())
		{
			auto& circleEntity = entity.GetComponent<CircleCollider2D>();

			circleEntity.shape.m_p.Set(circleEntity.offset.x, circleEntity.offset.y);
			circleEntity.shape.m_radius = transform.scale.x * circleEntity.radius;
			
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleEntity.shape;
			fixtureDef.density = circleEntity.density;
			fixtureDef.friction = circleEntity.friction;
			fixtureDef.restitution = circleEntity.restitution;
			body->CreateFixture(&fixtureDef);
		}
		// else if (entity.HasComponent<PolygonCollider2D>())
		// {
		// 	auto& polygonEntity = entity.GetComponent<PolygonCollider2D>();
		// 	// Manage physics...
		// 	//
		// }
		// else if (entity.HasComponent<EdgeCollider2D>())
		// {
		// 	auto& edgeEntity = entity.GetComponent<EdgeCollider2D>();
		// 	// Manage physics...
		// 	//
		// }

	}


	void PhysicsSystem2D::Update(Unique<Timestep>& ts)
	{
		// Export to Settings
		const int32_t velocityIterations = 8;
		const int32_t positionIterations = 3;

		Physics2D::Step(ts->GetLimitFPS(), velocityIterations, positionIterations);

		for (auto& entity : GetSystemEntities()) 
        {
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2D>();
			
			b2Body* body = (b2Body*)rb2d.runtimeBody;
			const auto& position = body->GetPosition();

			if (rb2d.type != BodyType::Static) 
			{
				transform.position.x = position.x;
				transform.position.y = position.y;
				transform.rotation.z = rb2d.fixedRotation ? 0.0f : body->GetAngle();
			}
		}

		// Send to Render and prepare to Debug on Runtime Editor
		Physics2D::DebugDraw();
	}


	std::vector<Entity> PhysicsSystem2D::GetSystemEntities() const
	{
		std::vector<Entity> entities;
		auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, Rigidbody2D>();

		for (auto entity : entitiesView)
			entities.emplace_back( Entity{ entity, m_Scene } );

		return entities;
	};
}