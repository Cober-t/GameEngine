
#include <pch.h>
#include "Platforms/SDLGPU/SDLGPUBuffer.h"
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"

#include <SDL3/SDL_gpu.h>

namespace Cober {

    std::unordered_map<uint32_t, const SDLGPUUniformBuffer*> SDLGPUUniformBuffer::s_BoundUniforms;

    namespace {

        static SDL_GPUDevice* GetDevice()
        {
            auto* api = SDLGPURenderAPI::Get();
            LOG_CORE_ASSERT(api, "SDLGPURenderAPI has not been initialized yet");
            return api->GetDevice();
        }

        static void UploadBytes(SDL_GPUBuffer* buffer, const void* data, uint32_t size, SDL_GPUCommandBuffer* existingCommandBuffer, bool cycle)
        {
            if (!buffer || !data || size == 0)
            {
                return;
            }

            SDL_GPUDevice* device = GetDevice();

            SDL_GPUTransferBufferCreateInfo buffCreateInfo{};
            buffCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            buffCreateInfo.size = size;
            SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer( device, &buffCreateInfo );

            LOG_CORE_ASSERT(transferBuffer, "SDL_CreateGPUTransferBuffer failed: {0}", SDL_GetError());

            void* mapped = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
            LOG_CORE_ASSERT(mapped, "SDL_MapGPUTransferBuffer failed: {0}", SDL_GetError());
            SDL_memcpy(mapped, data, size);
            SDL_UnmapGPUTransferBuffer(device, transferBuffer);

            bool ownCmdBuffer = existingCommandBuffer == nullptr;
            SDL_GPUCommandBuffer* commandBuffer = existingCommandBuffer ? existingCommandBuffer : SDL_AcquireGPUCommandBuffer(device);
            LOG_CORE_ASSERT(commandBuffer, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

            SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

            SDL_GPUTransferBufferLocation buffLocation{};
            buffLocation.transfer_buffer = transferBuffer;
            buffLocation.offset = 0;
            
            SDL_GPUBufferRegion buffRegion{};
            buffRegion.buffer = buffer;
            buffRegion.offset = 0;
            buffRegion.size = size;

            SDL_UploadToGPUBuffer( copyPass, &buffLocation, &buffRegion, cycle );

            SDL_EndGPUCopyPass(copyPass);

            if (ownCmdBuffer)
            {
                SDL_SubmitGPUCommandBuffer(commandBuffer);
            }

            SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        }
    }

    SDLGPUVertexBuffer::SDLGPUVertexBuffer(uint32_t size)
        : m_Size(size), m_Shadow(size)
    {
        SDL_GPUBufferCreateInfo buffInfo {};
        buffInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        buffInfo.size = size;

        m_Buffer = SDL_CreateGPUBuffer( GetDevice(), &buffInfo );

        LOG_CORE_ASSERT(m_Buffer, "SDL_CreateGPUBuffer(VERTEX) failed: {0}", SDL_GetError());
    }

    SDLGPUVertexBuffer::SDLGPUVertexBuffer(float* vertices, uint32_t size)
        : SDLGPUVertexBuffer(size)
    {
        SetData(vertices, size);
    }

    SDLGPUVertexBuffer::~SDLGPUVertexBuffer()
    {
        if (m_Buffer)
        {
            SDL_ReleaseGPUBuffer(GetDevice(), m_Buffer);
            m_Buffer = nullptr;
        }
    }

    void SDLGPUVertexBuffer::SetData(const void* data, uint32_t size)
    {
        LOG_CORE_ASSERT(size <= m_Size, "VertexBuffer::SetData overflow");
        if (m_Shadow.size() < size)
        {
            m_Shadow.resize(size);
        }
        SDL_memcpy(m_Shadow.data(), data, size);
        m_Dirty = true;
    }

    void SDLGPUVertexBuffer::EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle)
    {
        if (!m_Dirty)
        {
            return;
        }

        UploadBytes(m_Buffer, m_Shadow.data(), m_Size, commandBuffer, cycle);
        m_Dirty = false;
    }

    SDLGPUIndexBuffer::SDLGPUIndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count), m_Shadow(count * sizeof(uint32_t))
    {
        SDL_GPUBufferCreateInfo buffInfo {};
        buffInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        buffInfo.size = (uint32_t)m_Shadow.size();

        m_Buffer = SDL_CreateGPUBuffer( GetDevice(), &buffInfo );
        
        LOG_CORE_ASSERT(m_Buffer, "SDL_CreateGPUBuffer(INDEX) failed: {0}", SDL_GetError());

        SDL_memcpy(m_Shadow.data(), indices, m_Shadow.size());
        m_Dirty = true;
    }

    SDLGPUIndexBuffer::~SDLGPUIndexBuffer()
    {
        if (m_Buffer)
        {
            SDL_ReleaseGPUBuffer(GetDevice(), m_Buffer);
            m_Buffer = nullptr;
        }
    }

    void SDLGPUIndexBuffer::EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle)
    {
        if (!m_Dirty)
        {
            return;
        }

        UploadBytes(m_Buffer, m_Shadow.data(), (uint32_t)m_Shadow.size(), commandBuffer, cycle);
        m_Dirty = false;
    }

    SDLGPUUniformBuffer::SDLGPUUniformBuffer(uint32_t size, uint32_t binding)
        : m_Binding(binding), m_Bytes(size, 0)
    {
        s_BoundUniforms[binding] = this;
    }

    void SDLGPUUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
    {
        LOG_CORE_ASSERT(offset + size <= m_Bytes.size(), "UniformBuffer::SetData overflow");
        SDL_memcpy(m_Bytes.data() + offset, data, size);
        s_BoundUniforms[m_Binding] = this;
    }

    const SDLGPUUniformBuffer* SDLGPUUniformBuffer::GetBound(uint32_t binding)
    {
        auto it = s_BoundUniforms.find(binding);
        return it == s_BoundUniforms.end() ? nullptr : it->second;
    }
}
