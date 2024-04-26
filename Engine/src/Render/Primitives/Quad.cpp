#include <pch.h>
#include "Render/Primitives/Quad.h"
#include "Render/Primitives/Line.h"
#include "Render/RenderGlobals.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	namespace Primitive {

		static Quad::Data data;
		Quad* Quad::s_Instance = nullptr;

		Quad::Quad()
		{
			s_Instance = this;
		}

		Quad::~Quad()
		{
			delete[] data.VertexBufferBase;
		}

		void Quad::NextBatch()
		{
			Quad::Flush();
			Quad::StartBatch();
		}

		void Quad::EndBatch()
		{
			Flush();
		}

		uint32_t Quad::GetIndexCount()
		{
			return data.IndexCount;
		}

		void Quad::Init() 
		{
			data.VertexArray = VertexArray::Create();

			data.VertexBuffer = VertexBuffer::Create(Data::MaxVertices * sizeof(Attributes));
			data.VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     },
				{ ShaderDataType::Float,  "a_TilingFactor" },
				{ ShaderDataType::Int,    "a_EntityID"	   }
			});
			data.VertexArray->AddVertexBuffer(data.VertexBuffer);
			data.VertexBufferBase = new Attributes[Data::MaxVertices];

			uint32_t* quadIndices = new uint32_t[Data::MaxIndices];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < Data::MaxIndices; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, Data::MaxIndices);
			data.VertexArray->SetIndexBuffer(quadIB);
			delete[] quadIndices;

			data.WhiteTexture = Texture::Create(1, 1);
			uint32_t whiteTextureData = 0xffffffff;

			data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

			int32_t samplers[Data::MaxTextureSlots];
			for (uint32_t i = 0; i < Data::MaxTextureSlots; i++)
				samplers[i] = i;

			data.Shader = Shader::Create("Quad.glsl");

			// Set first texture slot to 0
			data.TextureSlots[0] = data.WhiteTexture;

			data.VertexPositions[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
			data.VertexPositions[1] = {  1.0f, -1.0f, 0.0f, 1.0f };
			data.VertexPositions[2] = {  1.0f,  1.0f, 0.0f, 1.0f };
			data.VertexPositions[3] = { -1.0f,  1.0f, 0.0f, 1.0f };
		}


		void Quad::Flush()
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
			data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
				data.TextureSlots[i]->Bind(i);

			data.Shader->Bind();
			RenderGlobals::DrawIndexed(data.VertexArray, data.IndexCount);
		}


		void Quad::StartBatch()
		{
			data.IndexCount = 0;
			data.VertexBufferPtr = data.VertexBufferBase;

			data.TextureSlotIndex = 1;
		}


		void Quad::Draw(Entity& entity) 
		{
			auto& enttTrans = entity.GetComponent<TransformComponent>();

			glm::vec3 position{ enttTrans.position.x, enttTrans.position.y, enttTrans.position.z + 0.001f };
			glm::vec3 scale{ enttTrans.scale.x, enttTrans.scale.y, 1.0f };

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::toMat4(glm::quat(enttTrans.rotation))
				* glm::scale(glm::mat4(1.0f), scale);

			float tilingFactor = 1.0f;
			const int textureIndex = 0; // White Texture
			glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
			glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

			SetAttributes(transform, color, textureIndex, textureCoords, 1.0f, (int)entity);
		}


		
        void Quad::DrawRect(Entity& entity)
        {
			glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
            glm::mat4 transform = entity.GetComponent<TransformComponent>().GetTransform();
            glm::vec3 lineVertices[4];
            for (size_t i = 0; i < 4; i++)
                lineVertices[i] = transform * data.VertexPositions[i];

            Line::Draw(lineVertices[0], lineVertices[1], color, (int)entity);
            Line::Draw(lineVertices[1], lineVertices[2], color, (int)entity);
            Line::Draw(lineVertices[2], lineVertices[3], color, (int)entity);
            Line::Draw(lineVertices[3], lineVertices[0], color, (int)entity);
        }


		void Quad::SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID) 
		{

			// int32_t samplers[Data::MaxIndices];
			// for (uint32_t i = 0; i < Data::MaxIndices; i++)
			// 	samplers[i] = i;

			// data.Shader->SetIntArray("u_Textures", samplers, Data::MaxTextureSlots);
			
			if (data.IndexCount >= Data::MaxIndices)
				NextBatch();

			for (size_t i = 0; i < data.VertexCount; i++) 
			{
				data.VertexBufferPtr->Position = transform * data.VertexPositions[i];
				data.VertexBufferPtr->Color = color;
				data.VertexBufferPtr->TexCoord = textureCoords[i];
				data.VertexBufferPtr->TexIndex = textureIndex;
				data.VertexBufferPtr->TilingFactor = tilingFactor;
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;
			}

			data.IndexCount += 6;
		}
	}
}