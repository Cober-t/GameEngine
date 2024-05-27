#include "Player2Controller.h"


void Player2Controller::OnStart()
{
}

void Player2Controller::OnUpdate(float dt)
{
	auto body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;
	float positionY = entity.GetComponent<TransformComponent>().position.y;

	if (Input::IsKeyDown(KeyCode::Up) && positionY < upperLimit)
	{
		Physics2D::Move(body, 0, 1);
	}
	else if (Input::IsKeyDown(KeyCode::Down) && positionY > bottomLimit)
	{
		Physics2D::Move(body, 0, -1);
	}
	else
		Physics2D::Move(body, 0, 0);
}

void Player2Controller::OnBeginContact(Entity* entityContact) 
{
	LOG_WARNING("{0} COLLIDE ", entityContact->GetName());
}