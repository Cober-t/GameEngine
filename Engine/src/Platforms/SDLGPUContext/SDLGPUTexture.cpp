
#include <pch.h>
#include "Platforms/SDLGPU/SDLGPUTexture.h"
#include "Platforms/SDLGPU/SDLGPURenderAPI.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <SDL3/SDL_gpu.h>

namespace Cober {

    std::array<const SDLGPUTexture*, 32> SDLGPUTexture::s_BoundTextures{};
    std::array<SDL_GPUTexture*, 32> SDLGPUTexture::s_RawBoundTextures{};
    std::array<SDL_GPUSampler*, 32> SDLGPUTexture::s_RawSamplers{};

    namespace {

        static SDL_GPUDevice* GetDevice()
        {
            auto* api = SDLGPURenderAPI::Get();
            LOG_CORE_ASSERT(api, "SDLGPURenderAPI has not been initialized yet");
            return api->GetDevice();
        }

        static SDL_GPUTextureFormat ToSDLFormat(ImageFormat format)
        {
            switch (format)
            {
                case ImageFormat::R8:      return SDL_GPU_TEXTUREFORMAT_R8_UNORM;
                case ImageFormat::RGB8:    return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM; // RGB uploads are expanded to RGBA for portability
                case ImageFormat::RGBA8:   return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
                case ImageFormat::RGBA32F: return SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT;
                default: break;
            }

            LOG_CORE_ASSERT(false, "Unsupported ImageFormat");
            return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        }

        static SDL_GPUFilter ToSDLFilter(ImageFilter filter)
        {
            switch (filter)
            {
                case ImageFilter::NEAREST: return SDL_GPU_FILTER_NEAREST;
                case ImageFilter::LINEAR:  return SDL_GPU_FILTER_LINEAR;
                default: break;
            }

            LOG_CORE_ASSERT(false, "Unsupported ImageFilter");
            return SDL_GPU_FILTER_LINEAR;
        }

        static SDL_GPUSamplerAddressMode ToSDLAddressMode(RepeatPattern pattern)
        {
            switch (pattern)
            {
                case RepeatPattern::REPEAT:          return SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
                case RepeatPattern::CLAM_TO_EDGE:    return SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
                case RepeatPattern::MIRRORED_REPEAT: return SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
                default: break;
            }

            LOG_CORE_ASSERT(false, "Unsupported RepeatPattern");
            return SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
        }

        static void UploadTextureBytes(SDL_GPUTexture* texture, const void* data, uint32_t width, uint32_t height, uint32_t bytesPerPixel, SDL_GPUCommandBuffer* existingCommandBuffer, bool cycle)
        {
            SDL_GPUDevice* device = GetDevice();
            uint32_t totalSize = width * height * bytesPerPixel;

            SDL_GPUTransferBufferCreateInfo buffInfo {};
            buffInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            buffInfo.size = totalSize;
            SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer( device, &buffInfo );

            LOG_CORE_ASSERT(transferBuffer, "SDL_CreateGPUTransferBuffer failed: {0}", SDL_GetError());

            void* mapped = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
            LOG_CORE_ASSERT(mapped, "SDL_MapGPUTransferBuffer failed: {0}", SDL_GetError());
            SDL_memcpy(mapped, data, totalSize);
            SDL_UnmapGPUTransferBuffer(device, transferBuffer);

            bool ownCmdBuffer = existingCommandBuffer == nullptr;
            SDL_GPUCommandBuffer* commandBuffer = existingCommandBuffer ? existingCommandBuffer : SDL_AcquireGPUCommandBuffer(device);
            LOG_CORE_ASSERT(commandBuffer, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

            SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

            SDL_GPUTextureTransferInfo textureTransferInfo {};
            textureTransferInfo.transfer_buffer = transferBuffer;
            textureTransferInfo.offset = 0;
            textureTransferInfo.pixels_per_row = width;
            textureTransferInfo.rows_per_layer = height;

            SDL_GPUTextureRegion textureRegion {};
            textureRegion.texture = texture;
            textureRegion.mip_level = 0;
            textureRegion.layer = 0;
            textureRegion.x = 0;
            textureRegion.y = 0;
            textureRegion.z = 0;
            textureRegion.w = width;
            textureRegion.h = height;
            textureRegion.d = 1;

            SDL_UploadToGPUTexture( 
                copyPass,
                &textureTransferInfo, 
                &textureRegion,
                cycle
            );

            SDL_EndGPUCopyPass(copyPass);

            if (ownCmdBuffer)
            {
                SDL_SubmitGPUCommandBuffer(commandBuffer);
            }

            SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
        }
    }

    SDLGPUTexture::SDLGPUTexture(const TextureSpecification& specification)
        : m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
    {
        if (m_Specification.Format == ImageFormat::RGB8)
        {
            // SDL_GPU has clean R8G8B8A8 support across backends, so keep the client-facing spec
            // but upload expanded bytes internally.
            m_Specification.Format = ImageFormat::RGBA8;
        }

        CreateGPUObjects();

        uint32_t bytesPerPixel = m_Specification.Format == ImageFormat::RGBA32F ? 16u : 4u;
        m_Shadow.resize(m_Width * m_Height * bytesPerPixel, 0);
        m_Dirty = true;
    }

    SDLGPUTexture::SDLGPUTexture(const std::filesystem::path& path)
        : m_Path(path)
    {
        LoadFromFile();
        CreateGPUObjects();
        m_Dirty = true;
    }

