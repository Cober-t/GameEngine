#ifndef SCRIPT_H
#define SCRIPT_H
#include "Events/KeyCodes.h"
#include "Scene/ECS.h"

namespace Cober {

	class Script
	{
	public:		
		virtual void OnStart() = 0;
		// virtual void OnEditorUpdate(Entity entity, float dt) = 0;
		// virtual void OnImGuiUpdate(Entity entity) = 0;

		virtual void OnUpdate(float dt) = 0;

		// virtual void OnBeginContact(Entity other) = 0;
		// virtual void OnEndContact(Entity other) = 0;
        virtual bool IsKeyDown(KeyCode key) = 0;
	};
}

#endif