#include <pch.h>
#include "Scene/SceneSerializer.h"
#include "Physics/Physics2D.h"

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

		// sceneSaver[name]["world2D"].SetInt(scene->GetWorldType());

        EngineApp& app = EngineApp::Get();
		sceneSaver[name]["numEntities"].SetInt(scene->GetSceneEntities().size());

		// Change to enum in the future
		sceneSaver[name]["Settings"]["SceneType"].SetString("2D");
		sceneSaver[name]["Settings"]["Gravity"].SetReal(Physics2D::GetSettings().Gravity);

		int32_t numEntity = 0;
		for (auto& entity : scene->GetSceneEntities()) 
        {
			auto& entityToBeSaved = sceneSaver[name]["Entity" + std::to_string(numEntity++)];
			
			entityToBeSaved["UUID"]["id"].SetString(std::to_string(entity.GetUUID()));
			
			entityToBeSaved["TagComponent"]["tag"].SetString(entity.GetComponent<TagComponent>().tag);
			
			auto& transform = entity.GetComponent<TransformComponent>();
			entityToBeSaved["TransformComponent"]["position"].SetVec3(transform.position);
			entityToBeSaved["TransformComponent"]["rotation"].SetVec3(transform.rotation);
			entityToBeSaved["TransformComponent"]["scale"].SetVec3(transform.scale);

			if (entity.HasComponent<CameraComponent>()) 
            {
				auto& camera = entity.GetComponent<CameraComponent>();
				entityToBeSaved["CameraComponent"]["focalPoint"].SetVec3(camera.focalPoint);
				entityToBeSaved["CameraComponent"]["distance"].SetReal(camera.distance);
				entityToBeSaved["CameraComponent"]["width"].SetInt(camera.width);
				entityToBeSaved["CameraComponent"]["height"].SetInt(camera.height);
				entityToBeSaved["CameraComponent"]["nearClip"].SetReal(camera.nearClip);
				entityToBeSaved["CameraComponent"]["farClip"].SetReal(camera.farClip);
				entityToBeSaved["CameraComponent"]["fov"].SetReal(camera.fov);
				entityToBeSaved["CameraComponent"]["perspective"].SetInt(camera.perspective);
				entityToBeSaved["CameraComponent"]["mainCamera"].SetInt(camera.mainCamera);
				entityToBeSaved["CameraComponent"]["debug"].SetInt(camera.debug);
			}

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
			}

			if (entity.HasComponent<CircleCollider2D>()) 
			{
				auto& bc2D = entity.GetComponent<CircleCollider2D>();
				entityToBeSaved["CircleCollider2D"]["offset"].SetVec2(bc2D.offset);
				entityToBeSaved["CircleCollider2D"]["radius"].SetReal(bc2D.radius);
				entityToBeSaved["CircleCollider2D"]["density"].SetReal(bc2D.density);
				entityToBeSaved["CircleCollider2D"]["friction"].SetReal(bc2D.friction);
				entityToBeSaved["CircleCollider2D"]["restitution"].SetReal(bc2D.restitution);
			}

			if (entity.HasComponent<EdgeCollider2D>()) 
			{
				auto& bc2D = entity.GetComponent<EdgeCollider2D>();
				entityToBeSaved["EdgeCollider2D"]["pointA"].SetVec2(bc2D.pointA);
				entityToBeSaved["EdgeCollider2D"]["pointB"].SetVec2(bc2D.pointB);
				entityToBeSaved["EdgeCollider2D"]["density"].SetReal(bc2D.density);
				entityToBeSaved["EdgeCollider2D"]["friction"].SetReal(bc2D.friction);
				entityToBeSaved["EdgeCollider2D"]["restitution"].SetReal(bc2D.restitution);
			}

			if (entity.HasComponent<PolygonCollider2D>()) 
			{
				auto& bc2D = entity.GetComponent<PolygonCollider2D>();
				entityToBeSaved["PolygonCollider2D"]["offset"].SetVec2(bc2D.offset);
				entityToBeSaved["PolygonCollider2D"]["density"].SetReal(bc2D.density);
				entityToBeSaved["PolygonCollider2D"]["friction"].SetReal(bc2D.friction);
				entityToBeSaved["PolygonCollider2D"]["restitution"].SetReal(bc2D.restitution);
			}

			if (entity.HasComponent<ScriptComponent>()) 
			{
				auto& scriptComponent = entity.GetComponent<ScriptComponent>();
				// entityToBeSaved["ScriptComponent"]["path"].SetVec2(scriptComponent.path);
			}

			if (entity.HasComponent<Render2DComponent>()) 
			{
				auto& bc2D = entity.GetComponent<Render2DComponent>();
				entityToBeSaved["Render2DComponent"]["color"].SetVec4(entity.GetComponent<Render2DComponent>().color);
				switch (entity.GetComponent<Render2DComponent>().shapeType)
				{
				case Shape2D::Line:
					entityToBeSaved["Render2DComponent"]["shape2D"].SetString("Line");
					entityToBeSaved["Render2DComponent"]["thickness"].SetReal(entity.GetComponent<Render2DComponent>().thickness);
					break;
				case Shape2D::Quad:
					entityToBeSaved["Render2DComponent"]["shape2D"].SetString("Quad");
					entityToBeSaved["Render2DComponent"]["fill"].SetInt(entity.GetComponent<Render2DComponent>().fill);
					break;
				case Shape2D::Circle:
					entityToBeSaved["Render2DComponent"]["shape2D"].SetString("Circle");
					entityToBeSaved["Render2DComponent"]["thickness"].SetReal(entity.GetComponent<Render2DComponent>().thickness);
					entityToBeSaved["Render2DComponent"]["fade"].SetReal(entity.GetComponent<Render2DComponent>().fade);
					break;
				case Shape2D::Sprite:
					entityToBeSaved["Render2DComponent"]["shape2D"].SetString("Sprite");
					entityToBeSaved["Render2DComponent"]["texture"].SetString(entity.GetComponent<Render2DComponent>().texture->GetPath());
					break;
				}
			}

			if (entity.HasComponent<NativeScriptComponent>())
			{
				auto& script = entity.GetComponent<NativeScriptComponent>();
				entityToBeSaved["NativeScriptComponent"]["className"].SetString(script.className);
			}

			if (entity.HasComponent<AudioComponent>())
			{
				auto& audio = entity.GetComponent<AudioComponent>();
				entityToBeSaved["AudioComponent"]["audioName"].SetString(audio.audioName);
				entityToBeSaved["AudioComponent"]["audioPath"].SetString(audio.audioPath.string());
				entityToBeSaved["AudioComponent"]["loop"].SetInt(audio.loop);
			}

			if (entity.HasComponent<TextComponent>())
			{
				auto& text = entity.GetComponent<TextComponent>();
				entityToBeSaved["TextComponent"]["text"].SetString(text.Text);
				entityToBeSaved["TextComponent"]["color"].SetVec4(text.Color);
				entityToBeSaved["TextComponent"]["kerning"].SetReal(text.Kerning);
				entityToBeSaved["TextComponent"]["lineSpacing"].SetReal(text.LineSpacing);
			}
		}
		
		return Utils::DataFile::Write(sceneSaver, sceneName);
    }


    Ref<Scene> SceneSerializer::Deserialize(std::string& sceneName)
    {
		Utils::DataFile sceneLoader;
		std::string name;

		std::filesystem::path scenesPath = std::filesystem::current_path() / "assets/scenes" / sceneName;

		if (!std::filesystem::exists(scenesPath))
		{
			sceneName = "SceneDefault.lua";
		}

		if (sceneName.find_last_of('.') != std::string::npos)
			name = sceneName.substr(0, sceneName.find_last_of('.'));
		else
			name = sceneName;



		if (Utils::DataFile::Read(sceneLoader, sceneName)) 
		{
			Ref<Scene> newScene = CreateRef<Scene>();

			Physics2D::GetSettings().Gravity = sceneLoader[name]["Settings"]["Gravity"].GetReal();

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
							
					if (loader.HasProperty("CameraComponent")) 
					{
						auto camera = loader["CameraComponent"];
						newEntity.AddComponent<CameraComponent>();
						newEntity.GetComponent<CameraComponent>().focalPoint = camera["focalPoint"].GetVec3();
						newEntity.GetComponent<CameraComponent>().distance = camera["distance"].GetReal();
						newEntity.GetComponent<CameraComponent>().width = camera["width"].GetInt();
						newEntity.GetComponent<CameraComponent>().height = camera["height"].GetInt();

						newEntity.GetComponent<CameraComponent>().nearClip = camera["nearClip"].GetReal();
						newEntity.GetComponent<CameraComponent>().farClip = camera["farClip"].GetReal();
						newEntity.GetComponent<CameraComponent>().fov = camera["fov"].GetReal();

						newEntity.GetComponent<CameraComponent>().perspective = camera["perspective"].GetInt();
						newEntity.GetComponent<CameraComponent>().debug = camera["debug"].GetInt();

						// Types of cameras in the future and Specification
						// newEntity.GetComponent<CameraComponent>().camera = CreateRef<GameCamera>(
						// 		camera["fov"].GetReal(),
						// 		camera["width"].GetReal(),
						// 		camera["height"].GetReal(),
						// 		camera["nearClip"].GetReal(),
						// 		camera["farClip"].GetReal()
						// 	);

						newEntity.GetComponent<CameraComponent>().mainCamera = camera["mainCamera"].GetInt();
						// newEntity.GetComponent<CameraComponent>().camera->SetMainCamera(camera["mainCamera"].GetInt());
					}

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
					}

					if (loader.HasProperty("CircleCollider2D")) 
					{
						auto bc2d = loader["CircleCollider2D"];
						newEntity.AddComponent<CircleCollider2D>();
						newEntity.GetComponent<CircleCollider2D>().offset = bc2d["offset"].GetVec2();
						newEntity.GetComponent<CircleCollider2D>().radius = bc2d["radius"].GetReal();
						newEntity.GetComponent<CircleCollider2D>().density = bc2d["density"].GetReal();
						newEntity.GetComponent<CircleCollider2D>().friction = bc2d["friction"].GetReal();
						newEntity.GetComponent<CircleCollider2D>().restitution = bc2d["restitution"].GetReal();
					}

					if (loader.HasProperty("EdgeCollider2D")) 
					{
						auto bc2d = loader["EdgeCollider2D"];
						newEntity.AddComponent<EdgeCollider2D>();
						newEntity.GetComponent<EdgeCollider2D>().pointA = bc2d["pointA"].GetVec2();
						newEntity.GetComponent<EdgeCollider2D>().pointB = bc2d["pointB"].GetVec2();
						newEntity.GetComponent<EdgeCollider2D>().density = bc2d["density"].GetReal();
						newEntity.GetComponent<EdgeCollider2D>().friction = bc2d["friction"].GetReal();
						newEntity.GetComponent<EdgeCollider2D>().restitution = bc2d["restitution"].GetReal();
					}

					if (loader.HasProperty("PolygonCollider2D")) 
					{
						auto bc2d = loader["PolygonCollider2D"];
						newEntity.AddComponent<PolygonCollider2D>();
						newEntity.GetComponent<PolygonCollider2D>().offset = bc2d["offset"].GetVec2();
						newEntity.GetComponent<PolygonCollider2D>().density = bc2d["density"].GetReal();
						newEntity.GetComponent<PolygonCollider2D>().friction = bc2d["friction"].GetReal();
						newEntity.GetComponent<PolygonCollider2D>().restitution = bc2d["restitution"].GetReal();
					}

					if (loader.HasProperty("ScriptComponent")) 
					{
						auto bc2d = loader["ScriptComponent"];
						newEntity.AddComponent<ScriptComponent>();
					}

					if (loader.HasProperty("Render2DComponent")) 
					{
						auto bc2d = loader["Render2DComponent"];
						newEntity.AddComponent<Render2DComponent>();
						newEntity.GetComponent<Render2DComponent>().color = loader["Render2DComponent"]["color"].GetVec4();

						std::string shapeType = loader["Render2DComponent"]["shape2D"].GetString();
						if (shapeType == "Line")
						{
							newEntity.GetComponent<Render2DComponent>().shapeType = Shape2D::Line;
							newEntity.GetComponent<Render2DComponent>().thickness = loader["Render2DComponent"]["thickness"].GetReal();
						}

						if (shapeType == "Quad")
						{
							newEntity.GetComponent<Render2DComponent>().shapeType = Shape2D::Quad;
							newEntity.GetComponent<Render2DComponent>().fill = loader["Render2DComponent"]["fill"].GetInt();
						}

						if (shapeType == "Circle")
						{
							newEntity.GetComponent<Render2DComponent>().shapeType = Shape2D::Circle;
							newEntity.GetComponent<Render2DComponent>().thickness = loader["Render2DComponent"]["thickness"].GetReal();
							newEntity.GetComponent<Render2DComponent>().fade = loader["Render2DComponent"]["fade"].GetReal();
						}

						if (shapeType == "Sprite")
						{
							std::string texturePath = loader["Render2DComponent"]["texture"].GetString();
							newEntity.GetComponent<Render2DComponent>().shapeType = Shape2D::Sprite;
							newEntity.GetComponent<Render2DComponent>().texture = Texture::Create(texturePath);
						}
					}

					if (loader.HasProperty("NativeScriptComponent")) 
					{
						auto script = loader["NativeScriptComponent"];
						newEntity.AddComponent<NativeScriptComponent>();
						newEntity.GetComponent<NativeScriptComponent>().className = script["className"].GetString();
					}

					if (loader.HasProperty("AudioComponent")) 
					{
						auto audio = loader["AudioComponent"];
						newEntity.AddComponent<AudioComponent>();
						newEntity.GetComponent<AudioComponent>().audioName = audio["audioName"].GetString();
						newEntity.GetComponent<AudioComponent>().audioPath = audio["audioPath"].GetString();
						newEntity.GetComponent<AudioComponent>().loop = audio["loop"].GetInt();
					}

					if (loader.HasProperty("TextComponent")) 
					{
						auto text = loader["TextComponent"];
						newEntity.AddComponent<TextComponent>();
						newEntity.GetComponent<TextComponent>().Text = text["text"].GetString();
						newEntity.GetComponent<TextComponent>().Color = text["color"].GetVec4();
						newEntity.GetComponent<TextComponent>().Kerning = text["kerning"].GetReal();
						newEntity.GetComponent<TextComponent>().LineSpacing = text["lineSpacing"].GetReal();
					}
				}
			}

			return newScene;
		}
		
		LOG_ERROR(("Cannot read scene with path: {0}", std::filesystem::current_path().string() + "\\assets\\scenes\\" + sceneName));
		return nullptr;
    }
}