#include <pch.h>
#include "Scene/SceneSerializer.h"
#include "Physics/Physics2D.h"
#include "Scripting/NativeScripting.h"

namespace Cober {


    bool SceneSerializer::Serialize(const Ref<Scene>& scene, const std::string& sceneName)
    {
        Utils::DataFile sceneSaver;
		std::string name;

		if (sceneName.rfind(".lua") == std::string::npos)
			return false;

		if (sceneName.find_last_of('.') != std::string::npos)
			name = sceneName.substr(0, sceneName.find_last_of('.'));
		else
			name = sceneName;

		auto& loader = sceneSaver[name]["Settings"];
		SerializeSceneSettings(loader);

		sceneSaver[name]["numEntities"].SetInt(scene->GetSceneEntities().size());

		int32_t numEntity = 0;
		for (auto& entity : scene->GetSceneEntities()) 
        {
			auto& entityToBeSaved = sceneSaver[name]["Entity" + std::to_string(numEntity++)];
			SerializeAllComponents(entity, entityToBeSaved);
		}
		
		std::filesystem::path scenePath = "assets/scenes/" + sceneName;
		return Utils::DataFile::Write(sceneSaver, scenePath);
    }


    Ref<Scene> SceneSerializer::Deserialize(std::string& sceneName)
    {
		Utils::DataFile sceneLoader;
		std::string name;

		std::filesystem::path scenesPath = std::filesystem::current_path() / "assets/scenes" / sceneName;

		if (!std::filesystem::exists(scenesPath))
			sceneName = "SceneDefault.lua";

		if (sceneName.find_last_of('.') != std::string::npos)
			name = sceneName.substr(0, sceneName.find_last_of('.'));
		else
			name = sceneName;


		std::filesystem::path scenePath = "assets/scenes/" + sceneName;
		if (Utils::DataFile::Read(sceneLoader, scenePath)) 
		{
			Ref<Scene> newScene = CreateRef<Scene>();

			auto& loader = sceneLoader[name]["Settings"];
			DeserializeSceneSettings(loader);

			for (int i = 0; i < sceneLoader[name]["numEntities"].GetInt(); i++) 
			{
				if (sceneLoader[name].HasProperty("Entity" + std::to_string(i))) 
				{
					loader = sceneLoader[name]["Entity" + std::to_string(i)];
					Entity newEntity = newScene->CreateEntityWithUUID(
							UUID(loader["UUID"]["id"].GetReal()),
							loader["TagComponent"]["tag"].GetString());

					DeserializeAllComponents(newEntity, loader);
				}
			}

			return newScene;
		}
		
		LOG_ERROR(("Cannot read scene with path: {0}", std::filesystem::current_path().string() + "\\assets\\scenes\\" + sceneName));
		return nullptr;
    }

	void SceneSerializer::SerializeSceneSettings(Utils::DataFile& serializer)
	{
		// Change to enum in the future
		serializer["SceneType"].SetString("2D");
		serializer["Gravity"].SetReal(Physics2D::GetSettings().Gravity);
	}

	void SceneSerializer::DeserializeSceneSettings(Utils::DataFile& loader)
	{
		Physics2D::GetSettings().Gravity = loader["Gravity"].GetReal();
	}

	void SceneSerializer::SerializeAllComponents(Entity& entity, Utils::DataFile& serializer)
	{
		SerializeUUIDComponent(entity, serializer);
		SerializeTagComponent(entity, serializer);
		SerializeTransformComponent(entity, serializer);
		SerializeCameraComponent(entity, serializer);
		SerializeRigidbody2DComponent(entity, serializer);
		SerializeBoxCollider2DComponent(entity, serializer);
		SerializeCircleCollider2DComponent(entity, serializer);
		SerializeRender2DComponent(entity, serializer);
		SerializeNativeScriptComponent(entity, serializer);
		SerializeAudioComponent(entity, serializer);
		SerializeTextComponent(entity, serializer);
		SerializeParticleEmitterComponent(entity, serializer);
	}

	void SceneSerializer::DeserializeAllComponents(Entity& entity, Utils::DataFile& loader)
	{
		DeserializeTransformComponent(entity, loader);
		DeserializeCameraComponent(entity, loader);
		DeserializeRigidbody2DComponent(entity, loader);
		DeserializeBoxCollider2DComponent(entity, loader);
		DeserializeCircleCollider2DComponent(entity, loader);
		DeserializeRender2DComponent(entity, loader);
		DeserializeNativeScriptComponent(entity, loader);
		DeserializeAudioComponent(entity, loader);
		DeserializeTextComponent(entity, loader);
		DeserializeParticleEmitterComponent(entity, loader);
	}

