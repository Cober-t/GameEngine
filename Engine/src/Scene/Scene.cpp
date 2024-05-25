#include <pch.h>
#include "Scene/Scene.h"

#include "Scene/ECS.h"
#include "Scene/Components.h"
#include "Scene/SceneSerializer.h"
#include "Scene/Systems/RenderSystem.h"
#include "Scene/Systems/PhysicsSystem2D.h"
#include "Scene/Systems/ScriptSystem.h"

#include <glm/glm.hpp>

namespace Cober {

	Scene::Scene()
	{
		LOG_INFO("Scene Created!!");
	}


	Scene::~Scene()
	{
       	LOG_INFO("Scene Destroyed!!");
	}


	bool Scene::Save(const Ref<Scene>& scene, std::string sceneName) 
	{
		if (SceneSerializer::Serialize(scene, sceneName) == false)
		{
			LOG_ERROR("Scene {0} could not be serialize", sceneName)
			return false;
		}

		return true;
	}


	Ref<Scene> Scene::Load(std::string sceneName) 
	{
		Log::ClearLogMessages();
		Ref<Scene> scene = SceneSerializer::Deserialize(sceneName);
		if (scene)
			scene->OnRuntimeStart();
			
		return scene;
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


	Ref<Scene> Scene::Copy(Ref<Scene> baseScene) 
	{
		Ref<Scene> newScene = CreateRef<Scene>();

		newScene->m_ViewportWidth = baseScene->m_ViewportWidth;
		newScene->m_ViewportHeight = baseScene->m_ViewportHeight;

		auto& srcSceneRegistry = baseScene->m_Registry;
		auto& dstSceneRegistry = newScene->m_Registry;
		std::unordered_map<UUID, entt::entity> enttMap;

		// Create entities in new scene
		auto idView = srcSceneRegistry.view<IDComponent>();
		for (auto e : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(e).ID;
			const auto& name = srcSceneRegistry.get<TagComponent>(e).tag;
			Entity newEntity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)newEntity;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

		return newScene;
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
		AddSystem<RenderSystem>();
		AddSystem<PhysicsSystem2D>();
		AddSystem<ScriptSystem>();

        GetSystem<RenderSystem>().Start();
		GetSystem<PhysicsSystem2D>().Start(this);
		GetSystem<ScriptSystem>().Start(this);
	}


    void Scene::OnSimulationStop()
	{
		RemoveSystem<PhysicsSystem2D>();
		RemoveSystem<RenderSystem>();
		GetSystem<ScriptSystem>().FreeScripts(this);
		RemoveSystem<ScriptSystem>();
	}


	void Scene::OnRuntimeStart()
	{
		AddSystem<RenderSystem>();

        GetSystem<RenderSystem>().Start();
	}


    void Scene::OnRuntimeStop()
	{
		RemoveSystem<RenderSystem>();
	}


	void Scene::OnUpdateSimulation(Unique<Timestep>& ts, const Ref<Camera>& camera)
	{
		GetSystem<RenderSystem>().Update(ts, camera, this);

		if (!m_IsPaused || m_StepFrames-- > 0.0f)
		{
			while(ts->GetDeltaTime() >= 1.0f)
            {
				GetSystem<PhysicsSystem2D>().Update(this, ts);
                ts->Update();
            }
		}

		GetSystem<ScriptSystem>().Update(this, ts->GetDeltaTime());
	}


	void Scene::OnUpdateRuntime(Unique<Timestep>& ts, const Ref<Camera>& camera)
	{	
		GetSystem<RenderSystem>().Update(ts, camera, this);
	}


	std::vector<Entity> Scene::GetSceneEntities()
	{ 
		std::vector<Entity> entities;

		for (auto entity : GetAllEntitiesWith<IDComponent, TagComponent>())
			entities.emplace_back( Entity{ entity, this } );

		return entities;
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


	Entity Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
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
}