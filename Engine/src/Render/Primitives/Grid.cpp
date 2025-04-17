#include <pch.h>
#include "Render/Primitives/Grid.h"
#include "Render/RenderGlobals.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

    namespace Primitive {

        static Grid::Data data;
        Grid* Grid::s_Instance = nullptr;


        Grid::Grid() 
        {
            s_Instance = this;
        }


        Grid::~Grid() 
        {
            delete[] data.VertexBufferBase;
        }


        void Grid::Init() 
        {
            data.VertexArray = VertexArray::Create();

            data.VertexBuffer = VertexBuffer::Create(data.MaxVertices * sizeof(Attributes));
            data.VertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position"		 },
                { ShaderDataType::Float3, "a_CameraPosition" },
                { ShaderDataType::Int,    "a_Game2D"		 },
                { ShaderDataType::Float4, "a_PatternSizes"	 },
                { ShaderDataType::Float,  "a_PatternNumber"	 },
                { ShaderDataType::Float,  "a_Opacity"        },
                });
            data.VertexArray->AddVertexBuffer(data.VertexBuffer);
            data.VertexBufferBase = new Attributes[Data::MaxVertices];

            uint32_t* gridIndices = new uint32_t[Data::MaxIndices];
                
            uint32_t offset = 0;
            for (uint32_t i = 0; i < Data::MaxIndices; i += 6)
            {
                gridIndices[i + 0] = offset + 0;
                gridIndices[i + 1] = offset + 1;
                gridIndices[i + 2] = offset + 2;

                gridIndices[i + 3] = offset + 2;
                gridIndices[i + 4] = offset + 3;
                gridIndices[i + 5] = offset + 0;

                offset += 4;
            }

            Ref<IndexBuffer> gridIB = IndexBuffer::Create(gridIndices, Data::MaxIndices);
            data.VertexArray->SetIndexBuffer(gridIB); // The shame as Quads Index Buffer
            delete[] gridIndices;

            data.Shader = Shader::Create("Grid.glsl");

            data.VertexPositions[0] = { -1.0f, -1.0f, 0.0f, 1.0f };
            data.VertexPositions[1] = {  1.0f, -1.0f, 0.0f, 1.0f };
            data.VertexPositions[2] = {  1.0f,  1.0f, 0.0f, 1.0f };
            data.VertexPositions[3] = { -1.0f,  1.0f, 0.0f, 1.0f };

            data.VertexBufferPtr = data.VertexBufferBase;
        }

        void Grid::Flush()
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
            data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);
            
            data.Shader->Bind();

            RenderGlobals::DrawIndexed(data.VertexArray, (uint32_t)6);
        }


        void Grid::Draw(const Ref<EditorCamera>& camera) 
        {

            glm::vec3 rotation{ 90.0f, 0.0f, 0.0f };
            glm::vec3 position = glm::vec3(0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                glm::toMat4(glm::quat(glm::radians(rotation))) *
                                glm::scale(glm::mat4(1.0f), { 100.0f, 100.0f, 1.0f });

            constexpr size_t gridVertexCount = 4;
            for (size_t i = 0; i < gridVertexCount; i++) 
            {
                data.VertexBufferPtr->Position = transform * data.VertexPositions[i];
                data.VertexBufferPtr->CameraPosition = glm::vec3(0.0f);
                data.VertexBufferPtr->Game2D = 0;
                data.VertexBufferPtr->PatternSizes = data.GridPatternSizes;
                data.VertexBufferPtr->PatternNumber = data.GridPatternNumber;
                data.VertexBufferPtr->Opacity = data.GridOpacity;

                data.VertexBufferPtr++;
            }
        }
    }
}