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
	
		static bool Serialize(const Ref<Scene>& scene, const std::string& sceneName = "Scene1");
		// static bool SerializeRuntime(const std::string& sceneName = "Scene1");

		static Ref<Scene> Deserialize(std::string& sceneName);
		// Ref<Scene> DeserializeRuntime(std::string& sceneName);
	};
}

#endif