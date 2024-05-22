#ifndef INIT_SCRIPTS_H
#define INIT_SCRIPTS_H
#define COBER_SCRIPT __declspec(dllexport)

#include <Windows.h>
#include <iostream>
#include <cstdlib>

#include "Core/Core.h"
#include "Scene/ECS.h"

#include "TestScript_generated.h"

#define ENTT_STANDARD_CPP
#include <entt/entt.hpp>


namespace Cober {

	entt::registry registry = entt::registry();

	extern "C" COBER_SCRIPT void InitScripts(Entity* entity)
	{
		if (entity->GetComponent<NativeScriptComponent>().className == "TestScript")
		{
			registry.emplace<TestScript>(entity->GetHandle());
			auto scriptHandle = registry.get<TestScript>(entity->GetHandle());
			scriptHandle.OnStart();
		}
	}

	extern "C" COBER_SCRIPT void UpdateScripts(float dt)
	{
		auto view = registry.view<TestScript>();

		for (auto entt : view)
		{
			auto enttScriptHandle = registry.get<TestScript>(entt);
			enttScriptHandle.OnUpdate(dt);
		}
	}

	extern "C" COBER_SCRIPT void NotifyBeginContact(Entity* entityA, Entity* entityB)
	{
		// std::cout << entityB->GetName() << " BEGIN CONTACT" << std::endl;
	}

	extern "C" COBER_SCRIPT void NotifyEndContact(Entity* entityA, Entity* entityB)
	{
	}

	extern "C" COBER_SCRIPT bool IsKeyDown(KeyCode key)
	{
		auto view = registry.view<TestScript>();

		for (auto entt : view)
		{
			auto enttScriptHandle = registry.get<TestScript>(entt);
			enttScriptHandle.IsKeyDown(key);
		}
		return true;
	}

	extern "C" COBER_SCRIPT void DeleteScripts()
	{
		registry.clear<TestScript>();
	}


	// extern "C" COBER_SCRIPT void EditorUpdateScripts(entt::registry& registryRef, float dt)
	// {
	// }


	// extern "C" COBER_SCRIPT void SaveScripts(entt::registry& registryRef, json& j, SceneData* sceneData)
	// {
		// Log::Info("Saving scripts");
	// }

	// extern "C" COBER_SCRIPT void LoadScript(entt::registry& registryRef, const json& j, Entity entity)
	// {
	// }


	// extern "C" COBER_SCRIPT void InitImGui(void* ctx)
	// {
	// 	// Log::Info("Initializing ImGui");
	// 	ImGui::SetCurrentContext((ImGuiContext*)ctx);
	// }

	// extern "C" COBER_SCRIPT void ImGui(entt::registry& registryRef, Entity entity)
	// {
	// }

	// extern "C" COBER_SCRIPT void DeleteScripts()
	// {
	// 	// Log::Info("Deletink Scripts");
	// }
}

#endif
