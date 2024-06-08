#include <pch.h>
#include "Scene/Systems/PhysicsSystem2D.h"
#include "Core/EngineApp.h"

namespace Cober {

	PhysicsSystem2D::PhysicsSystem2D()
	{
		LOG_INFO("Physics System Added to Registry!!");
	}


	PhysicsSystem2D::~PhysicsSystem2D()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
		LOG_INFO("Physics System Removed from Registry");
	}


	void PhysicsSystem2D::Start(Scene* scene)
	{
		Physics2D::Init();

		for (auto entt : scene->GetAllEntitiesWith<TransformComponent, Rigidbody2D>())
		{
			Entity entity = Entity((entt::entity)entt, scene );
			Physics2D::InitEntityPhysics(entity);
		}

		LOG_INFO("Physics System Started!!");
	}


	void PhysicsSystem2D::Update(Scene* scene, Unique<Timestep>& ts)
	{
		Physics2D::Step();

		auto view = scene->GetAllEntitiesWith<TransformComponent, Rigidbody2D>();

		for (auto entt : view) 
        {
			Entity entity = Entity((entt::entity)entt, scene );
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2D>();
			
			b2Body* body = (b2Body*)rb2d.runtimeBody;
			const auto& position = body->GetPosition();
			transform.position.x = position.x;
			transform.position.y = position.y;
			transform.rotation.z = rb2d.fixedRotation ? 0.0f : body->GetAngle();
		}
	}
}