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
		LOG_INFO("Physics System Removed from Registry");
	}


	void PhysicsSystem2D::Start(Scene* scene)
	{
		Physics2D::Init(scene);

		LOG_INFO("Physics System Started!!");
	}


	void PhysicsSystem2D::Update(Scene* scene)
	{
		Physics2D::Step();

		Physics2D::Update(scene);
	}
}