    SDLGPUTexture::~SDLGPUTexture()
    {
        auto* device = SDLGPURenderAPI::Get() ? SDLGPURenderAPI::Get()->GetDevice() : nullptr;
        if (!device)
        {
            return;
        }

        if (m_Sampler)
        {
            SDL_ReleaseGPUSampler(device, m_Sampler);
            m_Sampler = nullptr;
        }

        if (m_Texture)
        {
            SDL_ReleaseGPUTexture(device, m_Texture);
            m_Texture = nullptr;
        }
    }

    uintptr_t SDLGPUTexture::GetRendererID() const
    {
        return reinterpret_cast<uintptr_t>(m_Texture);
    }

    std::string SDLGPUTexture::GetName() const
    {
        return m_Path.empty() ? std::string() : m_Path.filename().string();
    }

    std::string SDLGPUTexture::GetFormat() const
    {
        return m_Path.empty() ? std::string() : m_Path.extension().string();
    }

    void SDLGPUTexture::SetData(void* data, uint32_t size)
    {
        LOG_CORE_ASSERT(size <= m_Shadow.size(), "Texture::SetData overflow");
        SDL_memcpy(m_Shadow.data(), data, size);
        m_Dirty = true;
    }

    void SDLGPUTexture::Bind(uint32_t slot) const
    {
        LOG_CORE_ASSERT(slot < s_BoundTextures.size(), "Texture slot out of bounds");
        s_BoundTextures[slot] = this;
        s_RawBoundTextures[slot] = m_Texture;
        s_RawSamplers[slot] = m_Sampler;
    }

    void SDLGPUTexture::BindSingleTexture(uintptr_t data) const
    {
        s_BoundTextures[0] = nullptr;
        s_RawBoundTextures[0] = reinterpret_cast<SDL_GPUTexture*>(data);
        s_RawSamplers[0] = m_Sampler;
    }

    void SDLGPUTexture::EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle)
    {
        if (!m_Dirty || !m_Texture || m_Shadow.empty())
        {
            return;
        }

        const uint32_t bytesPerPixel = m_Specification.Format == ImageFormat::RGBA32F ? 16u : 4u;
        UploadTextureBytes(m_Texture, m_Shadow.data(), m_Width, m_Height, bytesPerPixel, commandBuffer, cycle);
        m_Dirty = false;
    }

    const SDLGPUTexture* SDLGPUTexture::GetBound(uint32_t slot)
    {
        return s_BoundTextures[slot];
    }

    SDL_GPUTexture* SDLGPUTexture::GetRawBound(uint32_t slot)
    {
        return s_RawBoundTextures[slot];
    }

    SDL_GPUSampler* SDLGPUTexture::GetRawSampler(uint32_t slot)
    {
        return s_RawSamplers[slot];
    }

    void SDLGPUTexture::CreateGPUObjects()
    {
        SDL_GPUDevice* device = GetDevice();

        SDL_GPUTextureCreateInfo textureInfo {};
        textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
        textureInfo.format = ToSDLFormat(m_Specification.Format);
        textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER | (m_Specification.GenerateMips ? SDL_GPU_TEXTUREUSAGE_COLOR_TARGET : 0);
        textureInfo.width = m_Width;
        textureInfo.height = m_Height;
        textureInfo.layer_count_or_depth = 1;
        textureInfo.num_levels = 1;
        textureInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;

        m_Texture = SDL_CreateGPUTexture( device, &textureInfo );

        LOG_CORE_ASSERT(m_Texture, "SDL_CreateGPUTexture failed: {0}", SDL_GetError());

        SDL_GPUSamplerCreateInfo samplerInfo {};
        samplerInfo.min_filter = ToSDLFilter(m_Specification.Filter);
        samplerInfo.mag_filter = ToSDLFilter(m_Specification.Filter);
        samplerInfo.mipmap_mode = m_Specification.Filter == ImageFilter::LINEAR ? SDL_GPU_SAMPLERMIPMAPMODE_LINEAR : SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
        samplerInfo.address_mode_u = ToSDLAddressMode(m_Specification.Pattern);
        samplerInfo.address_mode_v = ToSDLAddressMode(m_Specification.Pattern);
        samplerInfo.address_mode_w = ToSDLAddressMode(m_Specification.Pattern);
        samplerInfo.min_lod = 0.0f;
        samplerInfo.max_lod = 0.0f;
        samplerInfo.enable_anisotropy = false;
        samplerInfo.enable_compare = false;

        m_Sampler = SDL_CreateGPUSampler( device, &samplerInfo );
        
        LOG_CORE_ASSERT(m_Sampler, "SDL_CreateGPUSampler failed: {0}", SDL_GetError());
    }

    void SDLGPUTexture::LoadFromFile()
    {
        LOG_CORE_ASSERT(std::filesystem::exists(m_Path), "Texture path does not exist: {0}", m_Path.string());

        // SDL_GPU uses top-left texture coordinates. Do not flip like the old OpenGL path.
        stbi_set_flip_vertically_on_load(0);

        int width = 0;
        int height = 0;
        int channels = 0;
        stbi_uc* data = stbi_load(m_Path.string().c_str(), &width, &height, &channels, 4);
        LOG_CORE_ASSERT(data, "stbi_load failed for: {0}", m_Path.string());

        m_Width = (uint32_t)width;
        m_Height = (uint32_t)height;
        m_Specification.Width = m_Width;
        m_Specification.Height = m_Height;
        m_Specification.Format = ImageFormat::RGBA8;
        m_Shadow.resize(m_Width * m_Height * 4);
        SDL_memcpy(m_Shadow.data(), data, m_Shadow.size());

        stbi_image_free(data);
    }
}
