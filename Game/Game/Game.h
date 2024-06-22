#ifndef Game_H
#define Game_H

#include <Engine.h>

using namespace Cober;


class Game : public Layer 
{
public:
	Game();
	virtual ~Game() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Unique<Timestep>& ts) override;
	void OnEvent(Event& event) override;
	
private:
	Ref<Scene> m_ActiveScene;
	Ref<Camera> m_DefaultCamera;
};

#endif
