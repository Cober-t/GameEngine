#ifndef TEXTURE_ARRAY_H
#define TEXTURE_ARRAY_H

#include "Core/Core.h"
#include "Render/Texture.h"

#include <vector>
#include <cstdint>

struct SDL_GPUTexture;
struct SDL_GPUSampler;
struct SDL_GPUCommandBuffer;

namespace Cober {

// TextureArray manages an SDL_GPU_TEXTURETYPE_2D_ARRAY for batched quad rendering.
//
// All layers share the same dimensions. The first real texture added sets the
// array size; subsequent textures must match or will be rejected.
//
// Layer 0 is always a solid white texture (for color-only quads with TexIndex=0).
// If no real texture has been added, the array is created at 1×1 with just the
// white layer, so BindToSlot() always works.
//
// Usage per frame:
//   1. EnsureWhiteLayer()  (creates the 1×1 array if no real texture exists yet)
//   2. ClearBatch()         (resets CPU state; GPU resources persist)
//   3. AddTexture()         for each unique sprite (returns layer index)
//   4. FlushUploads(cmdBuf) outside any render pass
//   5. BindToSlot(slot)     before the draw call
//
class CB_API TextureArray
{
public:
    explicit TextureArray(uint32_t maxLayers = 32);
    ~TextureArray();

    // Add a texture to the next available layer.
    // Returns the layer index (>=1; layer 0 is the white texture).
    // Returns UINT32_MAX if full, dimensions don't match, or GPU creation fails.
    uint32_t AddTexture(const Texture* texture);

    // Upload all pending pixel data to the GPU. Must be called outside a render pass.
    void FlushUploads(SDL_GPUCommandBuffer* cmdBuf);

    // Bind this array texture + sampler to a fragment sampler slot.
    void BindToSlot(uint32_t slot) const;

    // Clear batch for next frame. Resets CPU state only; GPU resources persist.
    void ClearBatch();

    // Full reset — releases GPU resources. Used at shutdown / destructor.
    void Reset();

    // Ensure the array exists with at least a white layer at index 0.
    // When no real texture has been added yet, creates a 1×1 array.
    void EnsureWhiteLayer();

    bool IsEmpty() const { return m_Count <= 1; }
    uint32_t GetCount() const { return m_Count; }
    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }

private:
    void CreateGPUResources(uint32_t width, uint32_t height);
    void InitWhiteLayer();

    SDL_GPUTexture* m_ArrayTexture = nullptr;
    SDL_GPUSampler* m_Sampler = nullptr;
    uint32_t m_Width = 0;
    uint32_t m_Height = 0;
    uint32_t m_MaxLayers;
    uint32_t m_Count = 0;       // layers used (including white at 0)
    bool m_Initialized = false;

    struct PendingUpload {
        uint32_t layer;
        std::vector<uint8_t> pixels;
    };
    std::vector<PendingUpload> m_PendingUploads;
};

} // namespace Cober

#endif
