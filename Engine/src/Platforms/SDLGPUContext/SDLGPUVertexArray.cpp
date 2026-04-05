
#include <pch.h>
#include "Platforms/SDLGPU/SDLGPUVertexArray.h"

namespace Cober {

    void SDLGPUVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        LOG_CORE_ASSERT(vertexBuffer, "VertexArray received a null vertex buffer");
        LOG_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout");
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void SDLGPUVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        LOG_CORE_ASSERT(indexBuffer, "VertexArray received a null index buffer");
        m_IndexBuffer = indexBuffer;
    }
}
