#include "Scene/Scene.h"

#include "Scene/Components.h"
#include "Scene/Systems/RenderSystem.h"

#include <glm/glm.hpp>

#include "Scene/ECS.h"


namespace Cober {

	int IComponent::nextIndex = 0;

	Scene::Scene()
	{
        AddSystem<RenderSystem>(this);
	}


	Scene::~Scene()
	{
        RemoveSystem<RenderSystem>();
	}


	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
		{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
				entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

				auto& srcComponent = src.get<Component>(srcEntity);
				dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
		}(), ...);
	}


	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<Component...>(dst, src, enttMap);
	}


	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
		{
			if (src.HasComponent<Component>())
				dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
		}(), ...);
	}


	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}


	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithUUID(UUID(), name);
	}


	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };

		
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tagComponent = entity.AddComponent<TagComponent>();
		tagComponent.tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = entity;

        LOG_INFO("Created entity with ID {0} and name {1}", uuid, tagComponent.tag);

		return entity;
	}


	void Scene::DestroyEntity(Entity entity)
	{
		m_EntityMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
	}


	void Scene::OnSimulationStart()
	{
        GetSystem<RenderSystem>().Start();

		// Entity entity = CreateEntity();
	}

    void Scene::OnSimulationStop()
	{
		// Delete Entities
	}


	void Scene::OnUpdateSimulation(Timestep ts, const Ref<GameCamera>& camera)
	{
        GetSystem<RenderSystem>().Update(ts, camera);
	}


	Entity Scene::FindEntityByName(std::string_view name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const TagComponent& tagComponent = view.get<TagComponent>(entity);
			if (tagComponent.tag == name)
				return Entity{ entity, this };
		}
		return {};
	}


	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		// TODO: Maybe should be assert
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return { m_EntityMap.at(uuid), this };

		return {};
	}


    template<typename TSystem, typename ... TArgs>
	void Scene::AddSystem(TArgs&& ...args)
    {
		Ref<TSystem> newSystem(CreateRef<TSystem>(std::forward<TArgs>(args)...));
		m_Systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
	}


	template<typename TSystem>
	void Scene::RemoveSystem()
    {
		auto system = m_Systems.find(std::type_index(typeid(TSystem)));
		m_Systems.erase(system);
	}


	template<typename TSystem>
	bool Scene::HasSystem() const
    {
		return m_Systems.find(std::type_index(typeid(TSystem))) != m_Systems.end();
	}


	template<typename TSystem>
	TSystem& Scene::GetSystem() const
    {
		LOG_ASSERT(HasSystem<TSystem>(), "System is not added!");
		auto system = m_Systems.find(std::type_index(typeid(TSystem)));
		return *(std::static_pointer_cast<TSystem>(system->second));
	}


    template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}
}