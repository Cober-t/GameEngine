#include "TestScript.h"


void TestScript::OnStart()
{
	LOG_WARNING("START!");
	entityAux = Scene::LoadPrefab(scene, "Pipe.lua");
    entityAux.GetComponent<TransformComponent>().position.y = 1.5;
}

void TestScript::OnUpdate(float dt)
{
	auto body = (b2Body*)entity.GetComponent<Rigidbody2D>().runtimeBody;

	if(Input::IsKeyDown(KeyCode::R))
	{
		Scene::Reload(scene, "SceneDefault.lua");
		return;
	}

	if (Input::IsKeyDown(KeyCode::Space))
	{
		Physics2D::SetBodyType(entity, BodyType::Dynamic);
	}
}

void TestScript::OnBeginContact(Entity* entityContact) 
{
	LOG_WARNING("{0} BEGIN COLLIDE ", entityContact->GetName());
}

void TestScript::OnEndContact(Entity* entityContact) 
{
	LOG_WARNING("{0} END COLLIDE ", entityContact->GetName());
}