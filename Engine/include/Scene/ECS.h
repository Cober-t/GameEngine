#ifndef ENTITY_H
#define ENTITY_H

#include "Core/UUID.h"
#include "Scene.h"
#include "Components.h"

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

			// Update Entity System Signature
            const auto componentID = Component<TComponent>::GetComponentID();
            m_EntityComponentSignature.set(componentID);

			// Update EntityMap
			m_Scene->m_EntityMap[GetUUID()] = *this;

			// Save Entity in the promote system vector, or update if it exists
			auto iterator = std::find(m_Scene->m_EntitiesToBeAdded.begin(), m_Scene->m_EntitiesToBeAdded.end(), *this);
			if (iterator != m_Scene->m_EntitiesToBeAdded.end())
			{
				int index = iterator - m_Scene->m_EntitiesToBeAdded.begin();
				m_Scene->m_EntitiesToBeAdded.erase(std::next(m_Scene->m_EntitiesToBeAdded.begin(), index));
			}

			m_Scene->m_EntitiesToBeAdded.push_back(*this);

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
            m_EntityComponentSignature[componentID].set(false);

			m_Scene->m_Registry.remove<TComponent>(m_EntityHandle);
		}

        const Signature& GetComponentSignature() { return m_EntityComponentSignature; }
		void ResetComponentSignature() { m_EntityComponentSignature.reset(); }
        
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
        Signature m_EntityComponentSignature;
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};


	class System
	{
	public:
		System() = default;
		~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityFromSystem(Entity entity);

		std::vector<Entity> GetSystemEntities() const { return m_Entities; }
		const Signature& GetComponentSignature() const { return m_ComponentSignature; }

		// Defines the component type that entities must have to be considered by the system
		template<typename TComponent>
		void RequireComponent();

	private:
		Signature m_ComponentSignature;
		std::vector<Entity> m_Entities;
	};

	
	template <typename TComponent>
	void System::RequireComponent() 
	{
		const auto componentID = Component<TComponent>::GetComponentID();
		m_ComponentSignature.set(componentID);
	}
}

#endif