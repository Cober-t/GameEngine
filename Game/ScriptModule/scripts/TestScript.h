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
	virtual void OnEndContact(Entity* entityContact) override;
	virtual void OnEvent(Event& event) override;
	
public:
	Entity entityAux;
	bool gameStarted = false;
};

#endif