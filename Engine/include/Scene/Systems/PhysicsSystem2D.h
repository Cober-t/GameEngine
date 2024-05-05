#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
// #include "Events/Event.h"

#include <b2_body.h>
#include <b2_fixture.h>
#include <b2_draw.h>

#include <Render/Primitives/Line.h>
#include <Physics/Physics2D.h>
#include <Physics/Debug2DPhysics.h>


namespace Cober {

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
	};
}

#endif