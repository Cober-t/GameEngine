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
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& sceneName = "Scene1");
		void SerializeRuntime(const std::string& sceneName = "Scene1");

		Ref<Scene> Deserialize(const std::string& sceneName);
		// Ref<Scene> DeserializeRuntime(const std::string& sceneName);

	private:
		Ref<Scene> m_Scene;
	};
}

#endif