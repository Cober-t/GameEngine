#include "TestScript.h"


void TestScript::OnStart()
{
	std::cout << entity.GetName() << std::endl;
}

void TestScript::OnUpdate(float dt)
{
	// Test movement
	auto body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;
	if (Input::IsKeyDown(KeyCode::W))
	{
		Physics2D::ApplyImpulseY(body, 0.05 * dt);
	}
	else if (Input::IsKeyDown(KeyCode::S))
	{
		Physics2D::ApplyImpulseY(body, -0.05 * dt);
	}
}