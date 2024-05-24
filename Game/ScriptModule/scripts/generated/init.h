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

	entt::registry registry = entt::registry();

	extern "C" CB_SCRIPT void InitScripts(Ref<Scene> scene, Entity* entity)
	{
		if (entity->GetComponent<NativeScriptComponent>().className == "TestScript")
		{

			registry.emplace<TestScript>(entity->GetHandle());
			auto scriptHandle = registry.get<TestScript>(entity->GetHandle());
			scriptHandle.OnStart();

			// entity->AddComponent<TestScript>();
			// entity->GetComponent<TestScript>().entity = entity;
			// entity->GetComponent<TestScript>().OnStart();
		}
	}

	extern "C" CB_SCRIPT void UpdateScripts(Ref<Scene> scene, float dt)
	{
		auto view = registry.view<TestScript>();

		for (auto entity : view)
		{
			auto comp = registry.get<TestScript>(entity);
			comp.OnUpdate(dt);
		}

		if (Input::IsKeyDown(KeyCode::A))
		{
			scene->FindEntityByName("Entity0").GetComponent<TransformComponent>().position.x -= 0.01 * dt;
		}
		if (Input::IsKeyDown(KeyCode::D))
		{
			scene->FindEntityByName("Entity0").GetComponent<TransformComponent>().position.x += 0.01 * dt;
		}
		if (Input::IsKeyDown(KeyCode::W))
		{
			scene->FindEntityByName("Entity0").GetComponent<TransformComponent>().position.y += 0.01 * dt;
		}
		if (Input::IsKeyDown(KeyCode::S))
		{
			scene->FindEntityByName("Entity0").GetComponent<TransformComponent>().position.y -= 0.01 * dt;
		}
	}

	extern "C" CB_SCRIPT void NotifyBeginContact(Entity* entityA, Entity* entityB)
	{
		// std::cout << entityB->GetName() << " BEGIN CONTACT" << std::endl;
	}

	// extern "C" CB_SCRIPT void NotifyEndContact(Entity* entityA, Entity* entityB)
	// {
	// }

	extern "C" CB_SCRIPT bool IsKeyDown(KeyCode key)
	{
		// if (Input::IsKeyDown(Key::A))
		// {
		// 	std::cout << "A PRESSED" << std::endl;
		// }
		// else
		
		// {
		// 	std::cout << "----" << std::endl;
		// }
		return true;
	}

	extern "C" CB_SCRIPT void DeleteScripts(Ref<Scene> scene)
	{
		registry.clear<TestScript>();
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
