#include <pch.h>
#include "Scene/Systems/ScriptSystem.h"

namespace Cober {


    ScriptSystem::ScriptSystem()
	{
		lua.open_libraries(sol::lib::base, sol::lib::math);

		LOG_INFO("Script System Added to Registry!!");
	}


	ScriptSystem::~ScriptSystem()
	{
        LOG_INFO("Script System Removed from Registry");
	}


    void ScriptSystem::Start()
    {
		LOG_INFO("Script System Started!!");
    }


    void ScriptSystem::Update(Scene* scene)
    {
		auto view = scene->GetAllEntitiesWith<ScriptComponent>();
		for (auto entt : view) 
        {
			Entity entity = Entity((entt::entity)entt, scene );
			// std::vector<sol::function> scriptsList = entity.GetComponent<ScriptComponent>().scripts;

			// for (sol::function script : scriptsList) 
			// {
			// 	// if (script != sol::lua_nil)
			// 		// script.func();	// Inoke lua function
			// }
			lua.script_file(SCRIPTS_DIR + (std::string)"scriptTest.lua");
			sol::function addFunction = lua["add"];
			int sum = addFunction(5, 6);

			std::cout << "Result: " << sum << std::endl;
		}
    }
}