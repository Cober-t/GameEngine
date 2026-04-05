
#include <pch.h>
#include "Platforms/SDLGPU/SDLGPUFramebuffer.h"
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"

#include <SDL3/SDL_gpu.h>

namespace Cober {

    namespace {

        static SDL_GPUDevice* GetDevice()
        {
            auto* api = SDLGPURenderAPI::Get();
            LOG_CORE_ASSERT(api, "SDLGPURenderAPI has not been initialized yet");
            return api->GetDevice();
        }

        static SDL_GPUTextureFormat ToSDLFramebufferFormat(FramebufferTextureFormat format)
        {
            switch (format)
            {
                case FramebufferTextureFormat::RGBA8:          return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
                case FramebufferTextureFormat::RED_INTEGER:    return SDL_GPU_TEXTUREFORMAT_R32_INT;
                case FramebufferTextureFormat::DEPTH24STENCIL8:return SDL_GPU_TEXTUREFORMAT_D24_UNORM_S8_UINT;
                default: break;
            }

            LOG_CORE_ASSERT(false, "Unsupported framebuffer format");
            return SDL_GPU_TEXTUREFORMAT_INVALID;
        }

        static bool IsDepthFormat(FramebufferTextureFormat format)
        {
            return format == FramebufferTextureFormat::DEPTH24STENCIL8;
        }
    }

    SDLGPUFramebuffer::SDLGPUFramebuffer(uint32_t width, uint32_t height)
    {
        FramebufferSpecification spec;
        spec.Attachments = {
            FramebufferTextureFormat::RGBA8,
            FramebufferTextureFormat::RED_INTEGER,
            FramebufferTextureFormat::Depth
        };
        spec.Width = width;
        spec.Height = height;
        m_Specification = spec;

        for (const auto& attachment : m_Specification.Attachments.Attachments)
        {
            if (IsDepthFormat(attachment.TextureFormat))
                m_DepthAttachmentSpecification = attachment;
            else
                m_ColorAttachmentSpecifications.push_back(attachment);
        }

        Invalidate();
    }

    SDLGPUFramebuffer::~SDLGPUFramebuffer()
    {
        Release();
    }

    void SDLGPUFramebuffer::Bind()
    {
        auto* api = SDLGPURenderAPI::Get();
        LOG_CORE_ASSERT(api, "SDLGPURenderAPI is not initialized");
        api->BeginFramebufferRenderPass(this);
    }

    void SDLGPUFramebuffer::Unbind()
    {
        auto* api = SDLGPURenderAPI::Get();
        LOG_CORE_ASSERT(api, "SDLGPURenderAPI is not initialized");
        api->EndActiveRenderPass();
    }

    void SDLGPUFramebuffer::Invalidate()
    {
        Release();

        SDL_GPUDevice* device = GetDevice();

        for (const auto& spec : m_ColorAttachmentSpecifications)
        {
            SDL_GPUTextureCreateInfo texInfo{};
            texInfo.type = SDL_GPU_TEXTURETYPE_2D;
            texInfo.format = ToSDLFramebufferFormat(spec.TextureFormat);
            texInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
            texInfo.width = m_Specification.Width;
            texInfo.height = m_Specification.Height;
            texInfo.layer_count_or_depth = 1;
            texInfo.num_levels = 1;
            texInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;
            
            SDL_GPUTexture* texture = SDL_CreateGPUTexture( device, &texInfo );
            LOG_CORE_ASSERT(texture, "SDL_CreateGPUTexture(color attachment) failed: {0}", SDL_GetError());

            m_ColorAttachments.push_back(texture);

            SDL_GPUSamplerCreateInfo samplerInfo {};
            samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
            samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
            samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
            samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            samplerInfo.min_lod = 0.0f;
            samplerInfo.max_lod = 0.0f;
            
            SDL_GPUSampler* sampler = SDL_CreateGPUSampler( device, &samplerInfo );
            LOG_CORE_ASSERT(sampler, "SDL_CreateGPUSampler(framebuffer attachment) failed: {0}", SDL_GetError());
            m_ColorAttachmentSamplers.push_back(sampler);
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
        {
            SDL_GPUTextureCreateInfo texInfo {};
            texInfo.type = SDL_GPU_TEXTURETYPE_2D;
            texInfo.format = ToSDLFramebufferFormat(m_DepthAttachmentSpecification.TextureFormat);
            texInfo.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
            texInfo.width = m_Specification.Width;
            texInfo.height = m_Specification.Height;
            texInfo.layer_count_or_depth = 1;
            texInfo.num_levels = 1,
            texInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;

            m_DepthAttachment = SDL_CreateGPUTexture( device, &texInfo );

            LOG_CORE_ASSERT(m_DepthAttachment, "SDL_CreateGPUTexture(depth attachment) failed: {0}", SDL_GetError());
        }

        SDL_GPUTransferBufferCreateInfo buffInfo {};
        buffInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
        buffInfo.size = sizeof(int32_t);
        m_ReadbackBuffer = SDL_CreateGPUTransferBuffer( device, &buffInfo );

        LOG_CORE_ASSERT(m_ReadbackBuffer, "SDL_CreateGPUTransferBuffer(download) failed: {0}", SDL_GetError());

        ClearAttachment(1, -1);
    }

    void SDLGPUFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0)
        {
            return;
        }

