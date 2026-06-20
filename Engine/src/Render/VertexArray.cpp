#include <pch.h>
#include "Render/VertexArray.h"

namespace Cober {

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
    LOG_CORE_ASSERT(vertexBuffer, "VertexArray received a null vertex buffer");
    LOG_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(),
        "Vertex buffer has no layout — call SetLayout() before adding");
    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
    LOG_CORE_ASSERT(indexBuffer, "VertexArray received a null index buffer");
    m_IndexBuffer = indexBuffer;
}

Ref<VertexArray> VertexArray::Create()
{
    return CreateRef<VertexArray>();
}

} // namespace Cober
