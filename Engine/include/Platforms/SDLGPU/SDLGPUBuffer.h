
#ifndef SDLGPU_BUFFER_H
#define SDLGPU_BUFFER_H

#include "Render/Buffer.h"

struct SDL_GPUBuffer;
struct SDL_GPUCommandBuffer;

namespace Cober {

    class SDLGPUVertexBuffer : public VertexBuffer
    {
    public:
        explicit SDLGPUVertexBuffer(uint32_t size);
        SDLGPUVertexBuffer(float* vertices, uint32_t size);
        ~SDLGPUVertexBuffer() override;

        void Bind() const override {}
        void Unbind() const override {}

        void SetData(const void* data, uint32_t size) override;

        const BufferLayout& GetLayout() const override { return m_Layout; }
        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

        SDL_GPUBuffer* GetGPUBuffer() const { return m_Buffer; }
        uint32_t GetSize() const { return m_Size; }
        void EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle = true);

    private:
        SDL_GPUBuffer* m_Buffer = nullptr;
        uint32_t m_Size = 0;
        BufferLayout m_Layout;
        std::vector<uint8_t> m_Shadow;
        bool m_Dirty = false;
    };


    class SDLGPUIndexBuffer : public IndexBuffer
    {
    public:
        SDLGPUIndexBuffer(uint32_t* indices, uint32_t count);
        ~SDLGPUIndexBuffer() override;

        void Bind() const override {}
        void Unbind() const override {}

        uint32_t GetCount() const override { return m_Count; }

        SDL_GPUBuffer* GetGPUBuffer() const { return m_Buffer; }
        void EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle = false);

    private:
        SDL_GPUBuffer* m_Buffer = nullptr;
        uint32_t m_Count = 0;
        std::vector<uint8_t> m_Shadow;
        bool m_Dirty = false;
    };


    class SDLGPUUniformBuffer : public UniformBuffer
    {
    public:
        SDLGPUUniformBuffer(uint32_t size, uint32_t binding);
        ~SDLGPUUniformBuffer() override = default;

        void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

        uint32_t GetBinding() const { return m_Binding; }
        const std::vector<uint8_t>& GetBytes() const { return m_Bytes; }

        static const SDLGPUUniformBuffer* GetBound(uint32_t binding);

    private:
        uint32_t m_Binding = 0;
        std::vector<uint8_t> m_Bytes;
        static std::unordered_map<uint32_t, const SDLGPUUniformBuffer*> s_BoundUniforms;
    };
}

#endif