        m_Specification.Width = width;
        m_Specification.Height = height;
        Invalidate();
    }

    int SDLGPUFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        LOG_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds");

        SDL_GPUDevice* device = GetDevice();
        SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(device);
        LOG_CORE_ASSERT(cmdbuf, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdbuf);

        SDL_GPUTextureRegion textureRegion {};
        textureRegion.texture = m_ColorAttachments[attachmentIndex];
        textureRegion.mip_level = 0;
        textureRegion.layer = 0;
        textureRegion.x = (uint32_t)x;
        textureRegion.y = (uint32_t)y;
        textureRegion.z = 0;
        textureRegion.w = 1;
        textureRegion.h = 1;
        textureRegion.d = 1;

        SDL_GPUTextureTransferInfo transferInfo{};
        transferInfo.transfer_buffer = m_ReadbackBuffer;
        transferInfo.offset = 0;
        transferInfo.pixels_per_row = 1;
        transferInfo.rows_per_layer = 1;
        SDL_DownloadFromGPUTexture( copyPass, &textureRegion, &transferInfo );

        SDL_EndGPUCopyPass(copyPass);

        SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdbuf);
        SDL_WaitForGPUFences(device, true, &fence, 1);
        SDL_ReleaseGPUFence(device, fence);

        void* mapped = SDL_MapGPUTransferBuffer(device, m_ReadbackBuffer, false);
        LOG_CORE_ASSERT(mapped, "SDL_MapGPUTransferBuffer(download) failed: {0}", SDL_GetError());
        int value = *reinterpret_cast<int*>(mapped);
        SDL_UnmapGPUTransferBuffer(device, m_ReadbackBuffer);

        return value;
    }

    void SDLGPUFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        LOG_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of bounds");

        SDL_GPUDevice* device = GetDevice();

        std::vector<int> clearData(m_Specification.Width * m_Specification.Height, value);

        SDL_GPUTransferBufferCreateInfo buffInfo {};
        buffInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        buffInfo.size = (uint32_t)(clearData.size() * sizeof(int));
        SDL_GPUTransferBuffer* upload = SDL_CreateGPUTransferBuffer( device, &buffInfo );

        LOG_CORE_ASSERT(upload, "SDL_CreateGPUTransferBuffer(upload) failed: {0}", SDL_GetError());

        void* mapped = SDL_MapGPUTransferBuffer(device, upload, false);
        SDL_memcpy(mapped, clearData.data(), clearData.size() * sizeof(int));
        SDL_UnmapGPUTransferBuffer(device, upload);

        SDL_GPUCommandBuffer* cmdbuf = SDL_AcquireGPUCommandBuffer(device);
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmdbuf);
        SDL_GPUTextureTransferInfo textureTransferInfo {};
        textureTransferInfo.transfer_buffer = upload;
        textureTransferInfo.offset = 0;
        textureTransferInfo.pixels_per_row = m_Specification.Width;
        textureTransferInfo.rows_per_layer = m_Specification.Height;

        SDL_GPUTextureRegion textureRegion {};
        textureRegion.texture = m_ColorAttachments[attachmentIndex];
        textureRegion.mip_level = 0;
        textureRegion.layer = 0;
        textureRegion.x = 0;
        textureRegion.y = 0;
        textureRegion.z = 0;
        textureRegion.w = m_Specification.Width;
        textureRegion.h = m_Specification.Height;
        textureRegion.d = 1;
        
        SDL_UploadToGPUTexture( copyPass, &textureTransferInfo, &textureRegion, false);

        SDL_EndGPUCopyPass(copyPass);

        SDL_SubmitGPUCommandBuffer(cmdbuf);
        SDL_ReleaseGPUTransferBuffer(device, upload);
    }

    uintptr_t SDLGPUFramebuffer::GetColorAttachmentRenderID(uint32_t index) const
    {
        LOG_CORE_ASSERT(index < m_ColorAttachments.size(), "Attachment index out of bounds");
        return reinterpret_cast<uintptr_t>(m_ColorAttachments[index]);
    }

    SDL_GPUTexture* SDLGPUFramebuffer::GetColorAttachmentTexture(uint32_t index) const
    {
        LOG_CORE_ASSERT(index < m_ColorAttachments.size(), "Attachment index out of bounds");
        return m_ColorAttachments[index];
    }

    void SDLGPUFramebuffer::Release()
    {
        auto* api = SDLGPURenderAPI::Get();
        SDL_GPUDevice* device = api ? api->GetDevice() : nullptr;
        if (!device)
        {
            return;
        }

        for (auto* sampler : m_ColorAttachmentSamplers)
        {
            if (sampler)
                SDL_ReleaseGPUSampler(device, sampler);
        }
        m_ColorAttachmentSamplers.clear();

        for (auto* texture : m_ColorAttachments)
        {
            if (texture)
                SDL_ReleaseGPUTexture(device, texture);
        }
        m_ColorAttachments.clear();

        if (m_DepthAttachment)
        {
            SDL_ReleaseGPUTexture(device, m_DepthAttachment);
            m_DepthAttachment = nullptr;
        }

        if (m_ReadbackBuffer)
        {
            SDL_ReleaseGPUTransferBuffer(device, m_ReadbackBuffer);
            m_ReadbackBuffer = nullptr;
        }
    }
}
