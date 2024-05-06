#include <pch.h>
#include "Render/Primitives/Quad.h"
#include "Render/Primitives/Line.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	namespace Primitive {

		static Quad::Data data;

		void Quad::CleanVertexBuffer()
		{
			delete[] data.VertexBufferBase;
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
			

			int32_t samplers[data.MaxTextureSlots];
			for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
				samplers[i] = i;
			
			// SHADER
			data.Shader = Shader::Create("Quad.glsl");
			data.Shader->Bind();
			data.Shader->SetIntArray("u_Textures", samplers, data.MaxTextureSlots);

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
				{
					// LOG_WARNING("{0} {1} {2}", (float)i, data.TextureSlotIndex, data.TextureSlots[i]->GetName());
					data.TextureSlots[i]->Bind(i);
				}

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


		void Quad::NextBatch()
		{
			Flush();
			StartBatch();
		}


		void Quad::EndBatch()
		{
			Flush();
		}


		void Quad::Draw(Entity& entity) 
		{
			auto& enttTrans = entity.GetComponent<TransformComponent>();
			
			glm::vec3 position{ enttTrans.position.x, enttTrans.position.y, enttTrans.position.z + 0.001f };
			glm::vec3 scale{ enttTrans.scale.x, enttTrans.scale.y, 1.0f };

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::toMat4(glm::quat(enttTrans.rotation))
				* glm::scale(glm::mat4(1.0f), scale);

			constexpr float tilingFactor = 1.0f;
			float textureIndex = 0.0f; // White Texture
			glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
			glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

			SetAttributes(transform, color, textureIndex, textureCoords, 1.0f, (int)entity);
		}


		void Quad::DrawTexture(Entity& entity) 
		{
			auto& enttTrans = entity.GetComponent<TransformComponent>();
			
			glm::vec3 position{ enttTrans.position.x, enttTrans.position.y, enttTrans.position.z + 0.001f };
			glm::vec3 scale{ enttTrans.scale.x, enttTrans.scale.y, 1.0f };

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
				* glm::toMat4(glm::quat(enttTrans.rotation))
				* glm::scale(glm::mat4(1.0f), scale);

			constexpr float tilingFactor = 1.0f;
			float textureIndex = 0.0f; // White Texture
			glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
			glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

				
			if (entity.GetComponent<Render2DComponent>().texture)
			{
				// LOG_WARNING("{0} {1} {2}", data.TextureSlotIndex, entity.GetComponent<Render2DComponent>().texture->GetName(), entity.GetName());
				for (uint32_t i = 1; i < data.TextureSlotIndex; i++) 
				{
					if (*data.TextureSlots[i] == *entity.GetComponent<Render2DComponent>().texture) 
					{
						textureIndex = (float)i;
						break;
					}
				}

				if (textureIndex == 0.0f) 
				{
					textureIndex = (float)data.TextureSlotIndex;
					data.TextureSlots[data.TextureSlotIndex] = entity.GetComponent<Render2DComponent>().texture;
					data.TextureSlotIndex++;
				}
			}

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


		void Quad::SetAttributes(const glm::mat4& transform, const glm::vec4& color, float textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID) 
		{
			if (data.IndexCount >= Render2D::GetStats().MaxIndices)
				NextBatch();

			// size_t VertexCount = sizeof(Render2D::GetStats().QuadVertexPositions) / sizeof(Render2D::GetStats().QuadVertexPositions[0]);
			for (size_t i = 0; i < 4; i++) 
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