#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Core/Layer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace Cober {

	class CB_API ImGuiLayer : public Layer 
	{
	public:

		ImGuiLayer(const char* glVersion = "#version 460");
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event) override;

		void Begin();
		void End();
		
		bool IsInputEnabled();
		void SetInputEnabled(bool enabled);
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void StyleDefault();
		void StyleCustom();

		static ImGuiContext* GetContext() { return m_ImGuiContext; }

	private:
		bool m_BlockEvents = false;
		const char* glsl_version;

		static ImGuiContext* m_ImGuiContext;
	};
}

#endif