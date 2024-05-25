#include <pch.h>
#include "Scripting/NativeScripting.h"

namespace Cober {

    namespace NativeScriptFn {
    
	    // Internal variables
		static InitScriptsFn m_InitScripts = nullptr;
		static UpdateScriptFn m_UpdateScripts = nullptr;
		static NotifyBeginContactFn m_NotifyBeginContact = nullptr;
		static DeleteScriptsFn m_DeleteScripts = nullptr;

		static bool m_IsLoaded = false;
		static HMODULE m_Module;

		// Forward Declarations
		static void InitScriptsStub(Ref<Scene> scene) {}
		static void UpdateScriptStub(Ref<Scene> scene, float dt) {}
		static void NotifyBeginContactStub(Entity* a, Entity* b) {}
		static void DeleteScriptsStub(Ref<Scene> scene) { }

		static FARPROC __stdcall TryLoadFunction(HMODULE module, const char* functionName)
		{
			auto func = GetProcAddress(module, functionName);
			if (func == NULL)
			{
				LOG_WARNING("Could not load dll function '%s'", functionName);
			}

			return func;
		}

		void Init(Ref<Scene> scene)
		{
			if (m_IsLoaded)
			{
				if (!FreeScriptLibrary()) return;
			}
			m_Module = LoadLibraryA("ScriptModule.dll");

			if (m_Module)
			{
				m_InitScripts = (InitScriptsFn)TryLoadFunction(m_Module, "InitScripts");
				m_UpdateScripts = (UpdateScriptFn)TryLoadFunction(m_Module, "UpdateScripts");
				m_NotifyBeginContact = (NotifyBeginContactFn)TryLoadFunction(m_Module, "NotifyBeginContact");
				m_DeleteScripts = (DeleteScriptsFn)TryLoadFunction(m_Module, "DeleteScripts");
				m_IsLoaded = true;

				if (m_InitScripts)
				{
					m_InitScripts(scene);
				}
			}
		}

		void Update(Ref<Scene> scene, float dt)
		{
			if (m_UpdateScripts)
			{
				m_UpdateScripts(scene, dt);
			}
		}

		void NotifyBeginContact(Entity* entityA, Entity* entityB)
		{
			if (m_NotifyBeginContact)
			{
				m_NotifyBeginContact(entityA, entityB);
			}
		}

		void DeleteScripts(Ref<Scene> scene)
		{
			if (m_DeleteScripts)
			{
				m_DeleteScripts(scene);
			}
		}


		bool FreeScriptLibrary()
		{
			if (!m_IsLoaded)
			{
				return true;
			}

			m_InitScripts = InitScriptsStub;
			m_UpdateScripts = UpdateScriptStub;
			m_NotifyBeginContact = NotifyBeginContactStub;
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
}