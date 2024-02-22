#pragma once

#include "Events/Codes/KeyCodes.h"
#include "Events/Codes/MouseCodes.h"

#include <glm/glm.hpp>

namespace Cober {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}