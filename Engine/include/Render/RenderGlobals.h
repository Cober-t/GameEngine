#ifndef RENDER_GLOBALS_H
#define RENDER_GLOBALS_H

#include "Core/Window.h"
#include "Render/VertexArray.h"
#include "Render/Shader.h"

// --------------------------------------------------------------------------------------
// RenderGlobals
//
// Static facade that provides the engine-wide rendering API.
// All high-level render operations go through this class.
//
// Responsibilities:
//   - Frame lifecycle (BeginFrame/EndFrame) → delegates to GraphicsDevice
//   - Drawing (DrawIndexed, DrawTriangles, DrawLines) → builds pipelines, binds state
//   - ImGui integration → delegates to GraphicsDevice
//   - Viewport and clear color → delegates to GraphicsDevice
// --------------------------------------------------------------------------------------

struct ImDrawData;

namespace Cober {

class CB_API RenderGlobals
{
public:
    static void Init(void* window, void* context);
    static void Clear();

    static void DrawInternal(const Ref<VertexBuffer>& vertexBuffer);

    static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0);
    static void DrawTriangles(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);
    static void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount);

    static bool BeginFrame();
    static void BeginMainRenderPass();
    static void EndFrame();

    static void ImGuiInit();
    static void ImGuiShutdown();
    static void ImGuiNewFrame();
    static void ImGuiPrepareDrawData(ImDrawData* drawData);
    static void ImGuiRenderDrawData(ImDrawData* drawData);

    static void SetViewport(uint32_t width, uint32_t height);
    static void SetClearColor(glm::vec4 color);
    static void SetClearColor(float r, float g, float b, float a = 255.0f);

    static void SetLineWidth(float width);
};

}

#endif
