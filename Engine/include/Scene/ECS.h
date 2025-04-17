#ifndef ENTITY_H
#define ENTITY_H

#include "Core/UUID.h"
#include "Components.h"
#include "Scene.h"

#include <entt/entt.hpp>


namespace Cober {

	class CB_API Entity
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
			m_Scene->m_EntityMap[GetUUID()] = *this;

			return component;
		}


		template<typename TComponent, typename... Args>
		TComponent& AddOrReplaceComponent(Args&&... args)
		{
			TComponent& component = m_Scene->m_Registry.emplace_or_replace<TComponent>(m_EntityHandle, std::forward<Args>(args)...);
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
			m_Scene->m_Registry.remove<TComponent>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
		operator int() const { return (int)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		Scene* GetScene() { return m_Scene; }
		std::string GetName() { return GetComponent<TagComponent>().tag; }
		inline void* GetBody() { return HasComponent<Rigidbody2D>() == true ? GetComponent<Rigidbody2D>().runtimeBody : nullptr; }
		const entt::entity GetHandle() { return m_EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const { return !(*this == other); }

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};


	class CB_API System
	{
	public:
		System() = default;
		~System() = default;
	};
}

#endif