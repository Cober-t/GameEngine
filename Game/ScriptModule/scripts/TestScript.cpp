#include "TestScript.h"


void TestScript::OnStart()
{
	LOG_WARNING("START!");
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
	LOG_WARNING("{0} COLLIDE ", entityContact->GetName());
}