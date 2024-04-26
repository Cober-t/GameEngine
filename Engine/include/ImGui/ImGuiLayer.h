#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Core/Layer.h"
#include "Core/EngineApp.h"

namespace Cober {

	class ImGuiLayer : public Layer 
	{
	public:

		ImGuiLayer(const char* glVersion = "#version 460");
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void StyleDefault();
		void StyleCustom();

	private:
		bool m_BlockEvents = true;
		const char* glsl_version;
	};
}

#endif