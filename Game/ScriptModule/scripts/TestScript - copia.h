#ifndef TEST_SCRIPT_H
#define TEST_SCRIPT_H

#include "Scene/Script.h"

using namespace Cober;

class TestScript : public Script
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual bool IsKeyDown(KeyCode key) override;

public:
	float m_ExampleFloatProperty = 3.14f;
	int m_ExampleIntProperty = 10;
};

#endif