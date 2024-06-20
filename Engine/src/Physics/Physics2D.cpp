#include <pch.h>

#include "Physics/Physics2D.h"
#include "Scene/Systems/ScriptSystem.h"
#include "Core/EngineApp.h"

namespace Cober {

    bool Physics2D::m_DebugActive = false;
    b2World* Physics2D::m_PhysicsWorld = nullptr;
    PhysicsSettings* Physics2D::m_PhysicsSettings = new PhysicsSettings();
    std::vector<Entity> Physics2D::m_EntitiesToInitPhysics;
    std::vector<b2Body*> Physics2D::m_BodiesToBeDestroyed;
    std::vector<std::pair<BodyChangesType, BodyValues>> Physics2D::m_ApplyBodyChangesPool;

    void Physics2D::Init(Scene* scene)
    {
        CreateWorld();
        
        m_PhysicsWorld->SetContactListener(new ContactListener());

        if (EngineApp::Get().IsDebugMode())
		{
			m_PhysicsWorld->SetDebugDraw(reinterpret_cast<b2Draw*>(&Debug2DPhysics::Get()));
            m_DebugActive = true;
		}

        for (auto entt : scene->GetAllEntitiesWith<TransformComponent, Rigidbody2D>())
		{
			Entity entity = Entity((entt::entity)entt, scene );
			Physics2D::InitEntity(entity);
		}
    }

    void Physics2D::InitEntityPhysics(Entity& entity)
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

        if (rb2d.runtimeBody)
        {
            m_PhysicsWorld->DestroyBody((b2Body*)rb2d.runtimeBody);
        }

		b2Body* body = Physics2D::CreateBody(bodyDef);

		body->SetFixedRotation(rb2d.fixedRotation);
		rb2d.runtimeBody = body;

		if (entity.HasComponent<BoxCollider2D>()) 
		{
			auto& boxEntity = entity.GetComponent<BoxCollider2D>();
            
            float hx = (abs(boxEntity.size.x) * abs(transform.scale.x));
            float hy = (abs(boxEntity.size.y) * abs(transform.scale.y));

            if (entity.HasComponent<Render2DComponent>())
            {
                auto& renderComponent = entity.GetComponent<Render2DComponent>();
                if (renderComponent.shapeType == Shape2D::Sprite && renderComponent.texture)
                {
                    hx *= abs(renderComponent.vertices[0].x);
                    hy *= abs(renderComponent.vertices[0].y);
                }
            }

            b2Vec2 center = b2Vec2(boxEntity.offset.x, boxEntity.offset.y);
			boxEntity.shape.SetAsBox(hx,  hy, center, 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxEntity.shape;
			fixtureDef.density = boxEntity.density;
			fixtureDef.friction = boxEntity.friction;
			fixtureDef.restitution = boxEntity.restitution;
            if (boxEntity.isSensor)
                fixtureDef.isSensor = true;
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
            if (circleEntity.isSensor)
                fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
		}
	}


    void Physics2D::InitEntity(Entity& entity)
    {
        if (entity.HasComponent<Rigidbody2D>())
            m_EntitiesToInitPhysics.push_back(entity);
    }


    void Physics2D::DestroyBody(Entity entity)
    {
        if (entity.HasComponent<Rigidbody2D>())
        {
            b2Body* body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;
            if (body)
                m_BodiesToBeDestroyed.push_back(body);
        }
    }


    void Physics2D::SetBodyType(Entity& entity, BodyType type)
    {
        if (entity.HasComponent<Rigidbody2D>())
        {
            auto& entityBodyType = entity.GetComponent<Rigidbody2D>().type;
            if (entityBodyType == type)
                return;

            entityBodyType = type;

            if (EngineApp::Get().GetGameState() == EngineApp::GameState::RUNTIME_EDITOR
                || EngineApp::Get().GetGameState() == EngineApp::GameState::PLAY)
            {
                Physics2D::InitEntity(entity);
            }
        }
    }


    void Physics2D::CreateWorld()
    {
        m_PhysicsWorld = new b2World({ 0.0f, m_PhysicsSettings->Gravity });
    }


    void Physics2D::Step()
    {
        m_PhysicsWorld->Step(m_PhysicsSettings->TimeStep, m_PhysicsSettings->VelocityIterations, m_PhysicsSettings->PositionIterations);
    }


