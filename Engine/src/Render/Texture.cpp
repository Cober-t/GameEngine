#include <pch.h>
#include "Render/Texture.h"
#include "Render/GraphicsDevice.h"
#include <SDL3/SDL_gpu.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Cober {

std::unordered_map<std::filesystem::path, Ref<Texture>> Texture::m_TexturesDataHolder;
std::array<const Texture*, 32> Texture::s_BoundTextures{};
std::array<SDL_GPUTexture*, 32> Texture::s_RawBoundTextures{};
std::array<SDL_GPUSampler*, 32> Texture::s_RawSamplers{};

namespace {

static SDL_GPUDevice* GetDevice()
{
    return GraphicsDevice::Get().GetDevice();
}

SDL_GPUTextureFormat ToSDLFormat(ImageFormat format)
{
    switch (format)
    {
        case ImageFormat::R8:      return SDL_GPU_TEXTUREFORMAT_R8_UNORM;
        case ImageFormat::RGB8:    return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM; // Expand to RGBA
        case ImageFormat::RGBA8:   return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        case ImageFormat::RGBA32F: return SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT;
        default: break;
    }
    LOG_CORE_ASSERT(false, "Unsupported ImageFormat");
    return SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
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

SDL_GPUSamplerAddressMode ToSDLAddressMode(RepeatPattern pattern)
{
    switch (pattern)
    {
        case RepeatPattern::REPEAT:          return SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
        case RepeatPattern::CLAM_TO_EDGE:    return SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        case RepeatPattern::MIRRORED_REPEAT: return SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
    }
    return SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
}

// Upload texture pixel data to GPU via a transfer buffer
void UploadTextureBytes(SDL_GPUTexture* texture, const void* data,
                        uint32_t width, uint32_t height, uint32_t bytesPerPixel,
                        SDL_GPUCommandBuffer* existingCommandBuffer, bool cycle)
{
    SDL_GPUDevice* device = GetDevice();
    uint32_t totalSize = width * height * bytesPerPixel;

    SDL_GPUTransferBufferCreateInfo buffInfo{};
    buffInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    buffInfo.size = totalSize;
    SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &buffInfo);
    LOG_CORE_ASSERT(transferBuffer, "SDL_CreateGPUTransferBuffer failed: {0}", SDL_GetError());

    void* mapped = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    LOG_CORE_ASSERT(mapped, "SDL_MapGPUTransferBuffer failed: {0}", SDL_GetError());
    SDL_memcpy(mapped, data, totalSize);
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);

    bool ownCmdBuffer = existingCommandBuffer == nullptr;
    SDL_GPUCommandBuffer* commandBuffer = existingCommandBuffer
        ? existingCommandBuffer
        : SDL_AcquireGPUCommandBuffer(device);
    LOG_CORE_ASSERT(commandBuffer, "SDL_AcquireGPUCommandBuffer failed: {0}", SDL_GetError());

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);

    SDL_GPUTextureTransferInfo texTransferInfo{};
    texTransferInfo.transfer_buffer = transferBuffer;
    texTransferInfo.offset = 0;
    texTransferInfo.pixels_per_row = width;
    texTransferInfo.rows_per_layer = height;

    SDL_GPUTextureRegion texRegion{};
    texRegion.texture = texture;
    texRegion.mip_level = 0;
    texRegion.layer = 0;
    texRegion.x = 0;
    texRegion.y = 0;
    texRegion.z = 0;
    texRegion.w = width;
    texRegion.h = height;
    texRegion.d = 1;

    SDL_UploadToGPUTexture(copyPass, &texTransferInfo, &texRegion, cycle);

    SDL_EndGPUCopyPass(copyPass);

    if (ownCmdBuffer)
        SDL_SubmitGPUCommandBuffer(commandBuffer);

    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
}

} // anonymous namespace

// ======================================================================================
// Texture
// ======================================================================================

