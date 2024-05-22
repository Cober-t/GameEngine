#include "TestScript.h"


void TestScript::OnStart(Entity* entity)
{
	std::cout << entity->GetName() << " attached" << std::endl;
}

void TestScript::OnUpdate(Entity* entity, float dt)
{
	// std::cout << "TEST SCRIPT Update" << std::endl;
}

bool TestScript::IsKeyDown(Entity* entity, KeyCode key)
{
	if (key == KeyCode::W)
		std::cout << entity->GetName() << std::endl;
	
	// else if (key == KeyCode::S)
	// 	entity->GetComponent<TransformComponent>().position.x -= m_ExampleIntProperty/2;

	return true;
}