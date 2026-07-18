#include <pch.h>
#include "Render/Primitives/Quad.h"
#include "Render/Primitives/Line.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"
#include "Render/GraphicsDevice.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

    namespace Primitive {

        static Quad::Data data;

        void Quad::CleanVertexBuffer()
        {
            delete[] data.VertexBufferBase;
            data.VertexArray->Unbind();
            data.VertexBuffer->Unbind();
        }

		void Quad::Init()
		{
			data.VertexArray = VertexArray::Create();

			data.VertexBuffer = VertexBuffer::Create(Render2D::GetStats().MaxVertices * sizeof(Attributes));
			data.VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "Position" },
				{ ShaderDataType::Float4, "Color"    },
				{ ShaderDataType::Float2, "TexCoord" },
				{ ShaderDataType::Float,  "TexIndex" },
				{ ShaderDataType::Float,  "TilingFactor" },
				{ ShaderDataType::Int,    "EntityID" },
			});

			data.VertexArray->AddVertexBuffer(data.VertexBuffer);
			data.VertexBufferBase = new Attributes[Render2D::GetStats().MaxVertices];

			uint32_t* quadIndices = Render2D::GetStats().GetIndices();

			Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, Render2D::GetStats().MaxIndices);
			data.VertexArray->SetIndexBuffer(quadIB);
			delete[] quadIndices;

			// SHADER
			data.Shader = Shader::Create("Quad");

			// TEXTURE ARRAY
			data.TexArray = CreateRef<TextureArray>(32);
		}

	void Quad::Shutdown()
	{
		CleanVertexBuffer();
		data = Data{};
	}

	void Quad::InitFramebuffer()
        {
            if (data.FramebufferVAO != nullptr)
                return;

            data.FramebufferVAO = VertexArray::Create();

            // Normalized Vertex Attributes for a quad that fills the entire screen
            float screenQuadVertices[4 * 6] =
            {
                // positions   // texCoords
                -1.0f,  1.0f,  0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,

                -1.0f,  1.0f,  0.0f, 1.0f,
                 1.0f, -1.0f,  1.0f, 0.0f,
                 1.0f,  1.0f,  1.0f, 1.0f
            };

            data.FramebufferVBO = VertexBuffer::Create(screenQuadVertices, sizeof(screenQuadVertices));
            data.FramebufferVBO->SetLayout(
            {
                { ShaderDataType::Float3, "Position" },
                { ShaderDataType::Float2, "TexCoord" }
            });
            data.FramebufferVAO->AddVertexBuffer(data.FramebufferVBO);

            TextureSpecification textureSpec;
            textureSpec.Filter = ImageFilter::LINEAR;
            data.QuadTexture = Texture::Create(textureSpec);
            uint32_t whiteTextureData = 0xffffffff;
            data.QuadTexture->SetData(&whiteTextureData, sizeof(uint32_t));

            data.FramebufferShader = Shader::Create("ScreenTexture");
            data.FramebufferShader->Bind();
            data.FramebufferShader->SetInt("u_ScreenTexture", 0);
        }

        void Quad::DrawFramebuffer(const Ref<Framebuffer>& framebuffer)
        {
            InitFramebuffer();

            RenderGlobals::SetClearColor(40, 120, 150);

            data.FramebufferShader->Bind();
            data.QuadTexture->BindSingleTexture(framebuffer->GetColorAttachmentRenderID());

            RenderGlobals::DrawTriangles(data.FramebufferVAO, (uint32_t)6);
        }

