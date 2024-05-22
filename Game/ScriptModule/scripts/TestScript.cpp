#include "TestScript.h"


void TestScript::OnStart()
{
	std::cout << "Entity Script Init" << std::endl;
}

void TestScript::OnUpdate(float dt)
{
	// std::cout << "Entity Script UPDATE" << std::endl;
}

bool TestScript::IsKeyDown(KeyCode key)
{
	std::cout << key << " pressed!" << std::endl;
	// if (key == KeyCode::W)
	// 	std::cout << entity->GetComponent<TransformComponent>().position << std::endl;
	
	// else if (key == KeyCode::S)
	// 	entity->GetComponent<TransformComponent>().position.x -= m_ExampleIntProperty/2;

	return true;
}