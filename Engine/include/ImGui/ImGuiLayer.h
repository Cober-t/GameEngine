#pragma once

#include "Core/Layer.h"
#include "Core/EngineApp.h"

namespace Cober {

	class ImGuiLayer : public Layer {
	public:

		ImGuiLayer(const char* glVersion = "#version 460");
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		//virtual void OnEvent(SDL_Event& event) override;

		void Begin();
		void End();

	private:
		GLFWwindow* _window;
		const char* glsl_version;
	};
}