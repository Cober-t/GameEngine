#ifndef GAME_H
#define GAME_H

#include <Engine.h>


namespace Cober {

	class Game : public Layer 
	{
	public:
		Game();
		virtual ~Game() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		// void OnEvent(SDL_Event& event) override;
	private:
		Ref<Scene> m_ActiveScene;
		Ref<GameCamera> m_Camera;
	};
}

#endif