#include "Scene/Systems/PhysicsSystem2D.h"

namespace Cober {

	PhysicsSystem2D::PhysicsSystem2D(Scene* scene) : m_Scene(scene)
	{
		LOG_INFO("Physics System Added to Registry!!");
	}


	PhysicsSystem2D::~PhysicsSystem2D()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
		LOG_INFO("Physics System removed from Registry");
	}


	void PhysicsSystem2D::Start()
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.8f });

		for (auto& entity : GetSystemEntities())
		{
			InitEntityPhysics(entity);
		}

		LOG_INFO("Physic System Started!!");
	}


	void PhysicsSystem2D::InitEntityPhysics(Entity entity)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb2d = entity.GetComponent<Rigidbody2D>();

		b2BodyDef bodyDef;
		bodyDef.type = (b2BodyType)rb2d.type;
		bodyDef.angle = transform.rotation.z;

		// BOX COLLIDER
		b2FixtureDef fixtureDef;
		if (entity.HasComponent<BoxCollider2D>()) 
		{
			auto& bc2d = entity.GetComponent<BoxCollider2D>();

			bc2d.shape.SetAsBox((abs(bc2d.size.x)* abs(transform.scale.x)) * 0.5f, (abs(bc2d.size.y) * abs(transform.scale.y)) * 0.5f);
			bodyDef.position.Set(transform.position.x + bc2d.offset.x, transform.position.y + bc2d.offset.y);

			fixtureDef.shape = &bc2d.shape;
			fixtureDef.density = bc2d.density;
			fixtureDef.friction = bc2d.friction;
			fixtureDef.restitution = bc2d.restitution;
			// fixtureDef.restitutionThreshold = bc2d.restitutionThreshold;
		}
		else 
			bodyDef.position.Set(transform.position.x, transform.position.y);


		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb2d.fixedRotation);
		
		if(entity.HasComponent<BoxCollider2D>())
			body->CreateFixture(&fixtureDef);

		rb2d.runtimeBody = body;
	}


	void PhysicsSystem2D::Update(Timestep ts)
	{
		const int32_t velocityIterations = 8;
		const int32_t positionIterations = 3;
		m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

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

				transform.rotation.x = 0.0f;	// 2D
				transform.rotation.y = 0.0f;	// 2D
				transform.rotation.z = rb2d.fixedRotation ? 0.0f : body->GetAngle();
			}
		}
	}


	std::vector<Entity> PhysicsSystem2D::GetSystemEntities() const
	{
		std::vector<Entity> entities;
		auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, Rigidbody2D>();
		// auto entitiesView = m_Scene->GetAllEntitiesWith<TransformComponent, SpriteComponent>();

		for (auto entity : entitiesView)
		{
			Entity newEntity = { entity, m_Scene };
			// entities.emplace( newEntity );
			entities.emplace_back( newEntity );
		}

		return entities;
	};
}