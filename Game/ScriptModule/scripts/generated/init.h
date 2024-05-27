#ifndef INIT_SCRIPTS_H
#define INIT_SCRIPTS_H

#include <Engine.h>
// #include <Windows.h>
#include <iostream>
#include <cstdlib>

#include "Core/Core.h"
#include "Scene/ECS.h"

#include "../Player1Controller.h"
#include "../Player2Controller.h"
#include "../Ball.h"

#define ENTT_STANDARD_CPP
#include <entt/entt.hpp>


namespace Cober {

	extern "C" CB_SCRIPT void InitScripts(Scene* scene, Entity entity)
	{
		if (entity.GetComponent<NativeScriptComponent>().className == "Player1Controller")
		{
			entity.AddComponent<Player1Controller>();
			entity.GetComponent<Player1Controller>().entity = entity;
			entity.GetComponent<Player1Controller>().scene = scene;
			entity.GetComponent<Player1Controller>().OnStart();
		}

		else if (entity.GetComponent<NativeScriptComponent>().className == "Player2Controller")
		{
			entity.AddComponent<Player2Controller>();
			entity.GetComponent<Player2Controller>().entity = entity;
			entity.GetComponent<Player2Controller>().scene = scene;
			entity.GetComponent<Player2Controller>().OnStart();
		}

		else if (entity.GetComponent<NativeScriptComponent>().className == "Ball")
		{
			entity.AddComponent<Ball>();
			entity.GetComponent<Ball>().entity = entity;
			entity.GetComponent<Ball>().scene = scene;
			entity.GetComponent<Ball>().OnStart();
		}
	}

	extern "C" CB_SCRIPT void UpdateScripts(Scene* scene, float dt)
	{
		auto player1ScriptView = scene->GetAllEntitiesWith<Player1Controller>();

		for (auto entt : player1ScriptView)
		{
			Entity entity = Entity(entt, scene);
			entity.GetComponent<Player1Controller>().OnUpdate(dt);
		}

		auto player2ScriptView = scene->GetAllEntitiesWith<Player2Controller>();

		for (auto entt : player2ScriptView)
		{
			Entity entity = Entity(entt, scene);
			entity.GetComponent<Player2Controller>().OnUpdate(dt);
		}

		auto ballScriptView = scene->GetAllEntitiesWith<Ball>();

		for (auto entt : ballScriptView)
		{
			Entity entity = Entity(entt, scene);
			entity.GetComponent<Ball>().OnUpdate(dt);
		}
	}

	extern "C" CB_SCRIPT void NotifyBeginContact(Entity* entityA, Entity* entityB)
	{
		if (entityA->HasComponent<Ball>())
			entityA->GetComponent<Ball>().OnBeginContact(entityB);
		else
			entityB->GetComponent<Ball>().OnBeginContact(entityA);
	}

	// extern "C" CB_SCRIPT void NotifyEndContact(Entity* entityA, Entity* entityB)
	// {
	// }


	extern "C" CB_SCRIPT void DeleteScripts(Scene* scene)
	{
		scene->GetRegistry()->clear<Player1Controller>();
		scene->GetRegistry()->clear<Player2Controller>();
		scene->GetRegistry()->clear<Ball>();
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

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpvReserved )  // reserved
	{
		// Perform actions based on the reason for calling.
		switch( fdwReason ) 
		{ 
			case DLL_PROCESS_ATTACH:
			// Initialize once for each new process.
			// Return FALSE to fail DLL load.
				break;

			case DLL_THREAD_ATTACH:
			// Do thread-specific initialization.
				break;

			case DLL_THREAD_DETACH:
			// Do thread-specific cleanup.
				break;

			case DLL_PROCESS_DETACH:
			
				if (lpvReserved != nullptr)
				{
					break; // do not do cleanup if process termination scenario
				}
				
			// Perform any necessary cleanup.
				break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

#endif
