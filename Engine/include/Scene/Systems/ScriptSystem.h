#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"

#include <sol/sol.hpp>


namespace Cober {

 	typedef void (*InitScriptsFn)(Entity* entity);
	typedef void (*UpdateScriptFn)(float dt);
  	typedef void (*NotifyBeginContactFn)(Entity* entityA, Entity* entityB);
	typedef bool (*IsKeyDownFn)(KeyCode key);
	typedef void (*DeleteScriptsFn)();

	namespace ScriptFn
	{
		void init(Entity* entity);
        void update(float dt);
		void notifyBeginContact(Entity* entityA, Entity* entityB);
		bool isKeyDown(KeyCode Key);
		void deleteScripts();

		bool freeScriptLibrary();
	}
	

	class ScriptSystem : public System 
    {
	public:
		ScriptSystem();
		~ScriptSystem();

        void Start(Scene* scene);
		void Update(float dt);

    private:
		sol::state lua;
		Scene* m_Scene;
	};
}

#endif