#include "TestScript.h"


void TestScript::OnStart()
{
	LOG_WARNING("START!");
}

void TestScript::OnUpdate(float dt)
{
	if (gameStarted == false && Input::IsKeyPressed(KeyCode::Space))
	{
		gameStarted = true;
		Scene::Reload(scene);
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