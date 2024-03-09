#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H

#include "Core/Core.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"

#include <sol/sol.hpp>

namespace Cober {

	class ScriptSystem : public System 
    {
	public:
		ScriptSystem() = default;
		ScriptSystem(Scene* scene);
		~ScriptSystem();

        void Start();
		void Update();

		virtual std::vector<Entity> GetSystemEntities() const override;

    private:
		sol::state lua;
		Scene* m_Scene = nullptr;
	};
}

#endif