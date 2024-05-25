#include <pch.h>
#include "Scene/Systems/ScriptSystem.h"

#include "Core/Input.h"


namespace Cober {

    ScriptSystem::ScriptSystem()
	{
		LOG_INFO("Script System Added to Registry!!");
	}


	ScriptSystem::~ScriptSystem()
	{
        LOG_INFO("Script System Removed from Registry");
	}


    void ScriptSystem::Start(Scene* scene)
    {
		NativeScriptFn::Init(scene);

		lua.open_libraries(sol::lib::base, sol::lib::math);
		LOG_INFO("Script System Started!!");

    }


    void ScriptSystem::Update(Scene* scene, float dt)
    {
		NativeScriptFn::Update(scene, dt);

		// LUA in the future
		// auto view = scene->GetAllEntitiesWith<ScriptComponent>();
		// for (auto entt : view) 
        // {
		// 	Entity entity = Entity((entt::entity)entt, scene );
		// 	// std::vector<sol::function> scriptsList = entity.GetComponent<ScriptComponent>().scripts;

		// 	// for (sol::function script : scriptsList) 
		// 	// {
		// 	// 	// if (script != sol::lua_nil)
		// 	// 		// script.func();	// Inoke lua function
		// 	// }
		// 	std::filesystem::path scriptPath = std::filesystem::current_path() / "assets/scripts";
		// 	lua.script_file(scriptPath.string() + "scriptTest.lua");
		// 	sol::function addFunction = lua["add"];
		// 	int sum = addFunction(5, 6);

		// 	std::cout << "Result: " << sum << std::endl;
		// }
    }


	void ScriptSystem::FreeScripts(Scene* scene)
	{
		NativeScriptFn::DeleteScripts(scene);
	}
}