#include <pch.h>
#include "Scene/Systems/ScriptSystem.h"

#include "Core/Input.h"


namespace Cober {

	namespace ScriptFn {

		// Internal variables
		static InitScriptsFn m_InitScripts = nullptr;
		static UpdateScriptFn m_UpdateScripts = nullptr;
		static NotifyBeginContactFn m_NotifyBeginContact = nullptr;
		static IsKeyDownFn m_IsKeyDown = nullptr;
		static DeleteScriptsFn m_DeleteScripts = nullptr;

		static bool m_IsLoaded = false;
		static HMODULE m_Module;

		// Forward Declarations
		static void InitScriptsStub(Entity* entity) {}
		static void UpdateScriptStub(float dt) {}
		static void NotifyBeginContactStub(Entity* a, Entity* b) {}
		static bool IsKeyDownStub(KeyCode key) { return true; }
		static void DeleteScriptsStub() { }

		static FARPROC __stdcall TryLoadFunction(HMODULE module, const char* functionName)
		{
			auto func = GetProcAddress(module, functionName);
			if (func == NULL)
			{
				LOG_WARNING("Could not load dll function '%s'", functionName);
			}

			return func;
		}

		void init(Entity* entity)
		{
			if (m_IsLoaded)
			{
				if (!freeScriptLibrary()) return;
			}
			m_Module = LoadLibraryA("ScriptModule.dll");

			if (m_Module)
			{
				m_InitScripts = (InitScriptsFn)TryLoadFunction(m_Module, "InitScripts");
				m_UpdateScripts = (UpdateScriptFn)TryLoadFunction(m_Module, "UpdateScripts");
				m_NotifyBeginContact = (NotifyBeginContactFn)TryLoadFunction(m_Module, "NotifyBeginContact");
				m_IsKeyDown = (IsKeyDownFn)TryLoadFunction(m_Module, "IsKeyDown");
				m_DeleteScripts = (DeleteScriptsFn)TryLoadFunction(m_Module, "DeleteScripts");
				m_IsLoaded = true;

				if (m_InitScripts)
				{
					m_InitScripts(entity);
				}
			}
		}

		void update(float dt)
		{
			if (m_UpdateScripts)
			{
				m_UpdateScripts(dt);
			}
		}

		void notifyBeginContact(Entity* entityA, Entity* entityB)
		{
			if (m_NotifyBeginContact)
			{
				m_NotifyBeginContact(entityA, entityB);
			}
		}

		bool isKeyDown(KeyCode key)
		{
			if (m_IsKeyDown)
			{
				return m_IsKeyDown(key);
			}

			return false;
		}

		void deleteScripts()
		{
			if (m_DeleteScripts)
			{
				m_DeleteScripts();
			}
		}


		bool freeScriptLibrary()
		{
			if (!m_IsLoaded)
			{
				return true;
			}

			m_InitScripts = InitScriptsStub;
			m_UpdateScripts = UpdateScriptStub;
			m_NotifyBeginContact = NotifyBeginContactStub;
			m_IsKeyDown = IsKeyDownStub;
			m_DeleteScripts = DeleteScriptsStub;

			if (!FreeLibrary(m_Module))
			{
				LOG_WARNING("Could not free script dll. Error Code:");
				return false;
			}

			m_Module = nullptr;
			m_IsLoaded = false;
			return true;
		}
	}


    ScriptSystem::ScriptSystem()
	{
		LOG_INFO("Script System Added to Registry!!");
	}


	ScriptSystem::~ScriptSystem()
	{
		ScriptFn::deleteScripts();
		ScriptFn::freeScriptLibrary();
        LOG_INFO("Script System Removed from Registry");
	}


    void ScriptSystem::Start(Scene* scene)
    {
		auto view = scene->GetAllEntitiesWith<NativeScriptComponent>();
		for (auto entt : view) 
        {
			Entity entity = Entity((entt::entity)entt, scene );
			ScriptFn::init(&entity);
		}

		lua.open_libraries(sol::lib::base, sol::lib::math);
		LOG_INFO("Script System Started!!");

    }


    void ScriptSystem::Update(float dt)
    {
		ScriptFn::update(dt);

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
}