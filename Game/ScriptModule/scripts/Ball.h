#ifndef GAME_MANAGER_SCRIPT_H
#define GAME_MANAGER_SCRIPT_H

#include <Engine.h>
#include <glm/glm.hpp>

using namespace Cober;


class Ball : public Script
{
public:
	virtual void OnStart() override;
	virtual void OnUpdate(float dt) override;
	virtual void OnBeginContact(Entity* entityContact) override;

public:
	bool m_MatchStarted = false;
	int m_Score = 3;
	bool m_ChangeColors = false;
	int m_Direction = -1;

	float upperLimit = 3;
	float bottomLimit = -3;
	float speed = 1.8f;

	Entity player1;
	Entity player2;
};

#endif