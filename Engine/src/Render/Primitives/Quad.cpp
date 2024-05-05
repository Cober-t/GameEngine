#include <pch.h>
#include "Render/Primitives/Quad.h"
#include "Render/Primitives/Line.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"

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

			data.VertexBuffer = VertexBuffer::Create(Render2D::GetStats().MaxVertices * sizeof(Attributes));
			data.VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position"     },
				{ ShaderDataType::Float4, "a_Color"        },
				{ ShaderDataType::Float2, "a_TexCoord"     },
				{ ShaderDataType::Float,  "a_TexIndex"     },
				{ ShaderDataType::Float,  "a_TilingFactor" },
				{ ShaderDataType::Int,    "a_EntityID"	   }
			});
			data.VertexArray->AddVertexBuffer(data.VertexBuffer);
			data.VertexBufferBase = new Attributes[Render2D::GetStats().MaxVertices];

			uint32_t* quadIndices = Render2D::GetStats().GetIndices();

			Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, Render2D::GetStats().MaxIndices);
			data.VertexArray->SetIndexBuffer(quadIB);
			delete[] quadIndices;

			// TEXTURE
			data.WhiteTexture = Texture::Create(1, 1);
			uint32_t whiteTextureData = 0xffffffff;

			data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

			int32_t samplers[Data::MaxTextureSlots];
			for (uint32_t i = 0; i < Data::MaxTextureSlots; i++)
				samplers[i] = i;

			// SHADER
			data.Shader = Shader::Create("Quad.glsl");

			// Set first texture slot to 0
			data.TextureSlots[0] = data.WhiteTexture;
		}


		void Quad::Flush()
		{
			if (data.IndexCount)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
				data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

				// Bind textures
				for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
					data.TextureSlots[i]->Bind(i);

				data.Shader->Bind();
				RenderGlobals::DrawIndexed(data.VertexArray, data.IndexCount);

				Render2D::GetStats().DrawCalls++;
			}
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
                lineVertices[i] = transform * Render2D::GetStats().QuadVertexPositions[i];

            Line::Draw(lineVertices[0], lineVertices[1], color, (int)entity);
            Line::Draw(lineVertices[1], lineVertices[2], color, (int)entity);
            Line::Draw(lineVertices[2], lineVertices[3], color, (int)entity);
            Line::Draw(lineVertices[3], lineVertices[0], color, (int)entity);
        }

		
		void Quad::Draw(const std::vector<glm::vec3> vertices, int vertexCount, const glm::vec4& color) 
		{
			if (data.IndexCount >= Render2D::GetStats().MaxIndices)
				NextBatch();

			glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
			size_t VertexCount = sizeof(Render2D::GetStats().QuadVertexPositions) / sizeof(Render2D::GetStats().QuadVertexPositions[0]);
			for (size_t i = 0; i < vertexCount; i++) 
			{
				data.VertexBufferPtr->Position = vertices[i];
				data.VertexBufferPtr->Color = color;
				data.VertexBufferPtr->TexCoord = textureCoords[i];
				data.VertexBufferPtr->TexIndex = 0;
				data.VertexBufferPtr->TilingFactor = 1.0f;
				data.VertexBufferPtr->EntityID = -1;
				data.VertexBufferPtr++;
			}

			data.IndexCount += 6;

			Render2D::GetStats().QuadCount++;
		}


		void Quad::SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID) 
		{

			// int32_t samplers[Data::MaxIndices];
			// for (uint32_t i = 0; i < Data::MaxIndices; i++)
			// 	samplers[i] = i;

			// data.Shader->SetIntArray("u_Textures", samplers, Data::MaxTextureSlots);
			
			if (data.IndexCount >= Render2D::GetStats().MaxIndices)
				NextBatch();

			size_t VertexCount = sizeof(Render2D::GetStats().QuadVertexPositions) / sizeof(Render2D::GetStats().QuadVertexPositions[0]);
			for (size_t i = 0; i < VertexCount; i++) 
			{
				data.VertexBufferPtr->Position = transform * Render2D::GetStats().QuadVertexPositions[i];
				data.VertexBufferPtr->Color = color;
				data.VertexBufferPtr->TexCoord = textureCoords[i];
				data.VertexBufferPtr->TexIndex = textureIndex;
				data.VertexBufferPtr->TilingFactor = tilingFactor;
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;
			}

			data.IndexCount += 6;

			Render2D::GetStats().QuadCount++;
		}
	}
}