#include "TestScript.h"


void TestScript::OnStart()
{
	LOG_WARNING("START!");
}

void TestScript::OnUpdate(float dt)
{
	auto body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;

	if (Input::IsKeyPressed(KeyCode::Space))
	{
		Audio::Play("score.wav", true);
		// Scene::LoadPrefab(scene, "Pipe.lua");
		Physics2D::SetBodyType(entity, BodyType::Dynamic);
		LOG_WARNING("NOW IS DYNAMIC");
	}
	
	if (Input::IsKeyPressed(KeyCode::Up))
	{
		Scene::Reload(scene, "NewScene.lua");
	}
}

void TestScript::OnBeginContact(Entity* entityContact) 
{
}

void TestScript::OnEndContact(Entity* entityContact) 
{
}