    void Physics2D::Update(Scene* scene)
    {
		Physics2D::CleanUp();

        if (m_EntitiesToInitPhysics.size() > 0)
        {
            for (Entity& entity : m_EntitiesToInitPhysics)
            {
                Physics2D::InitEntityPhysics(entity);
            }
            m_EntitiesToInitPhysics.clear();
        }

        ApplyBodyChanges();

        auto view = scene->GetAllEntitiesWith<TransformComponent, Rigidbody2D>();

		for (auto entt : view) 
        {
			Entity entity = Entity((entt::entity)entt, scene );
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<Rigidbody2D>();
			
			b2Body* body = (b2Body*)rb2d.runtimeBody;
            if (body)
            {
			    const auto& position = body->GetPosition();
			    transform.position.x = position.x;
			    transform.position.y = position.y;
			    transform.rotation.z = rb2d.fixedRotation ? 0.0f : body->GetAngle();
            }
		}
    }


    void Physics2D::ApplyBodyChanges()
    {
        if (m_ApplyBodyChangesPool.size() == 0)
            return;

        for (auto& bodyChanges : m_ApplyBodyChangesPool)
        {
            auto& bodyValues = bodyChanges.second;
            auto& body = bodyChanges.second.body;

            switch(bodyChanges.first)
            {
                case BodyChangesType::MOVE:
                    body->SetLinearVelocity(bodyValues.velocity);
                    break;
                case BodyChangesType::APPLY_LINEAR_IMPULSE:
                    body->ApplyLinearImpulse(bodyValues.impulse, body->GetWorldCenter(), true);
                    break;
                case BodyChangesType::APPLY_FORCE:
                    body->ApplyForce(bodyValues.force, body->GetWorldCenter(), true);
                    break;
            }
        }
        m_ApplyBodyChangesPool.clear();
    } 


    void Physics2D::CleanUp()
    {
        if (m_BodiesToBeDestroyed.size() > 0)
        {
            for (b2Body* body : m_BodiesToBeDestroyed)
            {
                m_PhysicsWorld->DestroyBody(body);
            }
            m_BodiesToBeDestroyed.clear();
        }
    }


    void Physics2D::DebugDraw(Scene* scene)
    {
		if (EngineApp::Get().IsDebugMode())
        {
            // Simulation / Runtime State
            if ((EngineApp::Get().GetGameState() == EngineApp::GameState::RUNTIME_EDITOR || 
                EngineApp::Get().GetGameState() == EngineApp::GameState::PLAY) &&
                m_PhysicsWorld) 
            {
                if (!m_DebugActive)
                {
                    m_DebugActive = true;
                    m_PhysicsWorld->SetDebugDraw(reinterpret_cast<b2Draw*>(&Debug2DPhysics::Get()));
                }

                m_PhysicsWorld->DebugDraw();
            }
            // Editor State
            else if (EngineApp::Get().GetGameState() == EngineApp::GameState::EDITOR)
            {
                glm::vec4 color{ 0.0f, 0.0f, 0.0f, 0.45f };

                // Debug Box Colliders
                auto boxColliderView = scene->GetAllEntitiesWith<TransformComponent, BoxCollider2D>();

                for (auto& ent : boxColliderView)
                {
                    Entity entity = Entity((entt::entity)ent, scene);
                    auto& entTrans = entity.GetComponent<TransformComponent>();
                    auto& bc2d = entity.GetComponent<BoxCollider2D>();

                    glm::vec3 position(entTrans.position.x + bc2d.offset.x, entTrans.position.y + bc2d.offset.y, entTrans.position.z + 0.01f);
                    glm::vec3 rotation(0.0f, 0.0f, entTrans.rotation.z);
                    glm::vec3 scale(abs(entTrans.scale.x * bc2d.size.x), abs(entTrans.scale.y * bc2d.size.y), 1.0f);

                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                        * glm::toMat4(glm::quat(rotation))
                        * glm::scale(glm::mat4(1.0f), scale);

                    if (entity.HasComponent<Render2DComponent>() && entity.GetComponent<Render2DComponent>().texture)
                        Render2D::DrawQuad(transform, color, entity.GetComponent<Render2DComponent>().subTexture, (int)entity);
                    else
                        Render2D::DrawQuad(transform, color, nullptr, (int)entity);
                }

                // Debug Circle Colliders
                auto circleColliderView = scene->GetAllEntitiesWith<TransformComponent, CircleCollider2D>();

                for (auto& ent : circleColliderView)
                {
                    Entity entity = Entity((entt::entity)ent, scene);
                    auto& entTrans = entity.GetComponent<TransformComponent>();
                    auto& cc2d = entity.GetComponent<CircleCollider2D>();

                    float thickness = 1.0f;
                    float radius = cc2d.radius * 2;
                    if (entity.HasComponent<Render2DComponent>())
                        thickness = entity.GetComponent<Render2DComponent>().thickness;

                    glm::vec3 position(entTrans.position.x + cc2d.offset.x, entTrans.position.y + cc2d.offset.y, entTrans.position.z + 0.01f);
                    glm::vec3 rotation(0.0f, 0.0f, entTrans.rotation.z);
                    glm::vec3 scale(abs(entTrans.scale.x * radius), abs(entTrans.scale.y * radius), 1.0f);

                    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                        * glm::toMat4(glm::quat(rotation))
                        * glm::scale(glm::mat4(1.0f), scale);

                    Render2D::DrawCircle(transform, color, thickness, (int)entity);
                }
            }
        }
        else
            m_DebugActive = false;
    }


