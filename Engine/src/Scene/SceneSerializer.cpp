#include "Scene/SceneSerializer.h"


namespace Cober {

    SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}


    void SceneSerializer::Serialize(const std::string& sceneName)
    {
        Utils::DataFile sceneSaver;
		std::string name;
		if (sceneName.find_last_of('.') != std::string::npos)
			name = sceneName.substr(0, sceneName.find_last_of('.'));
		else
			name = sceneName;

		// sceneSaver[name]["world2D"].SetInt(m_Scene->GetWorldType());

        EngineApp& app = EngineApp::Get();
		sceneSaver[name]["numEntities"].SetInt(m_Scene->GetSceneEntities().size());

		int32_t numEntity = 0;
		for (auto& entity : m_Scene->GetSceneEntities()) 
        {
			auto& entityToBeSaved = sceneSaver[name]["Entity" + std::to_string(numEntity++)];
			
			entityToBeSaved["UUID"]["id"].SetString(std::to_string(entity.GetUUID()));
			
			entityToBeSaved["TagComponent"]["tag"].SetString(entity.GetComponent<TagComponent>().tag);
			
			auto& transform = entity.GetComponent<TransformComponent>();
			entityToBeSaved["TransformComponent"]["position"].SetVec3(transform.position);
			entityToBeSaved["TransformComponent"]["rotation"].SetVec3(transform.rotation);
			entityToBeSaved["TransformComponent"]["scale"].SetVec3(transform.scale);

			if (entity.HasComponent<Rigidbody2D>()) 
            {
				auto& rb2d = entity.GetComponent<Rigidbody2D>();
				entityToBeSaved["Rigidbody2D"]["bodyType"].SetInt((int)rb2d.type);
				entityToBeSaved["Rigidbody2D"]["fixedRotation"].SetInt(rb2d.fixedRotation);
			}

			if (entity.HasComponent<BoxCollider2D>()) 
			{
				auto& bc2D = entity.GetComponent<BoxCollider2D>();
				entityToBeSaved["BoxCollider2D"]["offset"].SetVec2(bc2D.offset);
				entityToBeSaved["BoxCollider2D"]["size"].SetVec2(bc2D.size);
				entityToBeSaved["BoxCollider2D"]["density"].SetReal(bc2D.density);
				entityToBeSaved["BoxCollider2D"]["friction"].SetReal(bc2D.friction);
				entityToBeSaved["BoxCollider2D"]["restitution"].SetReal(bc2D.restitution);
				// entityToBeSaved["BoxCollider2D"]["restitution Threshold"].SetReal(bc2D.restitutionThreshold);
			}

			if (entity.HasComponent<ScriptComponent>()) 
			{
				auto& scriptComponent = entity.GetComponent<ScriptComponent>();
				// entityToBeSaved["ScriptComponent"]["path"].SetVec2(scriptComponent.path);
			}
		}
		
		Utils::DataFile::Write(sceneSaver, sceneName);
    }


    Ref<Scene> SceneSerializer::Deserialize(const std::string& sceneName)
    {
		Utils::DataFile sceneLoader;
		std::string name;
		if (sceneName.find_last_of('.') != std::string::npos)
			name = sceneName.substr(0, sceneName.find_last_of('.'));
		else
			name = sceneName;

		if (Utils::DataFile::Read(sceneLoader, sceneName)) 
		{

			Ref<Scene> newScene = CreateRef<Scene>();
			// newScene->_world2D = sceneLoader[name]["world2D"].GetInt();

			for (int i = 0; i < sceneLoader[name]["numEntities"].GetInt(); i++) 
			{
				if (sceneLoader[name].HasProperty("Entity" + std::to_string(i))) 
				{
					auto& loader = sceneLoader[name]["Entity" + std::to_string(i)];
					Entity newEntity = newScene->CreateEntity(loader["TagComponent"]["tag"].GetString());

					newEntity.GetComponent<IDComponent>().ID = loader["UUID"]["id"].GetReal();

					newEntity.GetComponent<TransformComponent>().position = loader["TransformComponent"]["position"].GetVec3();
					newEntity.GetComponent<TransformComponent>().rotation = loader["TransformComponent"]["rotation"].GetVec3();
					newEntity.GetComponent<TransformComponent>().scale = loader["TransformComponent"]["scale"].GetVec3();

					if (loader.HasProperty("Rigidbody2D")) 
					{
						auto rb2d = loader["Rigidbody2D"];
						newEntity.AddComponent<Rigidbody2D>();
						newEntity.GetComponent<Rigidbody2D>().type = BodyType((int)rb2d["bodyType"].GetInt());
						newEntity.GetComponent<Rigidbody2D>().fixedRotation = (int)rb2d["fixedRotation"].GetInt();
					}

					if (loader.HasProperty("BoxCollider2D")) 
					{
						auto bc2d = loader["BoxCollider2D"];
						newEntity.AddComponent<BoxCollider2D>();
						newEntity.GetComponent<BoxCollider2D>().offset = bc2d["offset"].GetVec2();
						newEntity.GetComponent<BoxCollider2D>().size = bc2d["size"].GetVec2();
						newEntity.GetComponent<BoxCollider2D>().density = bc2d["density"].GetReal();
						newEntity.GetComponent<BoxCollider2D>().friction = bc2d["friction"].GetReal();
						newEntity.GetComponent<BoxCollider2D>().restitution = bc2d["restitution"].GetReal();
						// newEntity.GetComponent<BoxCollider2D>().restitutionThreshold = bc2d["restitution threshold"].GetReal();
					}

					if (loader.HasProperty("ScriptComponent")) 
					{
						auto bc2d = loader["ScriptComponent"];
						newEntity.AddComponent<ScriptComponent>();
					}
				}
			}

			return newScene;
		}
		
		LOG_ERROR("Cannot read scene with path: {0}{1}",SCENES_DIR,sceneName);
		return nullptr;
    }
}