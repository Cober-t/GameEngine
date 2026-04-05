
#ifndef SDLGPU_FRAMEBUFFER_H
#define SDLGPU_FRAMEBUFFER_H

#include "Render/Framebuffer.h"

struct SDL_GPUTexture;
struct SDL_GPUTransferBuffer;
struct SDL_GPUSampler;

namespace Cober {

    class SDLGPUFramebuffer : public Framebuffer
    {
    public:
        SDLGPUFramebuffer(uint32_t width, uint32_t height);
        ~SDLGPUFramebuffer() override;

        void Bind() override;
        void Unbind() override;

        void Invalidate() override;
        void Resize(uint32_t width, uint32_t height) override;
        int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
        void ClearAttachment(uint32_t attachmentIndex, int value) override;

        uintptr_t GetColorAttachmentRenderID(uint32_t index = 0) const override;
        const FramebufferSpecification& GetSpecification() override { return m_Specification; }

        uint32_t GetColorAttachmentCount() const { return (uint32_t)m_ColorAttachments.size(); }
        SDL_GPUTexture* GetColorAttachmentTexture(uint32_t index) const;
        SDL_GPUTexture* GetDepthAttachmentTexture() const { return m_DepthAttachment; }

    private:
        void Release();

    private:
        FramebufferSpecification m_Specification;
        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

        std::vector<SDL_GPUTexture*> m_ColorAttachments;
        std::vector<SDL_GPUSampler*> m_ColorAttachmentSamplers;
        SDL_GPUTexture* m_DepthAttachment = nullptr;
        SDL_GPUTransferBuffer* m_ReadbackBuffer = nullptr;
    };
}

#endif
