#include <pch.h>
#include "Scene/Systems/ScriptSystem.h"

#include "Core/Input.h"


namespace Cober {

	namespace Script {

		// Internal variables
		static InitScriptsFn m_InitScripts = nullptr;
		static UpdateScriptFn m_UpdateScripts = nullptr;
		static NotifyBeginContactFn m_NotifyBeginContact = nullptr;
		static IsKeyDownFn m_IsKeyDown = nullptr;

		static bool m_IsLoaded = false;
		static HMODULE m_Module;

		// Forward Declarations
		static void InitScriptsStub() {}
		static void UpdateScriptStub() {}
		static void NotifyBeginContactStub(Entity* a, Entity* b) {}
		static bool IsKeyDownStub(KeyCode key) { return true; }

		static FARPROC __stdcall TryLoadFunction(HMODULE module, const char* functionName)
		{
			auto func = GetProcAddress(module, functionName);
			if (func == NULL)
			{
				LOG_WARNING("Could not load dll function '%s'", functionName);
			}

			return func;
		}

		void init()
		{
			reload();
		}

		void update()
		{
			if (m_UpdateScripts)
			{
				m_UpdateScripts();
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

		void reload()
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
				m_IsLoaded = true;

				if (m_InitScripts)
				{
					m_InitScripts();
				}
			}
		}


		bool freeScriptLibrary()
		{
			// TODO: Add way to clear a pool so that we can remove hot reload only script components, while leaving the
			// TODO: rest of the scene intact. This might not be possible, so we shal see 
			if (!m_IsLoaded)
			{
				return true;
			}

			m_InitScripts = InitScriptsStub;
			m_UpdateScripts = UpdateScriptStub;
			m_NotifyBeginContact = NotifyBeginContactStub;
			m_IsKeyDown = IsKeyDownStub;

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
		lua.open_libraries(sol::lib::base, sol::lib::math);
		Script::init();

		LOG_INFO("Script System Added to Registry!!");
	}


	ScriptSystem::~ScriptSystem()
	{
		Script::freeScriptLibrary();
        LOG_INFO("Script System Removed from Registry");
	}


    void ScriptSystem::Start()
    {
		LOG_INFO("Script System Started!!");

    }


    void ScriptSystem::Update(Scene* scene)
    {
		Script::update();

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