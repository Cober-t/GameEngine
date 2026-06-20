#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "Core/Core.h"

// --------------------------------------------------------------------------------------
// SDL3 GPU Framebuffer (Offscreen Rendering)
//
// A Framebuffer in this engine is a set of GPU textures used as render targets for
// offscreen rendering. It wraps SDL_GPUTexture objects for:
//   - Color attachments (displayable in a viewport via ImGui)
//   - Depth/stencil attachment
//
// The framebuffer is used by the editor to render the scene to a texture, which is
// then displayed in the viewport panel. Entity picking uses the RED_INTEGER attachment
// to read back the entity ID at a given pixel.
// --------------------------------------------------------------------------------------

struct SDL_GPUTexture;
struct SDL_GPUSampler;
struct SDL_GPUTransferBuffer;

namespace Cober {

enum class FramebufferTextureFormat
{
    None = 0,
    RGBA8,
    RED_INTEGER,
    DEPTH24STENCIL8,
    Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
        : TextureFormat(format) {}

    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
        : Attachments(attachments) {}

    std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification
{
    uint32_t Width = 0, Height = 0;
    FramebufferAttachmentSpecification Attachments{};
    uint32_t Samples = 1;
    bool SwapChainTarget = false;
};

// Framebuffer manages offscreen render target textures.
// Bind() starts a render pass into the framebuffer's textures.
// Unbind() ends the render pass.
class CB_API Framebuffer
{
public:
    explicit Framebuffer(uint32_t width, uint32_t height);
    ~Framebuffer();

    // Begins a render pass targeting this framebuffer's attachments
    void Bind();
    // Ends the current render pass
    void Unbind();

    void Invalidate();
    void Resize(uint32_t width, uint32_t height);
    int ReadPixel(uint32_t attachmentIndex, int x, int y);
    void ClearAttachment(uint32_t attachmentIndex, int value);

    uintptr_t GetColorAttachmentRenderID(uint32_t index = 0) const;
    const FramebufferSpecification& GetSpecification() { return m_Specification; }

    // SDL3 GPU accessors
    uint32_t GetColorAttachmentCount() const { return (uint32_t)m_ColorAttachments.size(); }
    SDL_GPUTexture* GetColorAttachmentTexture(uint32_t index) const;
    SDL_GPUTexture* GetDepthAttachmentTexture() const { return m_DepthAttachment; }

    static Ref<Framebuffer> Create(uint32_t width, uint32_t height);

private:
    void Release();

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
