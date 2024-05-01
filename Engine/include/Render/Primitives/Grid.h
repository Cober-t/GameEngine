#ifndef GRID_H
#define GRID_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Scene/ECS.h"

#include "Render/Camera/EditorCamera.h"

namespace Cober  {

    namespace Primitive {

        class Grid
        {
        public:
            Grid();
            ~Grid();

            static void Init();

            static void Flush();
            static void Draw(const Ref<EditorCamera>& camera);

        public:
            struct Attributes
            {
                glm::vec3 Position;
                glm::vec3 CameraPosition;
                int Game2D;
                glm::vec4 PatternSizes;
                float PatternNumber;
                float Opacity;
            };

            struct Data
            {
                Ref<VertexArray> VertexArray;
                Ref<VertexBuffer> VertexBuffer;
                Ref<Shader> Shader;

                uint32_t IndexCount = 0;

                static const uint32_t MaxGrid = 20000;
                static const uint32_t MaxVertices = MaxGrid * 4;
                static const uint32_t MaxIndices = MaxGrid * 6;
                
                Attributes* VertexBufferBase = nullptr;
                Attributes* VertexBufferPtr = nullptr;

                glm::vec4 VertexPositions[4];

                glm::vec4 GridPatternSizes = glm::vec4(1.0f, 10.0f, 0.0, 0.0f);
                int GridPatternNumber = 2;
                float GridOpacity = 1.0f;
            };

            Attributes* attributes;
        
        private:
            static Grid* s_Instance;
        };
    }
}


#endif