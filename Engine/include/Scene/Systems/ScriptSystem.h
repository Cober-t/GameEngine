#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"

#include <sol/sol.hpp>


namespace Cober {

 	typedef void (*InitScriptsFn)(Entity* entity);
	typedef void (*UpdateScriptFn)(float dt);
  	// typedef void (*NotifyBeginContactFn)(Entity* entityA, Entity* entityB);
	typedef bool (*IsKeyDownFn)(KeyCode key);
	// typedef void (*DeleteScriptsFn)();

	namespace ScriptFn
	{
		CB_API void init(Entity* entity);
        CB_API void update(float dt);
		// CB_API void notifyBeginContact(Entity* entityA, Entity* entityB);
		CB_API bool isKeyDown(KeyCode Key);
		// CB_API void deleteScripts();

		CB_API bool freeScriptLibrary();
	}
	

	class CB_API ScriptSystem : public System 
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