#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

struct SDL_Window;
struct SDL_GPUDevice;

namespace Cober {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Destroy() = 0;
		virtual SDL_GPUDevice* GetGPUDevice() = 0;

		static Unique<GraphicsContext> Create(void* window);
	};

}

#endif