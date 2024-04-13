#include <pch.h>
#include "Scene/ECS.h"

namespace Cober {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}
}