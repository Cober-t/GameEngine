#include "TestScript.h"


void TestScript::OnStart()
{
}

void TestScript::OnUpdate(float dt)
{
}

void TestScript::OnBeginContact(Entity* entityContact) 
{
	LOG_WARNING("{0} COLLIDE ", entityContact->GetName());
}