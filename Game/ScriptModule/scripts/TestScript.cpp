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
		// Scene::LoadPrefab(scene, "Pipe.lua");
		Physics2D::SetBodyType(entity, BodyType::Dynamic);
	}
}

void TestScript::OnBeginContact(Entity* entityContact) 
{
	if (entityContact->GetName() == "Entity0")
	{
		LOG_WARNING(entityContact->GetName());
		Physics2D::MoveTo(Scene::LoadPrefab(scene, "Pipe.lua"), 2, 0);
		// Audio::Play(entity.GetComponent<AudioComponent>().audioName);
	}
	// LOG_WARNING("{0} BEGIN COLLIDE ", entityContact->GetName());
}

void TestScript::OnEndContact(Entity* entityContact) 
{
	Scene::Reload(scene);
}