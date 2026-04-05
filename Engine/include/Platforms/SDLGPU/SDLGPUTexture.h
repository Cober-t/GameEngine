
#ifndef SDLGPU_TEXTURE_H
#define SDLGPU_TEXTURE_H

#include "Render/Texture.h"

struct SDL_GPUTexture;
struct SDL_GPUSampler;
struct SDL_GPUCommandBuffer;

namespace Cober {

    class SDLGPUTexture : public Texture
    {
    public:
        explicit SDLGPUTexture(const TextureSpecification& specification);
        explicit SDLGPUTexture(const std::filesystem::path& path);
        ~SDLGPUTexture() override;

        uintptr_t GetRendererID() const override;
        const TextureSpecification& GetSpecification() const override { return m_Specification; }

        uint32_t GetWidth() const override { return m_Width; }
        uint32_t GetHeight() const override { return m_Height; }
        std::string GetName() const override;
        std::string GetFormat() const override;
        const std::filesystem::path& GetPath() const override { return m_Path; }
        glm::mat4 GetTextureVertices() override { return m_TextureVertices; }
        void SetTextureVertices(const glm::mat4 vertices) override { m_TextureVertices = vertices; }

        void SetData(void* data, uint32_t size) override;
        void Bind(uint32_t slot = 0) const override;
        void BindSingleTexture(uintptr_t data) const override;

        bool operator==(const Texture& other) const override
        {
            return GetRendererID() == other.GetRendererID();
        }

        SDL_GPUTexture* GetGPUTexture() const { return m_Texture; }
        SDL_GPUSampler* GetSampler() const { return m_Sampler; }
        void EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle = true);

        static const SDLGPUTexture* GetBound(uint32_t slot);
        static SDL_GPUTexture* GetRawBound(uint32_t slot);
        static SDL_GPUSampler* GetRawSampler(uint32_t slot);

    private:
        void CreateGPUObjects();
        void LoadFromFile();

    private:
        TextureSpecification m_Specification;
        std::filesystem::path m_Path;
        uint32_t m_Width = 1, m_Height = 1;

        SDL_GPUTexture* m_Texture = nullptr;
        SDL_GPUSampler* m_Sampler = nullptr;

        std::vector<uint8_t> m_Shadow;
        bool m_Dirty = false;

        glm::mat4 m_TextureVertices{1.0f};

        static std::array<const SDLGPUTexture*, 32> s_BoundTextures;
        static std::array<SDL_GPUTexture*, 32> s_RawBoundTextures;
        static std::array<SDL_GPUSampler*, 32> s_RawSamplers;
    };
}

#endif
