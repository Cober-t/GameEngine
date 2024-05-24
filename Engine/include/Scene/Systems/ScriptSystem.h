#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"

#include <sol/sol.hpp>


namespace Cober {

 	typedef void (*InitScriptsFn)(Ref<Scene> scene, Entity* entity);
	typedef void (*UpdateScriptFn)(Ref<Scene> scene, float dt);
  	typedef void (*NotifyBeginContactFn)(Entity* entityA, Entity* entityB);
	typedef bool (*IsKeyDownFn)(KeyCode key);
	typedef void (*DeleteScriptsFn)(Ref<Scene> scene);

	namespace ScriptFn
	{
		CB_API void init(Ref<Scene> scene, Entity* entity);
        CB_API void update(Ref<Scene> scene, float dt);
		CB_API void notifyBeginContact(Entity* entityA, Entity* entityB);
		CB_API bool isKeyDown(KeyCode Key);
		CB_API void deleteScripts(Ref<Scene> scene);

		CB_API bool freeScriptLibrary();
	}
	

	class CB_API ScriptSystem : public System 
    {
	public:
		ScriptSystem();
		~ScriptSystem();

        void Start(Ref<Scene> scene);
		void Update(Ref<Scene> scene, float dt);
		void FreeScripts(Ref<Scene> scene);

    private:
		sol::state lua;
	};
}

#endif