#include <pch.h>
#include "Render/Primitives/Text.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	namespace Primitive {

		static Text::Data data;

		void Text::CleanVertexBuffer() 
		{
			delete[] data.VertexBufferBase;
		}

		void Text::Init() 
		{
			data.VertexArray = VertexArray::Create();

			data.VertexBuffer = VertexBuffer::Create(Render2D::GetStats().MaxVertices * sizeof(Attributes));
			data.VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color"    },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Int,    "a_EntityID" }
			});

			data.VertexArray->AddVertexBuffer(data.VertexBuffer);
			data.VertexBufferBase = new Attributes[Render2D::GetStats().MaxVertices];
			
			uint32_t* textIndices = Render2D::GetStats().GetIndices();

			Ref<IndexBuffer> textIB = IndexBuffer::Create(textIndices, Render2D::GetStats().MaxVertices);
			data.VertexArray->SetIndexBuffer(textIB); // The shame as Quads Index Buffer
			delete[] textIndices;

			data.Shader = Shader::Create("Text.glsl");

            data.WhiteTexture = Texture::Create(TextureSpecification());
			uint32_t whiteTextureData = 0xffffffff;
			data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

			data.TextureSlots[0] = data.WhiteTexture;
		}


		void Text::Flush()
		{
			if (data.IndexCount)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
				data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

                auto buf = data.VertexBufferBase;
    			data.FontAtlasTexture->Bind(0);

				data.Shader->Bind();
				RenderGlobals::DrawIndexed(data.VertexArray, data.IndexCount);

				Render2D::GetStats().DrawCalls++;
			}
		}


		void Text::NextBatch()
		{
			Flush();
			StartBatch();
		}


		void Text::EndBatch()
		{
			Flush();
		}


		void Text::StartBatch()
		{
			data.IndexCount = 0;
			data.VertexBufferPtr = data.VertexBufferBase;
		}


		uint32_t Text::GetIndexCount()
		{
			return data.IndexCount;
		}


		void Text::Draw(Entity& entity) 
		{
			// auto& enttTrans = entity.GetComponent<TransformComponent>();

			// glm::vec3 position{ enttTrans.position.x, enttTrans.position.y, enttTrans.position.z + 0.001f };
			// glm::vec3 scale{ enttTrans.scale.x, enttTrans.scale.y, 1.0f };

			// glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			// 	* glm::toMat4(glm::quat(enttTrans.rotation))
			// 	* glm::scale(glm::mat4(1.0f), scale);

			// if (data.IndexCount >= Render2D::GetStats().MaxIndices)
			// 	NextBatch();
			
			// glm::vec4 color = entity.GetComponent<Render2DComponent>().color;

			// SetAttributes(transform, color, thickness, fade, (int)entity);
		}


		void Text::Draw(const glm::mat4& transform, const glm::vec4& color, float thickness)
		{
			// SetAttributes(transform, color, thickness, 0, -1);
		}


		void Text::SetAttributes(const glm::mat3& position, const glm::vec4& color, const glm::vec2& texCoord, int entityID) 
		{
            // size_t VertexCount = sizeof(Render2D::GetStats().QuadVertexPositions) / sizeof(Render2D::GetStats().QuadVertexPositions[0]);

			// for (size_t i = 0; i < VertexCount; i++)
			// {
			// 	data.VertexBufferPtr->Position = position;
			// 	data.VertexBufferPtr->Color = color;
			// 	data.VertexBufferPtr->TexCoord = texCoord;
			// 	data.VertexBufferPtr->EntityID = entityID;
			// 	data.VertexBufferPtr++;
			// }
			
			data.IndexCount += 6;

			Render2D::GetStats().TextCount++;
		}
	}
}