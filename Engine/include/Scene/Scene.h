#ifndef SCENE_H
#define SCENE_H

#include "Core/Timestep.h"
#include "Core/UUID.h"
#include "Render/Camera/GameCamera.h"

#include <entt/entt.hpp>
#include <typeindex>


//class b2World;

namespace Cober {

	class Entity;
	class System;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateSimulation(Timestep ts, const Ref<GameCamera>& camera);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }

		void Step(int frames = 1);

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

        // System management
		template<typename T, typename ...TArgs> 			
        void AddSystem(TArgs&& ...args);

		template<typename T>		
        void RemoveSystem();

		template<typename T>		
        bool HasSystem() const;

		template<typename T>		
        T& GetSystem() const;

		void AddEntityToSystems(Entity entity);
		void RemoveEntityFromSystems(Entity entity);

        std::map<std::type_index, Ref<System>> GetSystems() { return m_Systems; }

	private:
		entt::registry m_Registry;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		int m_StepFrames = 0;

        std::map<std::type_index, Ref<System>> m_Systems;
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity;
		friend class System;
		//friend class SceneSerializer;
		//friend class SceneHierarchyPanel;
	};
}

#endif