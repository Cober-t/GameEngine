#ifndef SCENE_H
#define SCENE_H

#include "Core/Timestep.h"
#include "Core/UUID.h"
#include "Core/Utils.h"

#include "Scene/Components.h"

#include <entt/entt.hpp>
#include <typeindex>


namespace Cober {

	class Entity;
	class ScriptableEntity;
	class System;

	class CB_API Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateRuntime(Unique<Timestep>& ts, Ref<Camera>& camera);
		void OnUpdateSimulation(Unique<Timestep>& ts, Ref<Camera>& camera);

		template<typename... Components>
		inline auto GetAllEntitiesWith() { return m_Registry.view<Components...>(); }
		inline entt::registry* GetRegistry() { return &m_Registry; }
		const std::unordered_map<UUID, Entity>& GetEntityMap() { return m_EntityMap; };
		inline uint64_t GetEntityCount() { return m_EntityMap.size(); };
		
		std::vector<Entity> GetSceneEntities();

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);
		Entity DuplicateEntity(Entity entity);

		void Step(int step = 1) { m_StepFrames = step; };
		void Pause() { m_IsPaused = m_IsPaused == true ? false : true; };
		bool ExitFromRuntimeEditor() { return m_ExitFromRuntimeEditor; };

		static bool Save(const Ref<Scene>& scene, std::string sceneName = "Scene1");
		static void Exit(Scene* scene);

		static void Reload(Scene* scene, std::string name);
		static Ref<Scene> Load(std::string scenePath = "SceneDefault.lua");
		static Entity LoadPrefab(Scene* currentScene, std::string prefabName = "EntityDefault.lua");
		static Ref<Scene> Copy(Ref<Scene> scene);

	private:

		template<typename T, typename ...TArgs> 			
        void AddSystem(TArgs&& ...args);

		template<typename T>
        void RemoveSystem();

		template<typename T>
        bool HasSystem() const;

		template<typename T>
        T& GetSystem() const;

        std::map<std::type_index, Ref<System>> GetSystems() { return m_Systems; }
		void CleanUp();

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        std::map<std::type_index, Ref<System>> m_Systems;
		std::unordered_map<UUID, Entity> m_EntityMap;
		std::vector<Entity> m_EntitiesToBeDestroyed;

		int m_StepFrames;
		std::string m_SceneName;
		bool m_IsPaused = false;
		bool m_ReloadScripts = false;
		bool m_ExitFromRuntimeEditor = false;

		friend class Entity;
		friend class System;
	};
}

#endif