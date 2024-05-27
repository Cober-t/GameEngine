#include "Ball.h"


void Ball::OnStart()
{
	player1 = scene->FindEntityByName("Player1");
	player2 = scene->FindEntityByName("Player2");
}

void Ball::OnUpdate(float dt)
{
	auto body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;
	if (Input::IsKeyDown(KeyCode::Space) && !m_MatchStarted)
	{
		m_MatchStarted = true;

		int startDirection = Random::Value(-1, 1);
		while (startDirection == 0)
			startDirection = Random::Value(-1, 1);
			
		Physics2D::Move(body, speed * startDirection, 0.0);
		m_Direction = startDirection;
	}

	float positionY = entity.GetComponent<TransformComponent>().position.y;

	if (positionY > upperLimit)
		Physics2D::Move(body, speed * m_Direction, -body->GetLinearVelocity().y);
	else if (positionY < bottomLimit)
		Physics2D::Move(body, speed * m_Direction, -body->GetLinearVelocity().y);

}

void Ball::OnBeginContact(Entity* entityContact) 
{
	auto body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;

	// Contact with goals
	if (entityContact->GetName() == "Player1Goal")
	{
		m_Score -= 1;
		m_ChangeColors = true;
		m_Direction = m_Direction == -1 ? 1 : -1;
		// m_MatchStarted = false;
		// Physics2D::Move(body, 0, 0);
	}
	else if (entityContact->GetName() == "Player2Goal")
	{
		m_Score += 1;
		m_ChangeColors = true;
		m_Direction = m_Direction == -1 ? 1 : -1;
		// m_MatchStarted = false;
		// Physics2D::Move(body, 0, 0);
	}

	// Assign Colors
	if (m_ChangeColors)
	{
		m_ChangeColors = false;
		switch(m_Score)
		{
			case 0:
				m_Score += 1;
				// m_MatchStarted = false;
				// Physics2D::Move(body, 0, 0);
				LOG_WARNING("PLAYER 2 WIN");
				break;
			case 1:
				player1.GetComponent<Render2DComponent>().color = DebugColors::red;
				player2.GetComponent<Render2DComponent>().color = DebugColors::white;
				break;
			case 2:
				player1.GetComponent<Render2DComponent>().color = DebugColors::yellow;
				player2.GetComponent<Render2DComponent>().color = DebugColors::white;
				break;
			case 3:
				player1.GetComponent<Render2DComponent>().color = DebugColors::white;
				player2.GetComponent<Render2DComponent>().color = DebugColors::white;
				break;
			case 4:
				player1.GetComponent<Render2DComponent>().color = DebugColors::white;
				player2.GetComponent<Render2DComponent>().color = DebugColors::yellow;
				break;
			case 5:
				player1.GetComponent<Render2DComponent>().color = DebugColors::white;
				player2.GetComponent<Render2DComponent>().color = DebugColors::red;
				break;
			case 6:
				m_Score -= 1;
				// m_MatchStarted = false;
				// Physics2D::Move(body, 0, 0);
				LOG_WARNING("PLAYER 1 WIN");
				break;
		}

		// return;
	}

	// Contact with players
	if (entityContact->GetName() == "Player1" || entityContact->GetName() == "Player2")
	{
		m_Direction = m_Direction == -1 ? 1 : -1;
	}

	// Ball movement
	float yComponent = Random::Value(-8, 8) / 10;
	while (yComponent == 0)
		yComponent = Random::Value(-8, 8) / 10;

	Physics2D::Move(body, speed * m_Direction, yComponent);
}