#ifndef QUAD_H
#define QUAD_H

#include "Render/Primitives/Primitive.h"
#include "Render/Texture.h"
#include "Scene/ECS.h"

namespace Cober {

	class Render2D;

	class Quad : public Primitive
	{
	public:
		Quad();
		virtual ~Quad() override;
		
		virtual void Init() override;

		void Draw(Entity& entity);

		virtual void Flush() override;
		virtual void StartBatch() override;
		virtual void NextBatch() override;
		virtual void EndBatch() override;
		uint32_t GetIndexCount() const;

	public:

		struct Attributes
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;

			// Editor-only
			int EntityID;
		};

		struct Data
		{
			Ref<VertexArray> VertexArray;
			Ref<VertexBuffer> VertexBuffer;
			Ref<Shader> Shader;
			Ref<Texture> WhiteTexture;

			uint32_t IndexCount = 0;

			static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
			std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotIndex = 1; // 0 = white texture

			static const uint32_t MaxQuads = 20000;
			static const uint32_t MaxVertices = MaxQuads * 4; // Add Lines, Circles and Cubes to the count
			static const uint32_t MaxIndices = MaxQuads * 6;  // Add Lines, Circles and Cubes to the count
			
			Attributes* VertexBufferBase = nullptr;
			Attributes* VertexBufferPtr = nullptr;

			glm::vec4 VertexPositions[4];
			size_t VertexCount = 4;
		};

	private:
		void SetAttributes(const glm::mat4& transform, const glm::vec4& color, int textureIndex, const glm::vec2* textureCoords, float tilingFactor, int entityID);
	};
}

#endif