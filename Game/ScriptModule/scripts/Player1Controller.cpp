#include "Player1Controller.h"


void Player1Controller::OnStart()
{
}

void Player1Controller::OnUpdate(float dt)
{
	// Test movement
	auto body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;
	float positionY = entity.GetComponent<TransformComponent>().position.y;

	if (Input::IsKeyDown(KeyCode::W) && positionY < upperLimit)
	{
		Physics2D::Move(body, 0, 1);
	}
	else if (Input::IsKeyDown(KeyCode::S) && positionY > bottomLimit)
	{
		Physics2D::Move(body, 0, -1);
	}
	else
		Physics2D::Move(body, 0, 0);
}

void Player1Controller::OnBeginContact(Entity* entityContact) 
{
	LOG_WARNING("{0} COLLIDE ", entityContact->GetName());
}