Texture::Texture(const TextureSpecification& specification)
    : m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
{
    if (m_Specification.Format == ImageFormat::RGB8)
        m_Specification.Format = ImageFormat::RGBA8; // Expand to RGBA for SDL3 portability

    CreateGPUObjects();

    uint32_t bytesPerPixel = m_Specification.Format == ImageFormat::RGBA32F ? 16u : 4u;
    m_Shadow.resize(m_Width * m_Height * bytesPerPixel, 0);
    m_Dirty = true;
}

Texture::Texture(const std::filesystem::path& path)
    : m_Path(path)
{
    LoadFromFile();
    CreateGPUObjects();
    m_Dirty = true;
}

Texture::~Texture()
{
    auto* device = GraphicsDevice::Get().GetDevice();
    if (!device) return;

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

uintptr_t Texture::GetRendererID() const
{
    return reinterpret_cast<uintptr_t>(m_Texture);
}

std::string Texture::GetName() const
{
    return m_Path.empty() ? std::string() : m_Path.filename().string();
}

std::string Texture::GetFormat() const
{
    return m_Path.empty() ? std::string() : m_Path.extension().string();
}

void Texture::SetData(void* data, uint32_t size)
{
    LOG_CORE_ASSERT(size <= m_Shadow.size(), "Texture::SetData overflow");
    SDL_memcpy(m_Shadow.data(), data, size);
    m_Dirty = true;
}

void Texture::Bind(uint32_t slot) const
{
    LOG_CORE_ASSERT(slot < s_BoundTextures.size(), "Texture slot out of bounds");
    s_BoundTextures[slot] = this;
    s_RawBoundTextures[slot] = m_Texture;
    s_RawSamplers[slot] = m_Sampler;
}

void Texture::BindSingleTexture(uintptr_t data) const
{
    s_BoundTextures[0] = nullptr;
    s_RawBoundTextures[0] = reinterpret_cast<SDL_GPUTexture*>(data);
    s_RawSamplers[0] = m_Sampler;
}

void Texture::EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle)
{
    if (!m_Dirty || !m_Texture || m_Shadow.empty())
        return;

    const uint32_t bytesPerPixel = m_Specification.Format == ImageFormat::RGBA32F ? 16u : 4u;
    UploadTextureBytes(m_Texture, m_Shadow.data(), m_Width, m_Height, bytesPerPixel, commandBuffer, cycle);
    m_Dirty = false;
}

const Texture* Texture::GetBound(uint32_t slot)
{
    return s_BoundTextures[slot];
}

SDL_GPUTexture* Texture::GetRawBound(uint32_t slot)
{
    return s_RawBoundTextures[slot];
}

SDL_GPUSampler* Texture::GetRawSampler(uint32_t slot)
{
    return s_RawSamplers[slot];
}

void Texture::CreateGPUObjects()
{
    SDL_GPUDevice* device = GetDevice();

    SDL_GPUTextureCreateInfo texInfo{};
    texInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texInfo.format = ToSDLFormat(m_Specification.Format);
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER
        | (m_Specification.GenerateMips ? SDL_GPU_TEXTUREUSAGE_COLOR_TARGET : 0);
    texInfo.width = m_Width;
    texInfo.height = m_Height;
    texInfo.layer_count_or_depth = 1;
    texInfo.num_levels = 1;
    texInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;

    m_Texture = SDL_CreateGPUTexture(device, &texInfo);
    LOG_CORE_ASSERT(m_Texture, "SDL_CreateGPUTexture failed: {0}", SDL_GetError());

    SDL_GPUSamplerCreateInfo samplerInfo{};
    samplerInfo.min_filter = ToSDLFilter(m_Specification.Filter);
    samplerInfo.mag_filter = ToSDLFilter(m_Specification.Filter);
    samplerInfo.mipmap_mode = m_Specification.Filter == ImageFilter::LINEAR
        ? SDL_GPU_SAMPLERMIPMAPMODE_LINEAR
        : SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    samplerInfo.address_mode_u = ToSDLAddressMode(m_Specification.Pattern);
    samplerInfo.address_mode_v = ToSDLAddressMode(m_Specification.Pattern);
    samplerInfo.address_mode_w = ToSDLAddressMode(m_Specification.Pattern);
    samplerInfo.min_lod = 0.0f;
    samplerInfo.max_lod = 0.0f;
    samplerInfo.enable_anisotropy = false;
    samplerInfo.enable_compare = false;

    m_Sampler = SDL_CreateGPUSampler(device, &samplerInfo);
    LOG_CORE_ASSERT(m_Sampler, "SDL_CreateGPUSampler failed: {0}", SDL_GetError());
}

