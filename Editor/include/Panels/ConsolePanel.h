#ifndef CONSOLE_PANEL_H
#define CONSOLE_PANEL_H

#include "Core/Core.h"
#include "Events/Event.h"
#include "Theme/EditorResources.h"

#include <string>
#include <ctime>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace Cober {

	enum class ConsoleMessageFlags : int16_t
	{
		None = -1,
		Info = BIT(0),
		Warning = BIT(1),
		Error = BIT(2),

		All = Info | Warning | Error
	};

	struct ConsoleMessage
	{
		std::string message;
		int16_t Flags;

		time_t Time;
	};


	class ConsolePanel
	{
	public:
		ConsolePanel();
		~ConsolePanel();

		void OnImGuiRender();

		static ConsolePanel& Get() { return *s_Instance; }

	private:
		const char* GetMessageType(const ConsoleMessage& message) const;
		const ImVec4& GetMessageColor(const ConsoleMessage& message) const;
		void ToggleConsoleButton(const std::string iconKey, bool& isActive);

	private:
		static ConsolePanel* s_Instance;
		bool m_DebugEngine = false;
		bool m_DebugApp = false;
		bool m_TraceMessages = false;
		bool m_InfoMessages = false;
		bool m_WarningMessages = false;
		bool m_ErrorMessages = false;

		std::map<std::string, Ref<Texture>> m_AssetIconMap;

		int16_t m_MessageFilters = (int16_t)ConsoleMessageFlags::All;
	};

}

#endif