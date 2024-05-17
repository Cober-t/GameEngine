#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"

#include <sol/sol.hpp>


namespace Cober {

 	typedef void (*InitScriptsFn)();
	typedef void (*UpdateScriptFn)();
  	typedef void (*NotifyBeginContactFn)(Entity* entityA, Entity* entityB);
	typedef bool (*IsKeyDownFn)(KeyCode key);

	namespace Script 
	{
		void init();
        void update();
		void notifyBeginContact(Entity* entityA, Entity* entityB);
		bool isKeyDown(KeyCode Key);

		void reload();
		bool freeScriptLibrary();
	}
	

	class ScriptSystem : public System 
    {
	public:
		ScriptSystem();
		~ScriptSystem();

        void Start();
		void Update(Scene* scene);

    private:
		sol::state lua;
		Scene* m_Scene = nullptr;
	};
}

#endif