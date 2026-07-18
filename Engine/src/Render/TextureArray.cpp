#include <pch.h>
#include "Render/TextureArray.h"
#include "Render/GraphicsDevice.h"

#include <SDL3/SDL_gpu.h>

namespace Cober {

namespace {

static SDL_GPUDevice* GetDevice()
{
    if (!GraphicsDevice::IsAlive())
        return nullptr;
    return GraphicsDevice::Get().GetDevice();
}

SDL_GPUFilter ToSDLFilter(ImageFilter filter)
{
    switch (filter)
    {
        case ImageFilter::NEAREST: return SDL_GPU_FILTER_NEAREST;
        case ImageFilter::LINEAR:  return SDL_GPU_FILTER_LINEAR;
    }
    return SDL_GPU_FILTER_LINEAR;
}

} // anonymous namespace

// ======================================================================================
// Construction / Destruction
// ======================================================================================

TextureArray::TextureArray(uint32_t maxLayers)
    : m_MaxLayers(maxLayers)
{
}

TextureArray::~TextureArray()
{
    Reset();
}

// ======================================================================================
// Public API
// ======================================================================================

void TextureArray::EnsureWhiteLayer()
{
    if (m_Initialized)
        return;

    m_Width = 1;
    m_Height = 1;
    CreateGPUResources(m_Width, m_Height);
    if (!m_ArrayTexture)
        return;
    InitWhiteLayer();
    // Flush the white layer upload immediately
    FlushUploads(nullptr);
    m_Initialized = true;
}

uint32_t TextureArray::AddTexture(const Texture* texture)
{
    if (!texture || !texture->GetGPUTexture())
        return UINT32_MAX;

    if (m_Count >= m_MaxLayers)
    {
        LOG_CORE_WARNING("TextureArray: full ({0} layers)", m_MaxLayers);
        return UINT32_MAX;
    }

    if (!m_Initialized)
    {
        m_Width = texture->GetWidth();
        m_Height = texture->GetHeight();
        CreateGPUResources(m_Width, m_Height);
        if (!m_ArrayTexture)
            return UINT32_MAX;
        InitWhiteLayer();
        m_Initialized = true;
    }
    else if (texture->GetWidth() != m_Width || texture->GetHeight() != m_Height)
    {
        // If there are already user textures in this batch (m_Count > 1),
        // we cannot safely resize because previous vertices reference old
        // layer indices. Force the caller to flush and start a new batch.
        if (m_Count > 1)
        {
            LOG_CORE_WARNING("TextureArray: dimension mismatch with {0} layers — flushing batch", m_Count);
            return UINT32_MAX;
        }

        // Only the white layer exists (m_Count == 1) — safe to resize.
        Reset();
        m_Width = texture->GetWidth();
        m_Height = texture->GetHeight();
        CreateGPUResources(m_Width, m_Height);
        if (!m_ArrayTexture)
            return UINT32_MAX;
        InitWhiteLayer();
        m_Initialized = true;
    }

    uint32_t layer = m_Count;

    PendingUpload pending;
    pending.layer = layer;
    pending.pixels = texture->GetPixels();
    m_PendingUploads.push_back(std::move(pending));

    m_Count++;
    return layer;
}

void TextureArray::FlushUploads(SDL_GPUCommandBuffer* cmdBuf)
{
    if (m_PendingUploads.empty())
        return;

    uint32_t bpp = 4;
    for (auto& upload : m_PendingUploads)
    {
        Texture::UploadLayer(
            m_ArrayTexture, upload.layer,
            upload.pixels.data(), m_Width, m_Height,
            bpp, cmdBuf
        );
    }
    m_PendingUploads.clear();
}

void TextureArray::BindToSlot(uint32_t slot) const
{
    if (m_ArrayTexture && m_Sampler)
    {
        Texture::BindRaw(slot, m_ArrayTexture, m_Sampler);
    }
}

void TextureArray::ClearBatch()
{
    m_Count = m_Initialized ? 1 : 0;
    // Only clear pending uploads for layers >= m_Count
    // (keep any still-unflushed white layer upload — though normally it's
    //  already flushed in EnsureWhiteLayer).
    for (auto it = m_PendingUploads.begin(); it != m_PendingUploads.end(); )
    {
        if (it->layer >= m_Count)
            it = m_PendingUploads.erase(it);
        else
            ++it;
    }
}

void TextureArray::Reset()
{
    if (m_ArrayTexture)
    {
        auto* device = GetDevice();
        if (device)
            SDL_ReleaseGPUTexture(device, m_ArrayTexture);
        m_ArrayTexture = nullptr;
    }
    if (m_Sampler)
    {
        auto* device = GetDevice();
        if (device)
            SDL_ReleaseGPUSampler(device, m_Sampler);
        m_Sampler = nullptr;
    }

    m_Width = 0;
    m_Height = 0;
    m_Count = 0;
    m_Initialized = false;
    m_PendingUploads.clear();
}

// ======================================================================================
// Internal
// ======================================================================================

void TextureArray::CreateGPUResources(uint32_t width, uint32_t height)
{
    SDL_GPUDevice* device = GetDevice();

    SDL_GPUTextureCreateInfo texInfo{};
    texInfo.type = SDL_GPU_TEXTURETYPE_2D_ARRAY;
    texInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
    texInfo.width = width;
    texInfo.height = height;
    texInfo.layer_count_or_depth = m_MaxLayers;
    texInfo.num_levels = 1;
    texInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;

    m_ArrayTexture = SDL_CreateGPUTexture(device, &texInfo);
    if (!m_ArrayTexture)
    {
        LOG_CORE_ERROR("TextureArray: SDL_CreateGPUTexture failed: {0}", SDL_GetError());
        return;
    }

    SDL_GPUSamplerCreateInfo samplerInfo{};
    samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
    samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
    samplerInfo.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
    samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    samplerInfo.min_lod = 0.0f;
    samplerInfo.max_lod = 0.0f;
    samplerInfo.enable_anisotropy = false;
    samplerInfo.enable_compare = false;

    m_Sampler = SDL_CreateGPUSampler(device, &samplerInfo);
    if (!m_Sampler)
    {
        LOG_CORE_ERROR("TextureArray: SDL_CreateGPUSampler failed: {0}", SDL_GetError());
    }
}

void TextureArray::InitWhiteLayer()
{
    std::vector<uint8_t> white(m_Width * m_Height * 4, 255);
    PendingUpload upload;
    upload.layer = 0;
    upload.pixels = std::move(white);
    m_PendingUploads.push_back(std::move(upload));

    m_Count = 1;
}

} // namespace Cober
