#include "TestScript.h"


void TestScript::OnStart()
{
	LOG_WARNING("START!");
}

void TestScript::OnUpdate(float dt)
{
	auto body = (b2Body*)entity.GetBody();

	if (Input::IsKeyPressed(KeyCode::Space))
	{
		Physics2D::SetBodyType(entity, BodyType::Dynamic);
	}
	
	if (Input::IsKeyPressed(KeyCode::R) && Input::IsKeyDown(KeyCode::LeftControl))
	{
		Scene::Exit(scene);
		// Scene::Reload(scene, "NewScene.lua");
	}
}

void TestScript::OnBeginContact(Entity* entityContact) 
{
}

void TestScript::OnEndContact(Entity* entityContact) 
{
}