#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"

#include "Scripting/NativeScripting.h"

#include <sol/sol.hpp>


namespace Cober {

	class CB_API ScriptSystem : public System 
    {
	public:
		ScriptSystem();
		~ScriptSystem();

        void Start(Scene* scene);
		void Update(Scene* scene, float dt);
		void FreeScripts(Scene* scene);

    private:
		sol::state lua;
	};
}

#endif