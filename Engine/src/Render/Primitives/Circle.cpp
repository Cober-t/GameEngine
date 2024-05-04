#include <pch.h>
#include "Render/Primitives/Circle.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	namespace Primitive {

		static Circle::Data data;
		Circle* Circle::s_Instance = nullptr;

		Circle::Circle() 
		{
			s_Instance = this;
		}

		Circle::~Circle() 
		{
			delete[] data.VertexBufferBase;
		}

		void Circle::Init() 
		{
			data.VertexArray = VertexArray::Create();

			data.VertexBuffer = VertexBuffer::Create(Render2D::GetStats().MaxVertices * sizeof(Attributes));
			data.VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_WorldPosition" },
				{ ShaderDataType::Float3, "a_LocalPosition" },
				{ ShaderDataType::Float4, "a_Color"         },
				{ ShaderDataType::Float,  "a_Thickness"     },
				{ ShaderDataType::Float,  "a_Fade"          },
				{ ShaderDataType::Int,    "a_EntityID"      }
			});

			data.VertexArray->AddVertexBuffer(data.VertexBuffer);
			data.VertexBufferBase = new Attributes[Render2D::GetStats().MaxVertices];
			
			uint32_t* circleIndices = Render2D::GetStats().GetIndices();

			Ref<IndexBuffer> circleIB = IndexBuffer::Create(circleIndices, Render2D::GetStats().MaxVertices);
			data.VertexArray->SetIndexBuffer(circleIB); // The shame as Quads Index Buffer
			delete[] circleIndices;

			data.Shader = Shader::Create("Circle.glsl");
		}


		void Circle::Flush()
		{
			if (data.IndexCount)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
				data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

				data.Shader->Bind();
				RenderGlobals::DrawIndexed(data.VertexArray, data.IndexCount);

				Render2D::GetStats().DrawCalls++;
			}
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


		uint32_t Circle::GetIndexCount()
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

			if (data.IndexCount >= Render2D::GetStats().MaxIndices)
				NextBatch();
			
			glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
			float thickness = entity.GetComponent<Render2DComponent>().thickness;
			float fade = entity.GetComponent<Render2DComponent>().fade;

			SetAttributes(transform, color, thickness, fade, (int)entity);
		}


		void Circle::SetAttributes(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int entityID) 
		{
			size_t VertexCount = sizeof(Render2D::GetStats().QuadVertexPositions) / sizeof(Render2D::GetStats().QuadVertexPositions[0]);
			for (size_t i = 0; i < VertexCount; i++)
			{
				data.VertexBufferPtr->WorldPosition = transform * Render2D::GetStats().QuadVertexPositions[i];
				data.VertexBufferPtr->LocalPosition = Render2D::GetStats().QuadVertexPositions[i] * 2.0f;
				data.VertexBufferPtr->Color = color;
				data.VertexBufferPtr->Thickness = thickness;
				data.VertexBufferPtr->Fade = fade;
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;
			}
			
			data.IndexCount += 6;

			Render2D::GetStats().CircleCount++;
		}
	}
}