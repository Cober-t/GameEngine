#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <memory>
#include "Render/Buffer.h"

// --------------------------------------------------------------------------------------
// VertexArray
//
// A VertexArray combines one or more vertex buffers and an optional index buffer into
// a single drawable unit. It describes the vertex input state needed by the graphics
// pipeline.
//
// In SDL3 GPU, vertex input is defined by:
//   - SDL_GPUVertexBufferDescription: describes each vertex buffer's layout
//   - SDL_GPUVertexAttribute: describes each attribute's format and location
//   These are stored in an SDL_GPUVertexInputState struct used when creating pipelines.
// --------------------------------------------------------------------------------------

namespace Cober {

class CB_API VertexArray
{
public:
    VertexArray() = default;
    ~VertexArray() = default;

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);
    void Unbind() const {} // No-op: SDL3 GPU doesn't use bind/unbind for VAOs

    const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
    const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

    static Ref<VertexArray> Create();

private:
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};

}

#endif