    void ContactListener::BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
        uintptr_t indexBodyA = fixtureA->GetBody()->GetUserData().pointer;
        uintptr_t indexBodyB = fixtureB->GetBody()->GetUserData().pointer;
		if (indexBodyA || indexBodyB)
		{
            NativeScriptFn::NotifyBeginContact(reinterpret_cast<Entity*>(indexBodyA), reinterpret_cast<Entity*>(indexBodyB));
		}
	}


	void ContactListener::EndContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		uintptr_t indexBodyA = fixtureA->GetBody()->GetUserData().pointer;
		uintptr_t indexBodyB = fixtureB->GetBody()->GetUserData().pointer;
		if (indexBodyA || indexBodyB)
		{
            NativeScriptFn::NotifyEndContact(reinterpret_cast<Entity*>(indexBodyA), reinterpret_cast<Entity*>(indexBodyB));
		}
	}


	void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
	}


	void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
	}


    //////////////////////
    // SCRIPT FUNCTIONS //
    void Physics2D::ApplyForceX(b2Body* body, float x)
    {
        Physics2D::ApplyForce(body, x, 0);
    }

    void Physics2D::ApplyForceY(b2Body* body, float y)
    {
        Physics2D::ApplyForce(body, 0, y);
    }

    void Physics2D::ApplyForce(b2Body* body,float x, float y)
    {
        BodyValues bodyValues;
        bodyValues.body = body;
        bodyValues.force = b2Vec2(x, y);
        auto& bodyChangesToApply = std::make_pair(BodyChangesType::APPLY_FORCE, bodyValues);

        m_ApplyBodyChangesPool.push_back(bodyChangesToApply);
    }

    void Physics2D::ApplyImpulseX(b2Body* body,float x)
    {
        Physics2D::ApplyImpulse(body, x, 0);
    }

    void Physics2D::ApplyImpulseY(b2Body* body, float y)
    {
        Physics2D::ApplyImpulse(body, y, 0);
    }
    
    void Physics2D::ApplyImpulse(b2Body* body, float x, float y)
    {
        BodyValues bodyValues;
        bodyValues.body = body;
        bodyValues.impulse = b2Vec2(x, y);
        auto& bodyChangesToApply = std::make_pair(BodyChangesType::APPLY_LINEAR_IMPULSE, bodyValues);

        m_ApplyBodyChangesPool.push_back(bodyChangesToApply);
    }

    void Physics2D::Move(b2Body* body, float x, float y)
    {
        if (body == nullptr)
            return;

        b2Vec2 velocity(x, y);
        if (body->GetLinearVelocity() == velocity)
            return;

        BodyValues bodyValues;
        bodyValues.body = body;
        bodyValues.velocity = b2Vec2(x, y);
        auto& bodyChangesToApply = std::make_pair(BodyChangesType::MOVE, bodyValues);

        m_ApplyBodyChangesPool.push_back(bodyChangesToApply);
    }


    void Physics2D::MoveTo(Entity& entity, float x, float y)
    {
        entity.GetComponent<TransformComponent>().position.x = x;
        entity.GetComponent<TransformComponent>().position.y = y;
        Physics2D::InitEntity(entity);
    }
}