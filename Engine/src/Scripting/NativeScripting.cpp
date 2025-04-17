#include <pch.h>
#include "Scripting/NativeScripting.h"

namespace Cober {

    namespace NativeScriptFn {
    
	    // Internal variables
		static InitScriptsFn m_InitScripts = nullptr;
		static UpdateScriptFn m_UpdateScripts = nullptr;
		static EventScriptFn m_EventScripts = nullptr;
		static NotifyBeginContactFn m_NotifyBeginContact = nullptr;
		static NotifyEndContactFn m_NotifyEndContact = nullptr;
		static DeleteScriptsFn m_DeleteScripts = nullptr;

		static bool m_IsLoaded = false;
		static HMODULE m_Module;

		// Forward Declarations
		static void InitScriptsStub(Scene* scene, Entity entity) {}
		static void UpdateScriptStub(Scene* scene, float dt) {}
		static void EventScriptStub(Scene* scene, Event& event) {}
		static void NotifyBeginContactScriptStub(Entity* a, Entity* b) {}
		static void NotifyEndContactScriptStub(Entity* a, Entity* b) {}
		static void DeleteScriptsStub(Scene* scene) {}

		static FARPROC __stdcall TryLoadFunction(HMODULE module, const char* functionName)
		{
			auto func = GetProcAddress(module, functionName);
			if (func == NULL)
			{
				LOG_WARNING("Could not load dll function '{0}'", functionName);
			}

			return func;
		}

		void Init(Scene* scene)
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
				m_EventScripts = (EventScriptFn)TryLoadFunction(m_Module, "EventScripts");
				m_NotifyBeginContact = (NotifyBeginContactFn)TryLoadFunction(m_Module, "NotifyBeginContact");
				m_NotifyEndContact = (NotifyEndContactFn)TryLoadFunction(m_Module, "NotifyEndContact");
				m_DeleteScripts = (DeleteScriptsFn)TryLoadFunction(m_Module, "DeleteScripts");
				m_IsLoaded = true;

				if (m_InitScripts)
				{
					auto view = scene->GetAllEntitiesWith<NativeScriptComponent>();
					for (auto entt : view)
					{
						Entity entity = Entity(entt, scene);
						m_InitScripts(scene, entity);
					}
				}
			}
		}

		void InitEntity(Scene* scene, Entity entity)
		{
			if (m_InitScripts)
			{
				m_InitScripts(scene, entity);
			}
		}

		void Update(Scene* scene, float dt)
		{
			if (m_UpdateScripts)
			{
				m_UpdateScripts(scene, dt);
			}
		}
		
		void OnEvent(Scene* scene, Event& event)
		{
			if (m_EventScripts)
			{
				m_EventScripts(scene, event);
			}
		}

		void NotifyBeginContact(Entity* entityA, Entity* entityB)
		{
			if (m_NotifyBeginContact)
			{
				m_NotifyBeginContact(entityA, entityB);
			}
		}

		void NotifyEndContact(Entity* entityA, Entity* entityB)
		{
			if (m_NotifyEndContact)
			{
				m_NotifyEndContact(entityA, entityB);
			}
		}

		void DeleteScripts(Scene* scene)
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
			m_EventScripts = EventScriptStub;
			m_NotifyBeginContact = NotifyBeginContactScriptStub;
			m_NotifyEndContact = NotifyEndContactScriptStub;
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