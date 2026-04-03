#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "Core/Core.h"
#include "Render/RenderAPI.h"

namespace Cober {

    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void Destroy() = 0;

		virtual void* GetDevice() = 0;
        virtual void* GetWindowHandle() = 0;
		
		static Unique<GraphicsContext> Create(void* window);
    };
}

#endif