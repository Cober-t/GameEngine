#include "Render/Render2D.h"


#include <memory>


namespace Cober {

	// [+++++++++++++++++++++++++++]
	// [+++++ PRIMITIVES DATA +++++]
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;

		// Editor-only
		float EntityID;
	};


	struct RenderData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // 0 = white texture

		glm::vec4 QuadVertexPositions[4];

		Render2D::Statistics Stats;
		struct CameraData
		{
			glm::mat4 Projection;
			glm::mat4 View;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
	};


	static RenderData data;

	void Render2D::Start() 
	{
		data.QuadVertexArray = VertexArray::Create();

		data.QuadVertexBuffer = VertexBuffer::Create(data.MaxVertices * sizeof(QuadVertex));
		data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position"     },
			{ ShaderDataType::Float4, "a_Color"        },
			{ ShaderDataType::Float2, "a_TexCoord"     },
			{ ShaderDataType::Float,  "a_TexIndex"     },
			{ ShaderDataType::Float,  "a_TilingFactor" },
			{ ShaderDataType::Float,  "a_EntityID"	   }
		});
		data.QuadVertexArray->AddVertexBuffer(data.QuadVertexBuffer);
		data.QuadVertexBufferBase = new QuadVertex[data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, data.MaxIndices);
		data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		data.WhiteTexture = Texture::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;

		data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[data.MaxTextureSlots];
		for (uint32_t i = 0; i < data.MaxTextureSlots; i++)
			samplers[i] = i;

		data.QuadShader = Shader::Create("Quad.glsl");

		data.QuadShader->Bind();
		data.QuadShader->SetIntArray("u_Textures", samplers, data.MaxTextureSlots);
		// Set first texture slot to 0
		data.TextureSlots[0] = data.WhiteTexture;

		data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		data.QuadVertexPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		data.QuadVertexPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };

		data.CameraUniformBuffer = UniformBuffer::Create(sizeof(RenderData::CameraData), 0);
	}


	void Render2D::BeginScene(const Ref<Camera>& camera)
	{
		data.CameraBuffer.View = camera->GetViewMatrix();
		data.CameraBuffer.Projection = camera->GetProjectionMatrix();
		data.CameraUniformBuffer->SetData(&data.CameraBuffer, sizeof(RenderData::CameraData));

		Render2D::StartBatch();
	}


	void Render2D::Flush() 
	{
		if (data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)data.QuadVertexBufferPtr - (uint8_t*)data.QuadVertexBufferBase);
			data.QuadVertexBuffer->SetData(data.QuadVertexBufferBase, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < data.TextureSlotIndex; i++)
				data.TextureSlots[i]->Bind(i);

			data.QuadShader->Bind();
			RenderGlobals::DrawIndexed(data.QuadVertexArray, data.QuadIndexCount);
			data.Stats.DrawCalls++;
		}
	}


	void Render2D::StartBatch() 
	{
		data.QuadIndexCount = 0;
		data.QuadVertexBufferPtr = data.QuadVertexBufferBase;

		data.TextureSlotIndex = 1;
	}


	void Render2D::NextBatch() 
	{
		Flush();
		StartBatch();
	}


	void Render2D::ResetStats() 
	{
		memset(&data.Stats, 0, sizeof(Statistics));
	}


	Render2D::Statistics Render2D::GetStats() 
	{
		return data.Stats;
	}


	void Render2D::EndScene() 
	{
		Flush();
	}


	void Render2D::Shutdown() 
	{
		delete[] data.QuadVertexBufferBase;
	}


	void Render2D::DrawSolidPolygon() 
	{
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::toMat4(glm::quat(rotation))
			* glm::scale(glm::mat4(1.0f), scale);

		size_t quadVertexCount = 4;
		float textureIndex = 0.0f; // White Texture
		glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		float tilingFactor = 1.0f;

		if (data.QuadIndexCount >= data.MaxIndices)
			Render2D::NextBatch();

		glm::vec3 color = glm::vec3(1, 0, 0);	// Change color in settings
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			data.QuadVertexBufferPtr->Position = transform * data.QuadVertexPositions[i];
			data.QuadVertexBufferPtr->Color = glm::vec4(color, 1);
			data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
			data.QuadVertexBufferPtr->TexIndex = textureIndex;
			data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			data.QuadVertexBufferPtr->EntityID = 0;	// UUID in the near future
			data.QuadVertexBufferPtr++;
		}

		data.QuadIndexCount += 6;
		data.Stats.QuadCount++;
	}

	void Render2D::DrawSolidPolygon(Entity& entity) 
	{
		auto& enttTrans = entity.GetComponent<TransformComponent>();

		glm::vec3 position{ enttTrans.position.x, enttTrans.position.y, enttTrans.position.z + 0.001f };
		glm::vec3 scale{ enttTrans.scale.x, enttTrans.scale.y, 1.0f };

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::toMat4(glm::quat(enttTrans.rotation))
			* glm::scale(glm::mat4(1.0f), scale);

		size_t quadVertexCount = 4;
		float textureIndex = 0.0f; // White Texture
		glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		float tilingFactor = 1.0f;

		if (data.QuadIndexCount >= data.MaxIndices)
			Render2D::NextBatch();

		glm::vec3 color = glm::vec3(1, 0, 0);	// Change color in settings
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			data.QuadVertexBufferPtr->Position = transform * data.QuadVertexPositions[i];
			data.QuadVertexBufferPtr->Color = glm::vec4(color, 1);
			data.QuadVertexBufferPtr->TexCoord = glm::vec2(0.0f, 0.0f);
			data.QuadVertexBufferPtr->TexIndex = textureIndex;
			data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			data.QuadVertexBufferPtr->EntityID = entity.GetUUID();
			data.QuadVertexBufferPtr++;
		}

		data.QuadIndexCount += 6;
		data.Stats.QuadCount++;
	}
}