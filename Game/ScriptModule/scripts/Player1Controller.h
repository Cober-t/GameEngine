#ifndef PLAYER_1_CONTROLLER_SCRIPT_H
#define PLAYER_1_CONTROLLER_SCRIPT_H

#include <Engine.h>

using namespace Cober;


class Player1Controller : public Script
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnBeginContact(Entity* entityContact) override;

public:
	float upperLimit = 2.8;
	float bottomLimit = -2.8;
};

#endif