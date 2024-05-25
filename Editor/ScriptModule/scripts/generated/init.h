#ifndef INIT_SCRIPTS_H
#define INIT_SCRIPTS_H

#include <Engine.h>
// #include <Windows.h>
#include <iostream>
#include <cstdlib>

#include "Core/Core.h"
#include "Scene/ECS.h"

#include "TestScript_generated.h"

#define ENTT_STANDARD_CPP
#include <entt/entt.hpp>


namespace Cober {

	extern "C" CB_SCRIPT void InitScripts(Scene* scene)
	{
		auto view = scene->GetAllEntitiesWith<NativeScriptComponent>();
		// for (auto entt : view) 
        // {
		// 	Entity entity = Entity(entt, scene);
		// 	entity.AddComponent<TestScript>();
		// 	entity.GetComponent<TestScript>().entity = entity;
		// 	entity.GetComponent<TestScript>().OnStart();
		// }
	}

	extern "C" CB_SCRIPT void UpdateScripts(Scene* scene, float dt)
	{
		// auto view = scene->GetAllEntitiesWith<TestScript>();

		// for (auto entt : view)
		// {
		// 	Entity entity = Entity(entt, scene);
		// 	entity.GetComponent<TestScript>().OnUpdate(dt);
		// }
	}

	extern "C" CB_SCRIPT void NotifyBeginContact(Entity* entityA, Entity* entityB)
	{
		// std::cout << entityB->GetName() << " BEGIN CONTACT" << std::endl;
	}

	// extern "C" CB_SCRIPT void NotifyEndContact(Entity* entityA, Entity* entityB)
	// {
	// }


	extern "C" CB_SCRIPT void DeleteScripts(Scene* scene)
	{
		// scene->GetRegistry()->clear<TestScript>();
	}


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
