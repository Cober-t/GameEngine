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

	class CB_API PhysicsSystem2D : public System 
    {
	public:
		PhysicsSystem2D();
		~PhysicsSystem2D();

		void Start(Scene* scene);
		void Update(Scene* scene, Unique<Timestep>& ts);

		//void OnEvent(Event& event);
	
	private:
	};
}

#endif