void Quad::Flush()
{
    if (data.IndexCount)
    {
        // The TextureArray is always initialized with a white layer at index 0,
        // so BindToSlot() always works — even when no sprites were added this frame.
        if (data.TexArray)
        {
            data.TexArray->FlushUploads(nullptr);
            data.TexArray->BindToSlot(0);
        }

        uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
        data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

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
    if (data.TexArray)
    {
        data.TexArray->EnsureWhiteLayer();
        data.TexArray->ClearBatch();
    }
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

            glm::vec3 position{ enttTrans.position.x, enttTrans.position.y, enttTrans.position.z };
            glm::vec3 scale{ enttTrans.scale.x, enttTrans.scale.y, 1.0f };

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                * glm::toMat4(glm::quat(enttTrans.rotation))
                * glm::scale(glm::mat4(1.0f), scale);

            glm::vec4 color = entity.GetComponent<Render2DComponent>().color;

            glm::vec4 vertices[4]
            {
                { -1.0f, -1.0f, 0.0f, 1.0f },
                {  1.0f, -1.0f, 0.0f, 1.0f },
                {  1.0f,  1.0f, 0.0f, 1.0f },
                { -1.0f,  1.0f, 0.0f, 1.0f },
            };

            glm::vec2 texCoords[4]
            {
                { 0.0f, 0.0f },
                { 1.0f, 0.0f },
                { 1.0f, 1.0f },
                { 0.0f, 1.0f },
            };

            if (data.IndexCount >= Render2D::GetStats().MaxIndices)
            {
                NextBatch();
            }

            for (size_t i = 0; i < 4; i++)
            {
                glm::vec4 p = transform * vertices[i];
                data.VertexBufferPtr->Position = glm::vec3(p);
                data.VertexBufferPtr->Color = color;
                data.VertexBufferPtr->TexCoord = texCoords[i];
                data.VertexBufferPtr->TexIndex = 0.0f;
                data.VertexBufferPtr->TilingFactor = 1.0f;
                data.VertexBufferPtr->EntityID = (int)entity;
                data.VertexBufferPtr++;
            }

            data.IndexCount += 6;
            Render2D::GetStats().QuadCount++;
        }

		void Quad::Draw(const glm::mat4& transform, const glm::vec4& color, const Ref<SubTexture>& subTexture, int entityID)
		{
			float textureIndex = 0.0f;
			glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

			glm::mat4 vertices
			{
				{ -1.0f, -1.0f, 0.0f, 1.0f },
				{  1.0f, -1.0f, 0.0f, 1.0f },
				{  1.0f,  1.0f, 0.0f, 1.0f },
				{ -1.0f,  1.0f, 0.0f, 1.0f },
			};

			if (subTexture && subTexture->GetTexture())
			{
				vertices = subTexture->GetTexture()->GetTextureVertices();
				textureCoords[0] = { subTexture->GetTexCoords()[0].x, subTexture->GetTexCoords()[0].y };
				textureCoords[1] = { subTexture->GetTexCoords()[1].x, subTexture->GetTexCoords()[1].y };
				textureCoords[2] = { subTexture->GetTexCoords()[2].x, subTexture->GetTexCoords()[2].y };
				textureCoords[3] = { subTexture->GetTexCoords()[3].x, subTexture->GetTexCoords()[3].y };

				const Texture* tex = subTexture->GetTexture().get();

				// Check if texture is already in the batch
				for (uint32_t i = 1; i < data.TextureSlotIndex; i++)
				{
					if (*data.TextureSlots[i] == *tex)
					{
						textureIndex = (float)i;
						break;
					}
				}

				// Add new texture to batch
				if (textureIndex == 0.0f)
				{
					if (data.IndexCount >= Render2D::GetStats().MaxIndices)
						NextBatch();

					uint32_t layer = data.TexArray->AddTexture(tex);
					if (layer == UINT32_MAX)
					{
						NextBatch();
						layer = data.TexArray->AddTexture(tex);
					}

					textureIndex = (float)layer;
					data.TextureSlots[data.TextureSlotIndex] = subTexture->GetTexture();
					data.TextureSlotIndex++;
				}
			}

			for (size_t i = 0; i < 4; i++)
			{
				data.VertexBufferPtr->Position = transform * vertices[i];
				data.VertexBufferPtr->Color = color;
				data.VertexBufferPtr->TexCoord = textureCoords[i];
				data.VertexBufferPtr->TexIndex = textureIndex;
				data.VertexBufferPtr->TilingFactor = 1.0f;
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;
			}

			data.IndexCount += 6;
			Render2D::GetStats().QuadCount++;
		}

		void Quad::DrawTexture(Entity& entity)
		{
			auto& enttTrans = entity.GetComponent<TransformComponent>();

			float textureIndex = 0.0f;
			glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
			const glm::vec2* textureCoords = entity.GetComponent<Render2DComponent>().subTexture->GetTexCoords();

			if (entity.GetComponent<Render2DComponent>().texture)
			{
				const Texture* tex = entity.GetComponent<Render2DComponent>().texture.get();

				// Check if texture is already in the batch
				for (uint32_t i = 1; i < data.TextureSlotIndex; i++)
				{
					if (*data.TextureSlots[i] == *tex)
					{
						textureIndex = (float)i;
						break;
					}
				}

				// Add new texture to batch
				if (textureIndex == 0.0f)
				{
					if (data.IndexCount >= Render2D::GetStats().MaxIndices)
						NextBatch();

					uint32_t layer = data.TexArray->AddTexture(tex);
					if (layer == UINT32_MAX)
					{
						NextBatch();
						layer = data.TexArray->AddTexture(tex);
					}

					textureIndex = (float)layer;
					data.TextureSlots[data.TextureSlotIndex] = entity.GetComponent<Render2DComponent>().texture;
					data.TextureSlotIndex++;
				}
				Render2D::GetStats().TexturesLoadedOnScene = data.TextureSlotIndex - 1;
			}

			for (size_t i = 0; i < 4; i++)
			{
				glm::vec4 p = enttTrans.GetTransform() * entity.GetComponent<Render2DComponent>().vertices[i];
				data.VertexBufferPtr->Position = glm::vec3(p);
				data.VertexBufferPtr->Color = color;
				data.VertexBufferPtr->TexCoord = textureCoords[i];
				data.VertexBufferPtr->TexIndex = textureIndex;
				data.VertexBufferPtr->TilingFactor = 1.0f;
				data.VertexBufferPtr->EntityID = (int)entity;
				data.VertexBufferPtr++;
			}

			data.IndexCount += 6;
			Render2D::GetStats().QuadCount++;
		}

        void Quad::DrawRect(Entity& entity)
        {
            glm::mat4 vertices
            {
                { -1.0f, -1.0f, 0.0f, 1.0f },
                {  1.0f, -1.0f, 0.0f, 1.0f },
                {  1.0f,  1.0f, 0.0f, 1.0f },
                { -1.0f,  1.0f, 0.0f, 1.0f },
            };
            glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
            glm::mat4 transform = entity.GetComponent<TransformComponent>().GetTransform();
            glm::vec3 lineVertices[4];
            for (size_t i = 0; i < 4; i++)
                lineVertices[i] = transform * vertices[i];

            Line::Draw(lineVertices[0], lineVertices[1], color, (int)entity);
            Line::Draw(lineVertices[1], lineVertices[2], color, (int)entity);
            Line::Draw(lineVertices[2], lineVertices[3], color, (int)entity);
            Line::Draw(lineVertices[3], lineVertices[0], color, (int)entity);
        }

        void Quad::DrawRect(const glm::vec3& position, const glm::vec3& rotation, uint32_t width, uint32_t height, const glm::vec4& color, int entityID)
        {
            glm::mat4 vertices
            {
                { -1.0f, -1.0f, 0.0f, 1.0f },
                {  1.0f, -1.0f, 0.0f, 1.0f },
                {  1.0f,  1.0f, 0.0f, 1.0f },
                { -1.0f,  1.0f, 0.0f, 1.0f },
            };
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
                    * glm::toMat4(glm::quat(rotation))
                    * glm::scale(glm::mat4(1.0f), glm::vec3(width, height, 1.0f));

            glm::vec3 lineVertices[4];
            for (size_t i = 0; i < 4; i++)
                lineVertices[i] = transform * vertices[i];

            Line::Draw(lineVertices[0], lineVertices[1], color, entityID);
            Line::Draw(lineVertices[1], lineVertices[2], color, entityID);
            Line::Draw(lineVertices[2], lineVertices[3], color, entityID);
            Line::Draw(lineVertices[3], lineVertices[0], color, entityID);
        }

        void Quad::Draw(const std::vector<glm::vec3> vertices, int vertexCount, const glm::vec4& color)
        {
            if (data.IndexCount >= Render2D::GetStats().MaxIndices)
                NextBatch();

            glm::vec2 defaultTexCoords[4] = { {0,0}, {1,0}, {1,1}, {0,1} };

            for (int i = 0; i < vertexCount; i++)
            {
                data.VertexBufferPtr->Position = vertices[i];
                data.VertexBufferPtr->Color = color;
                data.VertexBufferPtr->TexCoord = defaultTexCoords[i % 4];
                data.VertexBufferPtr->TexIndex = 0.0f;
                data.VertexBufferPtr->TilingFactor = 1.0f;
                data.VertexBufferPtr->EntityID = -1;
                data.VertexBufferPtr++;
            }

            data.IndexCount += 6;
            Render2D::GetStats().QuadCount++;
        }
    }
}
