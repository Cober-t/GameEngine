#include "TestScript.h"


void TestScript::OnStart()
{
	LOG_WARNING("START!");
}

void TestScript::OnUpdate(float dt)
{
	auto body = (b2Body*)entity.GetBody();
	if (gameStarted == false)
	{
		// Physics2D::MoveTo(entity, -1, 0);
		if (Input::IsKeyPressed(KeyCode::Space))
		{
			gameStarted = true;
			// Physics2D::Move(body, 0.75f, 0.75f);

			auto& renderComponent = entity.GetComponent<Render2DComponent>();
			if (renderComponent.texture && renderComponent.isSubTexture)
			{
				Scene::Reload(scene);
			}
		}
	}
}

void TestScript::OnBeginContact(Entity* entityContact) 
{
}

void TestScript::OnEndContact(Entity* entityContact) 
{
}

void TestScript::OnEvent(Event& event)
{
}