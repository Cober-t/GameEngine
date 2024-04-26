#include <pch.h>
#include "Render/Primitives/Cube.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	// static Cube::Data data;
	Cube* Cube::s_Instance = nullptr;

    Cube::Cube() 
    {
        s_Instance = this;
	}

    Cube::~Cube() 
    {
	}

	void Cube::Init() 
    {
	}

    void Cube::Flush()
    {
    }
    
    void Cube::NextBatch()
    {
    }

    void Cube::EndBatch()
    {
    }

    void Cube::StartBatch()
    {
    }

	void Cube::Draw(Entity& entity) 
    {
	}

	void Cube::SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID) 
    {
	}
}