#ifndef ENTITY_H
#define ENTITY_H

#include "Core/UUID.h"
#include "Components.h"
#include "Scene.h"

#include <entt/entt.hpp>


const unsigned int MAX_COMPONENTS = 32;

//// SIGNATURE
// We use a bitset (1s and 0s) to keep track of which components an entity has
// and also keep track of which entities a system is interested in.
typedef std::bitset<MAX_COMPONENTS> Signature;

namespace Cober {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename TComponent, typename... Args>
		TComponent& AddComponent(Args&&... args)
		{
			LOG_CORE_ASSERT(!HasComponent<TComponent>(), "Entity already has component!");
			TComponent& component = m_Scene->m_Registry.emplace<TComponent>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<TComponent>(*this, component);

			m_Scene->m_EntityMap[GetUUID()] = *this;

			return component;
		}


		template<typename TComponent, typename... Args>
		TComponent& AddOrReplaceComponent(Args&&... args)
		{
			TComponent& component = m_Scene->m_Registry.emplace_or_replace<TComponent>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<TComponent>(*this, component);
			return component;
		}


		template<typename TComponent>
		TComponent& GetComponent()
		{
			LOG_CORE_ASSERT(HasComponent<TComponent>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<TComponent>(m_EntityHandle);
		}


		template<typename TComponent>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<TComponent>(m_EntityHandle);
		}


		template<typename TComponent>
		void RemoveComponent()
		{
			LOG_CORE_ASSERT(HasComponent<TComponent>(), "Entity does not have component!");
            const auto componentID = Component<TComponent>::GetComponentID();
			m_Scene->m_Registry.remove<TComponent>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().tag; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};


	class System
	{
	public:
		System() = default;
		~System() = default;

		virtual std::vector<Entity> GetSystemEntities() const = 0;
	};
}

#endif