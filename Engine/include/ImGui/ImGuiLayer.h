#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "Core/Layer.h"
#include <SDL3/SDL.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Cober {

    class CB_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;

        void OnSDLEvent(const SDL_Event& event);

        void Begin();
        void End();

        bool IsInputEnabled() const;
        void SetInputEnabled(bool enabled);
        void BlockEvents(bool block) { m_BlockEvents = block; }

        void StyleDefault();
        void StyleCustom();

        static ImGuiContext* GetContext() { return m_ImGuiContext; }

    private:
        bool m_BlockEvents = true;
        bool m_Initialized = false;
        static ImGuiContext* m_ImGuiContext;
    };

}

#endif