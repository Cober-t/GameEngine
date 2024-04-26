#include <pch.h>
#include "Render/Primitives/Circle.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

    static Circle::Data data;

    Circle::Circle() 
    {

	}

    Circle::~Circle() 
    {
        delete[] data.VertexBufferBase;
	}

	void Circle::Init() 
    {
        data.VertexArray = VertexArray::Create();

		data.VertexBuffer = VertexBuffer::Create(data.MaxVertices * sizeof(Attributes));
		data.VertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
		});

        data.VertexArray->AddVertexBuffer(data.VertexBuffer);
        data.VertexBufferBase = new Attributes[Data::MaxVertices];
        
        uint32_t* circleIndices = new uint32_t[Data::MaxIndices];
        
		uint32_t offset = 0;
		for (uint32_t i = 0; i < Data::MaxIndices; i += 6)
		{
			circleIndices[i + 0] = offset + 0;
			circleIndices[i + 1] = offset + 1;
			circleIndices[i + 2] = offset + 2;

			circleIndices[i + 3] = offset + 2;
			circleIndices[i + 4] = offset + 3;
			circleIndices[i + 5] = offset + 0;

			offset += 4;
		}

        Ref<IndexBuffer> circleIB = IndexBuffer::Create(circleIndices, Data::MaxIndices);
		data.VertexArray->SetIndexBuffer(circleIB); // The shame as Quads Index Buffer
        delete[] circleIndices;

        data.Shader = Shader::Create("Circle.glsl");

		data.VertexPositions[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
		data.VertexPositions[1] = {  1.0f, -1.0f, 0.0f, 1.0f };
		data.VertexPositions[2] = {  1.0f,  1.0f, 0.0f, 1.0f };
		data.VertexPositions[3] = { -1.0f,  1.0f, 0.0f, 1.0f };
	}


    void Circle::Flush()
    {
        uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
        data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

        data.Shader->Bind();
        RenderGlobals::DrawIndexed(data.VertexArray, data.IndexCount);
    }


    void Circle::NextBatch()
    {
        Flush();
		StartBatch();
    }


    void Circle::EndBatch()
    {
        Flush();
    }


    void Circle::StartBatch()
    {
        data.IndexCount = 0;
		data.VertexBufferPtr = data.VertexBufferBase;
    }


    uint32_t Circle::GetIndexCount() const
    {
        return data.IndexCount;
    }


	void Circle::Draw(Entity& entity) 
    {
        auto& enttTrans = entity.GetComponent<TransformComponent>();

		glm::vec3 position{ enttTrans.position.x, enttTrans.position.y, enttTrans.position.z + 0.001f };
		glm::vec3 scale{ enttTrans.scale.x, enttTrans.scale.y, 1.0f };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::toMat4(glm::quat(enttTrans.rotation))
			* glm::scale(glm::mat4(1.0f), scale);

		if (data.IndexCount >= Data::MaxIndices)
			NextBatch();
        
        glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
        float thickness = entity.GetComponent<Render2DComponent>().thickness;
        float fade = entity.GetComponent<Render2DComponent>().fade;

        SetAttributes(transform, color, thickness, fade, (int)entity);

		data.IndexCount += 6;
	}


	void Circle::SetAttributes(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID) 
    {
		for (size_t i = 0; i < data.VertexCount; i++)
		{
			data.VertexBufferPtr->WorldPosition = transform * data.VertexPositions[i];
			data.VertexBufferPtr->LocalPosition = data.VertexPositions[i] * 2.0f;
			data.VertexBufferPtr->Color = color;
			data.VertexBufferPtr->Thickness = thickness;
            data.VertexBufferPtr->Fade = fade;
			data.VertexBufferPtr->EntityID = entityID;
			data.VertexBufferPtr++;
		}
	}
}