	//////////////////////////////////////////
	/// 	SERIALIZE   //////////////////////
	void SceneSerializer::SerializeUUIDComponent(Entity& entity, Utils::DataFile& serializer)
	{
		serializer["UUID"]["id"].SetString(std::to_string(entity.GetUUID()));
	}
	void SceneSerializer::SerializeTagComponent(Entity& entity, Utils::DataFile& serializer)
	{
		serializer["TagComponent"]["tag"].SetString(entity.GetComponent<TagComponent>().tag);
	}
	void SceneSerializer::SerializeTransformComponent(Entity& entity, Utils::DataFile& serializer)
	{
		auto& transform = entity.GetComponent<TransformComponent>();
		serializer["TransformComponent"]["position"].SetVec3(transform.position);
		serializer["TransformComponent"]["rotation"].SetVec3(transform.rotation);
		serializer["TransformComponent"]["scale"].SetVec3(transform.scale);
	}
	void SceneSerializer::SerializeCameraComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<CameraComponent>()) 
		{
			auto& camera = entity.GetComponent<CameraComponent>();
			serializer["CameraComponent"]["distance"].SetReal(camera.distance);
			serializer["CameraComponent"]["width"].SetInt(camera.width);
			serializer["CameraComponent"]["height"].SetInt(camera.height);
			serializer["CameraComponent"]["nearClip"].SetReal(camera.nearClip);
			serializer["CameraComponent"]["farClip"].SetReal(camera.farClip);
			serializer["CameraComponent"]["fov"].SetReal(camera.fov);
			serializer["CameraComponent"]["perspective"].SetInt(camera.perspective);
			serializer["CameraComponent"]["mainCamera"].SetInt(camera.mainCamera);
			serializer["CameraComponent"]["debug"].SetInt(camera.debug);
		}
	}
	void SceneSerializer::SerializeRigidbody2DComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<Rigidbody2D>()) 
		{
			auto& rb2d = entity.GetComponent<Rigidbody2D>();
			serializer["Rigidbody2D"]["bodyType"].SetInt((int)rb2d.type);
			serializer["Rigidbody2D"]["fixedRotation"].SetInt(rb2d.fixedRotation);
		}
	}
	void SceneSerializer::SerializeBoxCollider2DComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<BoxCollider2D>()) 
		{
			auto& bc2D = entity.GetComponent<BoxCollider2D>();
			serializer["BoxCollider2D"]["offset"].SetVec2(bc2D.offset);
			serializer["BoxCollider2D"]["size"].SetVec2(bc2D.size);
			serializer["BoxCollider2D"]["density"].SetReal(bc2D.density);
			serializer["BoxCollider2D"]["friction"].SetReal(bc2D.friction);
			serializer["BoxCollider2D"]["restitution"].SetReal(bc2D.restitution);
			serializer["BoxCollider2D"]["isSensor"].SetInt(bc2D.isSensor);
		}
	}
	void SceneSerializer::SerializeCircleCollider2DComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<CircleCollider2D>()) 
		{
			auto& bc2D = entity.GetComponent<CircleCollider2D>();
			serializer["CircleCollider2D"]["offset"].SetVec2(bc2D.offset);
			serializer["CircleCollider2D"]["radius"].SetReal(bc2D.radius);
			serializer["CircleCollider2D"]["density"].SetReal(bc2D.density);
			serializer["CircleCollider2D"]["friction"].SetReal(bc2D.friction);
			serializer["CircleCollider2D"]["restitution"].SetReal(bc2D.restitution);
			serializer["CircleCollider2D"]["isSensor"].SetInt(bc2D.isSensor);
		}
	}
	void SceneSerializer::SerializeRender2DComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<Render2DComponent>()) 
		{
			auto& bc2D = entity.GetComponent<Render2DComponent>();
			serializer["Render2DComponent"]["color"].SetVec4(bc2D.color);
			switch (bc2D.shapeType)
			{
			case Shape2D::Line:
				serializer["Render2DComponent"]["shape2D"].SetString("Line");
				serializer["Render2DComponent"]["thickness"].SetReal(bc2D.thickness);
				break;
			case Shape2D::Quad:
				serializer["Render2DComponent"]["shape2D"].SetString("Quad");
				serializer["Render2DComponent"]["fill"].SetInt(bc2D.fill);
				break;
			case Shape2D::Circle:
				serializer["Render2DComponent"]["shape2D"].SetString("Circle");
				serializer["Render2DComponent"]["thickness"].SetReal(bc2D.thickness);
				serializer["Render2DComponent"]["fade"].SetReal(bc2D.fade);
				break;
			case Shape2D::Sprite:
				serializer["Render2DComponent"]["shape2D"].SetString("Sprite");
				serializer["Render2DComponent"]["texture"].SetString(bc2D.texture->GetPath());
				serializer["Render2DComponent"]["issubtexture"].SetReal(bc2D.isSubTexture);
				if (bc2D.isSubTexture)
				{
					serializer["Render2DComponent"]["subtexture"]["indices"].SetVec2(bc2D.subTextureIndex);
					serializer["Render2DComponent"]["subtexture"]["cellsize"].SetVec2(bc2D.subTextureCellSize);
					serializer["Render2DComponent"]["subtexture"]["spritesize"].SetVec2(bc2D.subTextureSpriteSize);
				}
				break;
			}
		}
	}
	void SceneSerializer::SerializeNativeScriptComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<NativeScriptComponent>())
		{
			auto& script = entity.GetComponent<NativeScriptComponent>();
			serializer["NativeScriptComponent"]["className"].SetString(script.className);
		}
	}
	void SceneSerializer::SerializeAudioComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<AudioComponent>())
		{
			auto& audio = entity.GetComponent<AudioComponent>();
			serializer["AudioComponent"]["audioName"].SetString(audio.audioName);
			serializer["AudioComponent"]["audioPath"].SetString(audio.audioPath.string());
			serializer["AudioComponent"]["loop"].SetInt(audio.loop);
		}
	}
	void SceneSerializer::SerializeTextComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<TextComponent>())
		{
			auto& text = entity.GetComponent<TextComponent>();
			serializer["TextComponent"]["font"].SetString(text.FontAsset->GetFontPath().string());
			serializer["TextComponent"]["text"].SetString(text.Text);
			serializer["TextComponent"]["color"].SetVec4(text.Color);
			serializer["TextComponent"]["kerning"].SetReal(text.Kerning);
			serializer["TextComponent"]["lineSpacing"].SetReal(text.LineSpacing);
		}
	}
	void SceneSerializer::SerializeParticleEmitterComponent(Entity& entity, Utils::DataFile& serializer)
	{
		if (entity.HasComponent<ParticleEmitterComponent>())
		{
			auto& particleEmitter = entity.GetComponent<ParticleEmitterComponent>();
			serializer["ParticleEmitterComponent"]["positionVariation"].SetVec2(particleEmitter.positionVariation);
			serializer["ParticleEmitterComponent"]["rotation"].SetReal(particleEmitter.rotation);
			serializer["ParticleEmitterComponent"]["sizeBegin"].SetReal(particleEmitter.sizeBegin);
			serializer["ParticleEmitterComponent"]["sizeEnd"].SetReal(particleEmitter.sizeEnd);
			serializer["ParticleEmitterComponent"]["sizeVariation"].SetReal(particleEmitter.sizeVariation);
			serializer["ParticleEmitterComponent"]["velocity"].SetVec2(particleEmitter.velocity);
			serializer["ParticleEmitterComponent"]["velocityVariation"].SetVec2(particleEmitter.velocityVariation);
			serializer["ParticleEmitterComponent"]["colorBegin"].SetVec4(particleEmitter.colorBegin);
			serializer["ParticleEmitterComponent"]["colorEnd"].SetVec4(particleEmitter.colorEnd);
			serializer["ParticleEmitterComponent"]["lifeTime"].SetReal(particleEmitter.lifeTime);
			serializer["ParticleEmitterComponent"]["lifeRemaining"].SetReal(particleEmitter.lifeRemaining);
			serializer["ParticleEmitterComponent"]["rate"].SetInt(particleEmitter.rate);
			serializer["ParticleEmitterComponent"]["active"].SetInt(particleEmitter.active);
			serializer["ParticleEmitterComponent"]["loop"].SetInt(particleEmitter.loop);

			serializer["ParticleEmitterComponent"]["texture"].SetString(particleEmitter.texture->GetPath());
			serializer["ParticleEmitterComponent"]["issubtexture"].SetReal(particleEmitter.isSubTexture);
			if (particleEmitter.isSubTexture)
			{
				serializer["ParticleEmitterComponent"]["subtexture"]["indices"].SetVec2(particleEmitter.subTextureIndex);
				serializer["ParticleEmitterComponent"]["subtexture"]["cellsize"].SetVec2(particleEmitter.subTextureCellSize);
				serializer["ParticleEmitterComponent"]["subtexture"]["spritesize"].SetVec2(particleEmitter.subTextureSpriteSize);
			}
		}
	}


	//////////////////////////////////////////
	/// 	DESERIALIZE   ////////////////////
	void SceneSerializer::DeserializeTransformComponent(Entity& entity, Utils::DataFile& loader)
	{
		entity.GetComponent<TransformComponent>().position = loader["TransformComponent"]["position"].GetVec3();
		entity.GetComponent<TransformComponent>().rotation = loader["TransformComponent"]["rotation"].GetVec3();
		entity.GetComponent<TransformComponent>().scale = loader["TransformComponent"]["scale"].GetVec3();
	}

	void SceneSerializer::DeserializeCameraComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("CameraComponent")) 
		{
			auto camera = loader["CameraComponent"];
			auto& component = entity.AddComponent<CameraComponent>();
			component.distance = camera["distance"].GetReal();
			component.width = camera["width"].GetInt();
			component.height = camera["height"].GetInt();

			component.nearClip = camera["nearClip"].GetReal();
			component.farClip = camera["farClip"].GetReal();
			component.fov = camera["fov"].GetReal();

			component.perspective = camera["perspective"].GetInt();
			component.debug = camera["debug"].GetInt();

			component.mainCamera = camera["mainCamera"].GetInt();
			component.UpdateCameraValues();
			component.camera->SetViewportSize(camera["width"].GetInt(), camera["height"].GetInt());
		}
	}

	void SceneSerializer::DeserializeRigidbody2DComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("Rigidbody2D")) 
		{
			auto rb2d = loader["Rigidbody2D"];
			auto& component = entity.AddComponent<Rigidbody2D>();
			component.type = BodyType((int)rb2d["bodyType"].GetInt());
			component.fixedRotation = (int)rb2d["fixedRotation"].GetInt();
		}
	}

	void SceneSerializer::DeserializeBoxCollider2DComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("BoxCollider2D")) 
		{
			auto bc2d = loader["BoxCollider2D"];
			auto& component = entity.AddComponent<BoxCollider2D>();
			component.offset = bc2d["offset"].GetVec2();
			component.size = bc2d["size"].GetVec2();
			component.density = bc2d["density"].GetReal();
			component.friction = bc2d["friction"].GetReal();
			component.restitution = bc2d["restitution"].GetReal();
			component.isSensor = bc2d["isSensor"].GetInt();
		}
	}

	void SceneSerializer::DeserializeCircleCollider2DComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("CircleCollider2D")) 
		{
			auto bc2d = loader["CircleCollider2D"];
			auto& component = entity.AddComponent<CircleCollider2D>();
			component.offset = bc2d["offset"].GetVec2();
			component.radius = bc2d["radius"].GetReal();
			component.density = bc2d["density"].GetReal();
			component.friction = bc2d["friction"].GetReal();
			component.restitution = bc2d["restitution"].GetReal();
			component.isSensor = bc2d["isSensor"].GetInt();
		}
	}

	void SceneSerializer::DeserializeRender2DComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("Render2DComponent")) 
		{
			auto renderComponent = loader["Render2DComponent"];
			auto& component = entity.AddComponent<Render2DComponent>();
			component.color = renderComponent["color"].GetVec4();

			std::string shapeType = renderComponent["shape2D"].GetString();
			if (shapeType == "Line")
			{
				component.shapeType = Shape2D::Line;
				component.thickness = renderComponent["thickness"].GetReal();
			}

			if (shapeType == "Quad")
			{
				component.shapeType = Shape2D::Quad;
				component.fill = renderComponent["fill"].GetInt();
			}

			if (shapeType == "Circle")
			{
				component.shapeType = Shape2D::Circle;
				component.thickness = renderComponent["thickness"].GetReal();
				component.fade = renderComponent["fade"].GetReal();
			}
			if (shapeType == "Sprite")
			{
				std::string texturePath = renderComponent["texture"].GetString();
				component.shapeType = Shape2D::Sprite;
				component.texture = Texture::Create(texturePath);
				component.isSubTexture = renderComponent["issubtexture"].GetReal();
				component.subTexture = CreateRef<SubTexture>();

				if (component.isSubTexture)
				{
					component.subTextureIndex = renderComponent["subtexture"]["indices"].GetVec2();
					component.subTextureCellSize = renderComponent["subtexture"]["cellsize"].GetVec2();
					component.subTextureSpriteSize = renderComponent["subtexture"]["spritesize"].GetVec2();

					component.subTexture = SubTexture::UpdateCoords(component.texture, component.vertices,
																	component.subTextureIndex, 
																	component.subTextureCellSize,
																	component.subTextureSpriteSize);					
				}
				else
				{
					component.subTexture = SubTexture::UpdateCoords(component.texture, component.vertices,
																	{0 ,0}, 
																	{component.texture->GetWidth(), component.texture->GetHeight()});
				}
			}
		}
	}

	void SceneSerializer::DeserializeNativeScriptComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("NativeScriptComponent")) 
		{
			auto script = loader["NativeScriptComponent"];
			auto& component = entity.AddComponent<NativeScriptComponent>();
			component.className = script["className"].GetString();
		}
	}

	void SceneSerializer::DeserializeAudioComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("AudioComponent")) 
		{
			auto audio = loader["AudioComponent"];
			auto& component = entity.AddComponent<AudioComponent>();
			component.audioName = audio["audioName"].GetString();
			component.audioPath = audio["audioPath"].GetString();
			component.loop = audio["loop"].GetInt();
		}
	}

	void SceneSerializer::DeserializeTextComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("TextComponent")) 
		{
			auto text = loader["TextComponent"];
			auto& component = entity.AddComponent<TextComponent>();
			component.FontAsset = CreateRef<Font>(text["font"].GetString());
			component.Text = text["text"].GetString();
			component.Color = text["color"].GetVec4();
			component.Kerning = text["kerning"].GetReal();
			component.LineSpacing = text["lineSpacing"].GetReal();
		}
	}

	void SceneSerializer::DeserializeParticleEmitterComponent(Entity& entity, Utils::DataFile& loader)
	{
		if (loader.HasProperty("ParticleEmitterComponent"))
		{
			auto particleEmitter = loader["ParticleEmitterComponent"];
			auto& component = entity.AddComponent<ParticleEmitterComponent>();
			component.position.x = entity.GetComponent<TransformComponent>().position.x;
			component.position.y = entity.GetComponent<TransformComponent>().position.y;
			component.positionVariation = particleEmitter["positionVariation"].GetVec2();
			component.rotation = particleEmitter["rotation"].GetReal();
			component.sizeBegin = particleEmitter["sizeBegin"].GetReal();
			component.sizeEnd = particleEmitter["sizeEnd"].GetReal();
			component.sizeVariation = particleEmitter["sizeVariation"].GetReal();
			component.velocity = particleEmitter["velocity"].GetVec2();
			component.velocityVariation = particleEmitter["velocityVariation"].GetVec2();
			component.colorBegin = particleEmitter["colorBegin"].GetVec4();
			component.colorEnd = particleEmitter["colorEnd"].GetVec4();
			component.lifeTime = particleEmitter["lifeTime"].GetReal();
			component.lifeRemaining = particleEmitter["lifeRemaining"].GetReal();
			component.rate = particleEmitter["rate"].GetInt();
			component.active = particleEmitter["active"].GetInt();
			component.loop = particleEmitter["loop"].GetInt();
			component.InitDefaultParticle();
			std::string texturePath = particleEmitter["texture"].GetString();

			if (texturePath == "")
				return;

			component.texture = Texture::Create(texturePath);
			component.isSubTexture = particleEmitter["issubtexture"].GetReal();
			component.subTexture = CreateRef<SubTexture>();

			if (component.isSubTexture)
			{
				component.subTextureIndex = particleEmitter["subtexture"]["indices"].GetVec2();
				component.subTextureCellSize = particleEmitter["subtexture"]["cellsize"].GetVec2();
				component.subTextureSpriteSize = particleEmitter["subtexture"]["spritesize"].GetVec2();

				component.subTexture = SubTexture::UpdateCoords(component.texture, component.vertices,
																component.subTextureIndex, 
																component.subTextureCellSize,
																component.subTextureSpriteSize);					
			}
			else
			{
				component.subTexture = SubTexture::UpdateCoords(component.texture, component.vertices,
																{0 ,0}, 
																{component.texture->GetWidth(), component.texture->GetHeight()});
			}
			
		}
	}


	bool EntitySerializer::Serialize(Entity& entity, const std::string& entityName)
	{
		Utils::DataFile scriptableEntitiesSaver;
		std::string name;

		if (entityName.rfind(".lua") == std::string::npos)
			return false;

		if (entityName.find_last_of('.') != std::string::npos)
			name = entityName.substr(0, entityName.find_last_of('.'));
		else
			name = entityName;

		int32_t numEntity = 0;
		auto& entityToBeSaved = scriptableEntitiesSaver[name];

		EntitySerializer::SerializeAllComponents(entity, entityToBeSaved);

		std::filesystem::path scriptableEntitiesPath = "assets/scripts/" + entityName;		
		return Utils::DataFile::Write(scriptableEntitiesSaver, scriptableEntitiesPath);
	}


	Entity EntitySerializer::Deserialize(Scene* scene, std::string& entityName)
	{
		Utils::DataFile scriptableEntitiesLoader;
		std::string name;

		std::filesystem::path scriptsPath = std::filesystem::current_path() / "assets/scripts" / entityName;

		if (!std::filesystem::exists(scriptsPath))
			entityName = "EntityDefault.lua";

		if (entityName.find_last_of('.') != std::string::npos)
			name = entityName.substr(0, entityName.find_last_of('.'));
		else
			name = entityName;

		std::filesystem::path scriptableEntitiesPath = "assets/scripts/" + entityName;
		if (Utils::DataFile::Read(scriptableEntitiesLoader, scriptableEntitiesPath)) 
		{
			if (scriptableEntitiesLoader.HasProperty(name)) 
			{
				auto& loader = scriptableEntitiesLoader[name];
				Entity newEntity = scene->CreateEntity(name);

				EntitySerializer::DeserializeAllComponents(newEntity, loader);

				if (newEntity.HasComponent<Rigidbody2D>() &&
				   (EngineApp::Get().GetGameState() == EngineApp::GameState::RUNTIME_EDITOR
					|| EngineApp::Get().GetGameState() == EngineApp::GameState::PLAY))
				{
					Physics2D::InitEntity(newEntity);
				}

				if (newEntity.HasComponent<NativeScriptComponent>())
					NativeScriptFn::InitEntity(scene, newEntity);

				return newEntity;
			}
		}
		
		LOG_ERROR(("ScriptableEntity does not exists with path: {0}", std::filesystem::current_path().string() + "\\assets\\scripts\\" + entityName));
		return {};
	}


	void EntitySerializer::SerializeAllComponents(Entity& entity, Utils::DataFile& serializer)
	{
		SceneSerializer::SerializeTransformComponent(entity, serializer);
		SceneSerializer::SerializeCameraComponent(entity, serializer);
		SceneSerializer::SerializeRigidbody2DComponent(entity, serializer);
		SceneSerializer::SerializeBoxCollider2DComponent(entity, serializer);
		SceneSerializer::SerializeCircleCollider2DComponent(entity, serializer);
		SceneSerializer::SerializeRender2DComponent(entity, serializer);
		SceneSerializer::SerializeNativeScriptComponent(entity, serializer);
		SceneSerializer::SerializeAudioComponent(entity, serializer);
		SceneSerializer::SerializeTextComponent(entity, serializer);
		SceneSerializer::SerializeParticleEmitterComponent(entity, serializer);
	}

	void EntitySerializer::DeserializeAllComponents(Entity& entity, Utils::DataFile& loader)
	{
		SceneSerializer::DeserializeTransformComponent(entity, loader);
		SceneSerializer::DeserializeCameraComponent(entity, loader);
		SceneSerializer::DeserializeBoxCollider2DComponent(entity, loader);
		SceneSerializer::DeserializeCircleCollider2DComponent(entity, loader);
		SceneSerializer::DeserializeRigidbody2DComponent(entity, loader);
		SceneSerializer::DeserializeRender2DComponent(entity, loader);
		SceneSerializer::DeserializeNativeScriptComponent(entity, loader);
		SceneSerializer::DeserializeAudioComponent(entity, loader);
		SceneSerializer::DeserializeTextComponent(entity, loader);
		SceneSerializer::DeserializeParticleEmitterComponent(entity, loader);
	}
}