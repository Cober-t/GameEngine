#ifndef TEST_SCRIPT_SCRIPT_H
#define TEST_SCRIPT_SCRIPT_H

#include <Engine.h>

using namespace Cober;


class TestScript : public Script
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnBeginContact(Entity* entityContact) override;
	
public:
	float exampleVariable = 2.8;
};

#endif