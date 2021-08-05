#pragma once

#include "Core.h"

namespace GameEngine {

	class GameEngine_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT (SandBox)
	Application* CreateApplication();

}
