
#ifndef SDLGPU_VERTEX_ARRAY_H
#define SDLGPU_VERTEX_ARRAY_H

#include "Render/VertexArray.h"

namespace Cober {

    class SDLGPUVertexArray : public VertexArray
    {
    public:
        SDLGPUVertexArray() = default;
        ~SDLGPUVertexArray() override = default;

        void Bind() const override {}
        void Unbind() const override {}

        void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
        const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

    private:
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };
}

#endif
