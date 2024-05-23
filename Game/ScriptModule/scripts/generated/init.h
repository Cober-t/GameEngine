#ifndef INIT_SCRIPTS_H
#define INIT_SCRIPTS_H

#include <Engine.h>
// #include <Windows.h>
#include <iostream>
#include <cstdlib>

#include "Core/Core.h"
// #include "Scene/ECS.h"

#include "TestScript_generated.h"

#define ENTT_STANDARD_CPP
#include <entt/entt.hpp>




namespace Cober {

	extern "C" CB_SCRIPT void InitScripts(Entity* entity)
	{
		// if (entity->GetComponent<NativeScriptComponent>().className == "TestScript")
		// {
			// registry.emplace<TestScript>(entity->GetHandle());
			// auto scriptHandle = registry.get<TestScript>(entity->GetHandle());
			// scriptHandle.OnStart();
		// }
	}

	extern "C" CB_SCRIPT void UpdateScripts(float dt)
	{
		// auto view = registry.view<TestScript>();

		// for (auto entt : view)
		// {
		// 	auto enttScriptHandle = registry.get<TestScript>(entt);
		// 	enttScriptHandle.OnUpdate(dt);
		// }
	}

	// extern "C" CB_API void NotifyBeginContact(Entity* entityA, Entity* entityB)
	// {
	// 	// std::cout << entityB->GetName() << " BEGIN CONTACT" << std::endl;
	// }

	// extern "C" CB_API void NotifyEndContact(Entity* entityA, Entity* entityB)
	// {
	// }

	extern "C" CB_SCRIPT bool IsKeyDown(KeyCode key)
	{
		if (Input::IsKeyDown(Key::A))
		{
			std::cout << "A PRESSED" << std::endl;
		}
		else
		
		{
			std::cout << "----" << std::endl;
		}
		// auto view = registry.view<TestScript>();

		// for (auto entt : view)
		// {
		// 	auto enttScriptHandle = registry.get<TestScript>(entt);
		// 	enttScriptHandle.IsKeyDown(key);
		// }
		return true;
	}

	// extern "C" CB_API void DeleteScripts()
	// {
	// }


	// extern "C" CB_SCRIPT void EditorUpdateScripts(entt::registry& registryRef, float dt)
	// {
	// }


	// extern "C" CB_SCRIPT void SaveScripts(entt::registry& registryRef, json& j, SceneData* sceneData)
	// {
		// Log::Info("Saving scripts");
	// }

	// extern "C" CB_SCRIPT void LoadScript(entt::registry& registryRef, const json& j, Entity entity)
	// {
	// }


	// extern "C" CB_SCRIPT void InitImGui(void* ctx)
	// {
	// 	// Log::Info("Initializing ImGui");
	// 	ImGui::SetCurrentContext((ImGuiContext*)ctx);
	// }

	// extern "C" CB_SCRIPT void ImGui(entt::registry& registryRef, Entity entity)
	// {
	// }

	// extern "C" CB_SCRIPT void DeleteScripts()
	// {
	// 	// Log::Info("Deletink Scripts");
	// }
}

#endif
