#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"

#include "Render/RenderGlobals.h"

namespace Cober  {

    class Primitive
    {
    public:
    
        Primitive() = default;
        virtual ~Primitive() = default;
        virtual void Init();
		virtual void Flush();
		virtual void StartBatch();
		virtual void NextBatch();
		virtual void EndBatch();
    
    public:

        struct PrimitiveAttributes
		{
            glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;

			// Editor-only
			int EntityID;
		};

        struct PrimitiveData
		{
			Ref<VertexArray> VertexArray;
			Ref<VertexBuffer> VertexBuffer;
			Ref<Shader> Shader;
			Ref<Texture> WhiteTexture;

			uint32_t IndexCount = 0;

			static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps
			std::array<Ref<Texture>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotIndex = 1; // 0 = white texture
		};
    };
}


#endif