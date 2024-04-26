#include "Panels/ConsolePanel.h"
#include "Core/EngineApp.h"
#include "Core/Log.h"

// #include "Core/Events/SceneEvents.h"
// #include "Editor/EditorResources.h"
// #include "Editor/FontAwesome.h"

// #include "imgui/Colors.h"

namespace Cober {

    ConsolePanel* ConsolePanel::s_Instance = nullptr;

	static const ImVec4 s_InfoTint = ImVec4(0.0f, 0.431372549f, 1.0f, 1.0f);
	static const ImVec4 s_WarningTint = ImVec4(1.0f, 0.890196078f, 0.0588235294f, 1.0f);
	static const ImVec4 s_ErrorTint = ImVec4(1.0f, 0.309803922f, 0.309803922f, 1.0f);

	ConsolePanel::ConsolePanel()
	{
		LOG_CORE_ASSERT(s_Instance == nullptr, "Console Panel instance is null");
		s_Instance = this;
	}


	ConsolePanel::~ConsolePanel()
	{
		s_Instance = nullptr;
	}

    
	const char* ConsolePanel::GetMessageType(const ConsoleMessage& message) const
	{
		if (message.Flags & (int16_t)ConsoleMessageFlags::Info) return "Info";
		if (message.Flags & (int16_t)ConsoleMessageFlags::Warning) return "Warning";
		if (message.Flags & (int16_t)ConsoleMessageFlags::Error) return "Error";
		return "Unknown";
	}


	const ImVec4& ConsolePanel::GetMessageColor(const ConsoleMessage& message) const
	{
		//if (message.Flags & (int16_t)ConsoleMessageFlags::Info) return s_InfoButtonOnTint;
		if (message.Flags & (int16_t)ConsoleMessageFlags::Warning) return s_WarningTint;
		if (message.Flags & (int16_t)ConsoleMessageFlags::Error) return s_ErrorTint;
		return s_InfoTint;
	}

    
	void ConsolePanel::OnImGuiRender()
	{
		ImGui::Begin("Console");
        
        for (auto msg : Log::GetMessages())
        {
			ImVec4 color;
			switch (msg.level)
			{
				case Log::LOG_LEVELS::TRACE:
					color = ImVec4(0.95f, 0.95f, 0.95f, 1);	break;
				case Log::LOG_LEVELS::INFO:
					color = ImVec4(0.11f, 0.61f, 0.95f, 1);	break;
				case Log::LOG_LEVELS::WARN:
					color = ImVec4(0.93f, 0.57f, 0.21f, 1);	break;
				case Log::LOG_LEVELS::ERR:
					color = ImVec4(1, 0, 0.24f, 1);	break;
				case Log::LOG_LEVELS::CRITICAL:
					color = ImVec4(1, 0, 1, 1);	break;
			}

			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextWrapped("[");	
			ImGui::SameLine();
			ImGui::TextWrapped(msg.fileName.c_str());
			ImGui::SameLine();
			ImGui::TextWrapped(msg.function.c_str());
			ImGui::SameLine();
			ImGui::TextWrapped("%i",msg.line);
			ImGui::SameLine();
			ImGui::TextWrapped("] ");
			ImGui::SameLine();
			ImGui::TextWrapped(msg.message.c_str());
			// ImGui::SameLine();
			// ImGui::TextWrapped("%i",msg.level);
			ImGui::PopStyleColor();
        }

		ImGui::End();
	}
}