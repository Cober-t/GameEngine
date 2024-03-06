#include "Scene/ECS.h"

namespace Cober {

	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}


	void System::AddEntityToSystem(Entity entity) 
	{
		if (std::find(m_Entities.begin(), m_Entities.end(), entity) == m_Entities.end()) 
		{
			m_Entities.push_back(entity);
		}
	}

	void System::RemoveEntityFromSystem(Entity entity) 
	{
		m_Entities.erase(std::remove_if(m_Entities.begin(), m_Entities.end(), [&entity](Entity other) 
		{
			return entity == other;
		}), m_Entities.end());
	}
}