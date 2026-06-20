#include <pch.h>
#include "Render/Buffer.h"
#include "Render/GraphicsDevice.h"
#include <SDL3/SDL_gpu.h>

namespace Cober {

std::unordered_map<uint32_t, const UniformBuffer*> UniformBuffer::s_BoundUniforms;

namespace {

static SDL_GPUDevice* GetDevice()
{
    return GraphicsDevice::Get().GetDevice();
}

// Helper: upload data from CPU to an SDL_GPUBuffer via a transfer buffer.
//
// SDL3 GPU transfer flow:
//   1. Create an SDL_GPUTransferBuffer (CPU-visible memory)
//   2. Map it and write data with SDL_memcpy
//   3. Unmap it
//   4. Begin a SDL_GPUCopyPass on a command buffer
//   5. Call SDL_UploadToGPUBuffer to schedule the copy
//   6. End the copy pass
//   7. Submit the command buffer (if we created our own)
//
static void UploadBytes(SDL_GPUBuffer* buffer, const void* data, uint32_t size,
                        SDL_GPUCommandBuffer* existingCommandBuffer, bool cycle)
{
    if (!buffer || !data || size == 0)
        return;

    SDL_GPUDevice* device = GetDevice();

    // Step 1-3: Create transfer buffer and fill it
    SDL_GPUTransferBufferCreateInfo buffCreateInfo{};
    buffCreateInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    buffCreateInfo.size = size;
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &buffCreateInfo);
    LOG_CORE_ASSERT(transferBuffer, "SDL_CreateGPUTransferBuffer failed: {0}", SDL_GetError());

    void* mapped = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    LOG_CORE_ASSERT(mapped, "SDL_MapGPUTransferBuffer failed: {0}", SDL_GetError());
    SDL_memcpy(mapped, data, size);
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);

    // Step 4-7: Copy to GPU buffer
    bool ownCmdBuffer = existingCommandBuffer == nullptr;
    SDL_GPUCommandBuffer* commandBuffer = existingCommandBuffer
        ? existingCommandBuffer
        : SDL_AcquireGPUCommandBuffer(device);
    LOG_CORE_ASSERT(commandBuffer, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

    SDL_GPUTransferBufferLocation buffLocation{};
    buffLocation.transfer_buffer = transferBuffer;
    buffLocation.offset = 0;

    SDL_GPUBufferRegion buffRegion{};
    buffRegion.buffer = buffer;
    buffRegion.offset = 0;
    buffRegion.size = size;

    SDL_UploadToGPUBuffer(copyPass, &buffLocation, &buffRegion, cycle);

    SDL_EndGPUCopyPass(copyPass);

    if (ownCmdBuffer)
        SDL_SubmitGPUCommandBuffer(commandBuffer);

    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
}

} // anonymous namespace

// ======================================================================================
// VertexBuffer
// ======================================================================================

VertexBuffer::VertexBuffer(uint32_t size)
    : m_Size(size), m_Shadow(size)
{
    SDL_GPUBufferCreateInfo buffInfo{};
    buffInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    buffInfo.size = size;

    m_Buffer = SDL_CreateGPUBuffer(GetDevice(), &buffInfo);
    LOG_CORE_ASSERT(m_Buffer, "SDL_CreateGPUBuffer(VERTEX) failed: {0}", SDL_GetError());
}

VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
    : VertexBuffer(size)
{
    SetData(vertices, size);
}

VertexBuffer::~VertexBuffer()
{
    if (m_Buffer)
    {
        SDL_ReleaseGPUBuffer(GetDevice(), m_Buffer);
        m_Buffer = nullptr;
    }
}

void VertexBuffer::SetData(const void* data, uint32_t size)
{
    LOG_CORE_ASSERT(size <= m_Size, "VertexBuffer::SetData overflow");
    if (m_Shadow.size() < size)
        m_Shadow.resize(size);
    SDL_memcpy(m_Shadow.data(), data, size);
    m_Dirty = true;
}

void VertexBuffer::EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle)
{
    if (!m_Dirty)
        return;

    UploadBytes(m_Buffer, m_Shadow.data(), m_Size, commandBuffer, cycle);
    m_Dirty = false;
}

Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
{
    return CreateRef<VertexBuffer>(size);
}

Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
{
    return CreateRef<VertexBuffer>(vertices, size);
}

// ======================================================================================
// IndexBuffer
// ======================================================================================

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
    : m_Count(count), m_Shadow(count * sizeof(uint32_t))
{
    SDL_GPUBufferCreateInfo buffInfo{};
    buffInfo.usage = SDL_GPU_BUFFERUSAGE_INDEX;
    buffInfo.size = (uint32_t)m_Shadow.size();

    m_Buffer = SDL_CreateGPUBuffer(GetDevice(), &buffInfo);
    LOG_CORE_ASSERT(m_Buffer, "SDL_CreateGPUBuffer(INDEX) failed: {0}", SDL_GetError());

    SDL_memcpy(m_Shadow.data(), indices, m_Shadow.size());
    m_Dirty = true;
}

IndexBuffer::~IndexBuffer()
{
    if (m_Buffer)
    {
        SDL_ReleaseGPUBuffer(GetDevice(), m_Buffer);
        m_Buffer = nullptr;
    }
}

void IndexBuffer::EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle)
{
    if (!m_Dirty)
        return;

    UploadBytes(m_Buffer, m_Shadow.data(), (uint32_t)m_Shadow.size(), commandBuffer, cycle);
    m_Dirty = false;
}

Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
    return CreateRef<IndexBuffer>(indices, count);
}

// ======================================================================================
// UniformBuffer
// ======================================================================================

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
    : m_Binding(binding), m_Bytes(size, 0)
{
    s_BoundUniforms[binding] = this;
}

void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
{
    LOG_CORE_ASSERT(offset + size <= m_Bytes.size(), "UniformBuffer::SetData overflow");
    SDL_memcpy(m_Bytes.data() + offset, data, size);
    s_BoundUniforms[m_Binding] = this;
}

const UniformBuffer* UniformBuffer::GetBound(uint32_t binding)
{
    auto it = s_BoundUniforms.find(binding);
    return it == s_BoundUniforms.end() ? nullptr : it->second;
}

Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
{
    return CreateRef<UniformBuffer>(size, binding);
}

} // namespace Cober
