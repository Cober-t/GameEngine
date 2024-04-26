#include <pch.h>
#include "Render/Primitives/Line.h"
#include "Render/RenderGlobals.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

    namespace Primitive {

        static Line::Data data;
        Line* Line::s_Instance = nullptr;

        Line::Line() 
        {
            s_Instance = this;
        }
        
        Line::~Line() 
        {
            delete[] data.VertexBufferBase;
        }

        void Line::Init() 
        {
            data.VertexArray = VertexArray::Create();

            data.VertexBuffer = VertexBuffer::Create(Data::MaxVertices * sizeof(Attributes));
            data.VertexBuffer->SetLayout({
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color"    },
                { ShaderDataType::Int,    "a_EntityID" }
            });
            data.VertexArray->AddVertexBuffer(data.VertexBuffer);
            data.VertexBufferBase = new Attributes[Data::MaxVertices];

            data.Shader = Shader::Create("Line.glsl");
        }


        void Line::NextBatch()
        {
            Line::Flush();
            Line::StartBatch();
        }

        void Line::EndBatch()
        {
            Flush();
        }

        uint32_t Line::GetVertexCount()
        {
            return data.VertexCount;
        }


        void Line::Flush()
        {
            uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
            data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

            data.Shader->Bind();
            RenderGlobals::SetLineWidth(data.thickness);
            RenderGlobals::DrawLines(data.VertexArray, data.VertexCount);
        }
        

        void Line::StartBatch()
        {
            data.VertexCount = 0;
            data.VertexBufferPtr = data.VertexBufferBase;	
        }

        
        void Line::Draw(Entity& entity)
        {
            glm::vec3 point1 = entity.GetComponent<Render2DComponent>().point1;
            glm::vec3 point2 = entity.GetComponent<Render2DComponent>().point2;
            glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
            data.thickness = entity.GetComponent<Render2DComponent>().thickness;

            SetAttributes(point1, point2, color, (int)entity);
        }


        void Line::Draw(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID)
        {
            SetAttributes(p0, p1, color, (int)entityID);
        }


        void Line::SetAttributes(const glm::vec3& p0, glm::vec3& p1, const glm::vec4& color, int entityID) 
        {
            data.VertexBufferPtr->Position = p0;
            data.VertexBufferPtr->Color = color;
            data.VertexBufferPtr->EntityID = entityID;
            data.VertexBufferPtr++;

            data.VertexBufferPtr->Position = p1;
            data.VertexBufferPtr->Color = color;
            data.VertexBufferPtr->EntityID = entityID;
            data.VertexBufferPtr++;
            
            data.VertexCount += 2;
        }
    }
}