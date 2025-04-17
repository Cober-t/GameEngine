#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include "Core/Utils.h"
#include "Core/EngineApp.h"

#include "Components.h"
#include "Scene.h"


namespace Cober {

	class CB_API SceneSerializer
	{
	public:
		static bool Serialize(const Ref<Scene>& scene, const std::string& sceneName = "Scene1.lua");
		static Ref<Scene> Deserialize(std::string& sceneName);

	public: // Because EntitySerializer use the same functions
		static void SerializeSceneSettings(Utils::DataFile& serializer);
		static void DeserializeSceneSettings(Utils::DataFile& loader);

		static void SerializeAllComponents(Entity& entity, Utils::DataFile& serializer);
		static void DeserializeAllComponents(Entity& entity, Utils::DataFile& loader);

		static void SerializeUUIDComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeTagComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeTransformComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeCameraComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeRigidbody2DComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeBoxCollider2DComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeCircleCollider2DComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeRender2DComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeNativeScriptComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeAudioComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeTextComponent(Entity& entity, Utils::DataFile& serializer);
		static void SerializeParticleEmitterComponent(Entity& entity, Utils::DataFile& serializer);

		static void DeserializeTransformComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeCameraComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeRigidbody2DComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeBoxCollider2DComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeCircleCollider2DComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeRender2DComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeNativeScriptComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeAudioComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeTextComponent(Entity& entity, Utils::DataFile& loader);
		static void DeserializeParticleEmitterComponent(Entity& entity, Utils::DataFile& loader);
	};


	class CB_API EntitySerializer
	{
	public:
		static bool Serialize(Entity& entity, const std::string& entityName = "EntityDefault.lua");
		static Entity Deserialize(Scene* scene, std::string& entityName);
	private:
		static void SerializeAllComponents(Entity& entity, Utils::DataFile& serializer);
		static void DeserializeAllComponents(Entity& entity, Utils::DataFile& loader);
	};
}

#endif
