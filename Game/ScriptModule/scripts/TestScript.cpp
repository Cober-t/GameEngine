#include "TestScript.h"


void TestScript::OnStart()
{
	std::cout << entity.GetName() << std::endl;
}

void TestScript::OnUpdate(float dt)
{
	// Test movement
	if (Input::IsKeyDown(KeyCode::A))
	{
		entity.GetComponent<TransformComponent>().position.x -= 0.01 * dt;
	}
	if (Input::IsKeyDown(KeyCode::D))
	{
		entity.GetComponent<TransformComponent>().position.x += 0.01 * dt;
	}
	if (Input::IsKeyDown(KeyCode::W))
	{
		entity.GetComponent<TransformComponent>().position.y += 0.01 * dt;
	}
	if (Input::IsKeyDown(KeyCode::S))
	{
		entity.GetComponent<TransformComponent>().position.y -= 0.01 * dt;
	}
}