void Texture::LoadFromFile()
{
    LOG_CORE_ASSERT(std::filesystem::exists(m_Path), "Texture path does not exist: {0}", m_Path.string());

    // SDL3 GPU uses top-left texture coordinates (D3D/Metal convention).
    // No flip needed (unlike OpenGL which uses bottom-left).
    stbi_set_flip_vertically_on_load(0);

    int width = 0, height = 0, channels = 0;
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

Ref<Texture> Texture::Create(const TextureSpecification& specification)
{
    return CreateRef<Texture>(specification);
}

Ref<Texture> Texture::Create(const std::filesystem::path& path)
{
    if (m_TexturesDataHolder.find(path) == m_TexturesDataHolder.end())
        m_TexturesDataHolder[path] = CreateRef<Texture>(path);
    return m_TexturesDataHolder[path];
}

// ======================================================================================
// SubTexture
// ======================================================================================

SubTexture::SubTexture()
{
    m_TexCoords[0] = { 0.0f, 0.0f };
    m_TexCoords[1] = { 1.0f, 0.0f };
    m_TexCoords[2] = { 1.0f, 1.0f };
    m_TexCoords[3] = { 0.0f, 1.0f };
}

SubTexture::SubTexture(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max,
                       const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
    : m_Texture(texture)
{
    m_TexCoords[0] = { min.x, min.y };
    m_TexCoords[1] = { max.x, min.y };
    m_TexCoords[2] = { max.x, max.y };
    m_TexCoords[3] = { min.x, max.y };

    subTextureIndex = coords;
    subTextureCellSize = cellSize;
    subTextureSpriteSize = spriteSize;
}

Ref<SubTexture> SubTexture::UpdateCoords(const Ref<Texture> texture, glm::mat4& vertices,
                                         const glm::vec2& coords, const glm::vec2& cellSize,
                                         const glm::vec2& spriteSize)
{
    float scaleFactor = 100;
    float textWidth = cellSize.x;
    float textHeight = cellSize.y;

    if (spriteSize.x != 1.0f || spriteSize.y != 1.0f)
    {
        textWidth = cellSize.x * spriteSize.x;
        textHeight = cellSize.y * spriteSize.y;
    }

    vertices = {
        { -textWidth / 2 / scaleFactor, -textHeight / 2 / scaleFactor, 0.0f, 1.0f },
        {  textWidth / 2 / scaleFactor, -textHeight / 2 / scaleFactor, 0.0f, 1.0f },
        {  textWidth / 2 / scaleFactor,  textHeight / 2 / scaleFactor, 0.0f, 1.0f },
        { -textWidth / 2 / scaleFactor,  textHeight / 2 / scaleFactor, 0.0f, 1.0f }
    };

    texture->SetTextureVertices(vertices);

    glm::vec2 min = {
        (coords.x * cellSize.x) / texture->GetWidth(),
        (coords.y * cellSize.y) / texture->GetHeight()
    };
    glm::vec2 max = {
        ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(),
        ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight()
    };

    return CreateRef<SubTexture>(texture, min, max, coords, cellSize, spriteSize);
}

void SubTexture::ChangeIndices(Ref<SubTexture>& subTexture, glm::vec2 newIndices)
{
    (void)subTexture;
    (void)newIndices;
}

} // namespace Cober
