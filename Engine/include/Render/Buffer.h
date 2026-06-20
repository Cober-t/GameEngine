#ifndef BUFFER_H
#define BUFFER_H

#include "Core/Core.h"
#include "Core/Log.h"

// --------------------------------------------------------------------------------------
// SDL3 GPU Buffer Types
//
// In SDL3 GPU, buffers are GPU-memory allocations used for vertex data, index data,
// or as transfer buffers for uploading/downloading data. The buffer type is declared
// at creation time via SDL_GPUBufferCreateInfo.usage:
//
//   SDL_GPU_BUFFERUSAGE_VERTEX   - for vertex shader input
//   SDL_GPU_BUFFERUSAGE_INDEX    - for indexed drawing
//   SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE - for storage buffers
//
// Uniform data in SDL3 GPU uses a different mechanism: SDL_PushGPUVertexUniformData()
// and SDL_PushGPUFragmentUniformData() push data directly to uniform slots without
// needing a buffer object.
// --------------------------------------------------------------------------------------

struct SDL_GPUBuffer;
struct SDL_GPUCommandBuffer;

namespace Cober {

// ---- ShaderDataType - describes the type of a single vertex attribute ----
enum class ShaderDataType
{
    None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:    return 4;
        case ShaderDataType::Float2:   return 4 * 2;
        case ShaderDataType::Float3:   return 4 * 3;
        case ShaderDataType::Float4:   return 4 * 4;
        case ShaderDataType::Mat3:     return 4 * 3 * 3;
        case ShaderDataType::Mat4:     return 4 * 4 * 4;
        case ShaderDataType::Int:      return 4;
        case ShaderDataType::Int2:     return 4 * 2;
        case ShaderDataType::Int3:     return 4 * 3;
        case ShaderDataType::Int4:     return 4 * 4;
        case ShaderDataType::Bool:     return 1;
        default: LOG_CORE_WARNING("Unknown ShaderDataType!"); break;
    }
    return 0;
}

// ---- BufferElement - describes one attribute in a vertex buffer layout ----
struct BufferElement
{
    std::string name;
    ShaderDataType type;
    uint32_t size;
    size_t offset;
    bool normalized;

    BufferElement() = default;

    BufferElement(ShaderDataType Type, const std::string& Name, bool Normalized = false)
        : name(Name), type(Type), size(ShaderDataTypeSize(type)), offset(0), normalized(Normalized)
    {
    }

    uint32_t GetComponentCount() const
    {
        switch (type)
        {
            case ShaderDataType::Float:   return 1;
            case ShaderDataType::Float2:  return 2;
            case ShaderDataType::Float3:  return 3;
            case ShaderDataType::Float4:  return 4;
            case ShaderDataType::Mat3:    return 3 * 3;
            case ShaderDataType::Mat4:    return 4 * 4;
            case ShaderDataType::Int:     return 1;
            case ShaderDataType::Int2:    return 2;
            case ShaderDataType::Int3:    return 3;
            case ShaderDataType::Int4:    return 4;
            case ShaderDataType::Bool:    return 1;
            default: LOG_CORE_WARNING("Unknown ShaderDataType!"); break;
        }
        return 0;
    }
};

// ---- BufferLayout - describes the full vertex layout (multiple attributes) ----
class BufferLayout
{
public:
    BufferLayout() {}

    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : _elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    uint32_t GetStride() const { return _stride; }
    const std::vector<BufferElement>& GetElements() const { return _elements; }

    std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
    std::vector<BufferElement>::iterator end()   { return _elements.end();  }
    std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
    std::vector<BufferElement>::const_iterator end()   const { return _elements.end();   }

private:
    void CalculateOffsetsAndStride()
    {
        size_t offset = 0;
        _stride = 0;
        for (auto& element : _elements) {
            element.offset = offset;
            offset += element.size;
            _stride += element.size;
        }
    }

private:
    std::vector<BufferElement> _elements;
    uint32_t _stride = 0;
};

// ---- VertexBuffer - a GPU buffer for vertex data ----
// Wraps an SDL_GPUBuffer with VERTEX usage.
// Supports dirty tracking with shadow copy for deferred upload.
class CB_API VertexBuffer
{
public:
    explicit VertexBuffer(uint32_t size);
    VertexBuffer(float* vertices, uint32_t size);
    ~VertexBuffer();

    void SetData(const void* data, uint32_t size);

    void Unbind() const {} // No-op: SDL3 GPU doesn't use bind/unbind for vertex buffers

    const BufferLayout& GetLayout() const { return m_Layout; }
    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }

    SDL_GPUBuffer* GetGPUBuffer() const { return m_Buffer; }
    uint32_t GetSize() const { return m_Size; }

    // Uploads shadow data to GPU if dirty. Call before drawing.
    // The cycle parameter enables safe reuse across frames.
    void EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle = true);

    static Ref<VertexBuffer> Create(uint32_t size);
    static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

private:
    SDL_GPUBuffer* m_Buffer = nullptr;
    uint32_t m_Size = 0;
    BufferLayout m_Layout;
    std::vector<uint8_t> m_Shadow;
    bool m_Dirty = false;
};

// ---- IndexBuffer - a GPU buffer for index data ----
class CB_API IndexBuffer
{
public:
    IndexBuffer(uint32_t* indices, uint32_t count);
    ~IndexBuffer();

    uint32_t GetCount() const { return m_Count; }

    SDL_GPUBuffer* GetGPUBuffer() const { return m_Buffer; }
    void EnsureUploaded(SDL_GPUCommandBuffer* commandBuffer, bool cycle = false);

    static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

private:
    SDL_GPUBuffer* m_Buffer = nullptr;
    uint32_t m_Count = 0;
    std::vector<uint8_t> m_Shadow;
    bool m_Dirty = false;
};

// ---- UniformBuffer - stores uniform data for shaders ----
// In SDL3 GPU, uniforms are pushed directly to command buffer slots rather than
// using buffer objects. This class manages the data and binding for that system.
class CB_API UniformBuffer
{
public:
    UniformBuffer(uint32_t size, uint32_t binding);
    ~UniformBuffer() = default;

    void SetData(const void* data, uint32_t size, uint32_t offset = 0);

    uint32_t GetBinding() const { return m_Binding; }
    const std::vector<uint8_t>& GetBytes() const { return m_Bytes; }

    static const UniformBuffer* GetBound(uint32_t binding);

    static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);

private:
    uint32_t m_Binding = 0;
    std::vector<uint8_t> m_Bytes;
    static std::unordered_map<uint32_t, const UniformBuffer*> s_BoundUniforms;
};

}

#endif
