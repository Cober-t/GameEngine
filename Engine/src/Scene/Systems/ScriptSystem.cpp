#include <pch.h>
#include "Scene/Systems/ScriptSystem.h"

namespace Cober {


    ScriptSystem::ScriptSystem(Scene* scene) : m_Scene(scene)
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


    void ScriptSystem::Update()
    {
		for (auto& entity : GetSystemEntities()) 
        {
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


	std::vector<Entity> ScriptSystem::GetSystemEntities() const
	{
		std::vector<Entity> entities;
		auto entitiesView = m_Scene->GetAllEntitiesWith<ScriptComponent>();

		for (auto entity : entitiesView)
			entities.emplace_back( Entity{ entity, m_Scene });

		return entities;
	};
}