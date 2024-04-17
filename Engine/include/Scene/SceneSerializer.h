#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include "Core/Utils.h"
#include "Core/EngineApp.h"

#include "Components.h"
#include "Scene.h"


namespace Cober {

	class SceneSerializer
	{
	public:
	
		static void Serialize(const Ref<Scene>& scene, const std::string& sceneName = "Scene1");
		static void SerializeRuntime(const std::string& sceneName = "Scene1");

		static Ref<Scene> Deserialize(const std::string& sceneName);
		// Ref<Scene> DeserializeRuntime(const std::string& sceneName);
	};
}

#endif