#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <filesystem>
#include <cstdint>

#include "Core/Core.h"

// --------------------------------------------------------------------------------------
// SDL3 GPU Texture System
//
// SDL_GPUTexture represents a GPU image that can be used as a:
//   - Sampled texture (read in shaders via SDL_BindGPUFragmentSamplers)
//   - Color target (rendered into via a render pass)
//   - Depth/stencil target
//
// Texture data is uploaded to the GPU using transfer buffers:
//   1. Create a SDL_GPUTransferBuffer, map it, write pixel data
//   2. Begin a copy pass on a command buffer
//   3. Call SDL_UploadToGPUTexture to copy from transfer buffer to texture
//   4. End the copy pass and submit the command buffer
// --------------------------------------------------------------------------------------

#include <glm/glm.hpp>

struct SDL_GPUTexture;
struct SDL_GPUSampler;
struct SDL_GPUCommandBuffer;

namespace Cober {

enum class ImageFormat
{
    None = 0,
    R8,
    RGB8,
    RGBA8,
    RGBA32F
};

enum class ImageFilter
{
    NEAREST = 0,
    LINEAR,
};

enum class RepeatPattern
{
    REPEAT = 0,
    CLAM_TO_EDGE,
    MIRRORED_REPEAT,
};

struct TextureSpecification
{
    uint32_t Width = 1;
    uint32_t Height = 1;
    ImageFormat Format = ImageFormat::RGBA8;
    ImageFilter Filter = ImageFilter::LINEAR;
    RepeatPattern Pattern = RepeatPattern::REPEAT;
    bool GenerateMips = true;
};

// Texture wraps an SDL_GPUTexture + SDL_GPUSampler pair.
// Supports loading from file (stb_image) or creating from specification.
class CB_API Texture
{
public:
    explicit Texture(const TextureSpecification& specification);
    explicit Texture(const std::filesystem::path& path);
    ~Texture();

    // Returns the raw pointer as an integer ID (for ImGui texture handles)
    uintptr_t GetRendererID() const;
    const TextureSpecification& GetSpecification() const { return m_Specification; }

    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }
    std::string GetName() const;
    std::string GetFormat() const;
    const std::filesystem::path& GetPath() const { return m_Path; }
    glm::mat4 GetTextureVertices() { return m_TextureVertices; }
    void SetTextureVertices(const glm::mat4 vertices) { m_TextureVertices = vertices; }

    static int GetTexturesLoadedCount() { return (int)m_TexturesDataHolder.size(); }

    // Set pixel data (copies to shadow, uploaded on next EnsureUploaded)
    void SetData(void* data, uint32_t size);

    // Bind to a texture slot for fragment shader sampling
    void Bind(uint32_t slot = 0) const;
    void BindSingleTexture(uintptr_t data) const;

    bool operator==(const Texture& other) const
    {
        return GetRendererID() == other.GetRendererID();
    }

    // SDL3 GPU accessors
    SDL_GPUTexture* GetGPUTexture() const { return m_Texture; }
    SDL_GPUSampler* GetSampler() const { return m_Sampler; }
    void EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle = true);

    static Ref<Texture> Create(const TextureSpecification& specification);
    static Ref<Texture> Create(const std::filesystem::path& path);

    // Slot state tracking (used during draw calls to bind textures)
    static const Texture* GetBound(uint32_t slot);
    static SDL_GPUTexture* GetRawBound(uint32_t slot);
    static SDL_GPUSampler* GetRawSampler(uint32_t slot);

private:
    void CreateGPUObjects();
    void LoadFromFile();

    TextureSpecification m_Specification;
    std::filesystem::path m_Path;
    uint32_t m_Width = 1, m_Height = 1;

    SDL_GPUTexture* m_Texture = nullptr;
    SDL_GPUSampler* m_Sampler = nullptr;

    std::vector<uint8_t> m_Shadow;
    bool m_Dirty = false;

    glm::mat4 m_TextureVertices{ 1.0f };

    static std::unordered_map<std::filesystem::path, Ref<Texture>> m_TexturesDataHolder;
    static std::array<const Texture*, 32> s_BoundTextures;
    static std::array<SDL_GPUTexture*, 32> s_RawBoundTextures;
    static std::array<SDL_GPUSampler*, 32> s_RawSamplers;
};

// SubTexture represents a region within a texture atlas
class CB_API SubTexture
{
public:
    SubTexture();
    SubTexture(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max,
               const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize);

    const Ref<Texture> GetTexture() const { return m_Texture; }
    const glm::vec2* GetTexCoords() const { return m_TexCoords; }

    static Ref<SubTexture> UpdateCoords(const Ref<Texture> texture, glm::mat4& vertices,
                                         const glm::vec2& coords, const glm::vec2& cellSize,
                                         const glm::vec2& spriteSize = { 1, 1 });
    static void ChangeIndices(Ref<SubTexture>& subTexture, glm::vec2 newIndices);

private:
    glm::vec2 subTextureIndex = { 0, 0 };
    glm::vec2 subTextureCellSize = { 16, 16 };
    glm::vec2 subTextureSpriteSize = { 1, 1 };

    Ref<Texture> m_Texture;
    glm::vec2 m_TexCoords[4];
};

}

#endif
