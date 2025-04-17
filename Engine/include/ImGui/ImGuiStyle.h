#pragma once

// MACROS
//#ifdef DEBUG
//
//#endif // IMGUI_IMPL_


// COLORS
#define IMGUI_FULL_OPACITY		ImVec4(1.000f, 1.000f, 1.000f, 0.000f)
#define IMGUI_WHITE				ImVec4(1.000f, 1.000f, 1.000f, 1.000f)
#define IMGUI_WHITE_OPACITY_1	ImVec4(1.000f, 1.000f, 1.000f, 0.900f)
#define IMGUI_WHITE_OPACITY_2	ImVec4(1.000f, 1.000f, 1.000f, 0.800f)
#define IMGUI_WHITE_OPACITY_3	ImVec4(1.000f, 1.000f, 1.000f, 0.700f)
#define IMGUI_WHITE_OPACITY_4	ImVec4(1.000f, 1.000f, 1.000f, 0.600f)
#define IMGUI_WHITE_OPACITY_5	ImVec4(1.000f, 1.000f, 1.000f, 0.500f)
#define IMGUI_WHITE_OPACITY_6	ImVec4(1.000f, 1.000f, 1.000f, 0.400f)
#define IMGUI_WHITE_OPACITY_7	ImVec4(1.000f, 1.000f, 1.000f, 0.300f)
#define IMGUI_WHITE_OPACITY_8	ImVec4(1.000f, 1.000f, 1.000f, 0.200f)
#define IMGUI_WHITE_OPACITY_9	ImVec4(1.000f, 1.000f, 1.000f, 0.150f)
#define IMGUI_WHITE_OPACITY_10	ImVec4(1.000f, 1.000f, 1.000f, 0.100f)

#define IMGUI_BLACK				ImVec4(0.000f, 0.000f, 0.000f, 1.000f)
#define IMGUI_BLACK_OPACITY_1	ImVec4(0.000f, 0.000f, 0.000f, 0.900f)
#define IMGUI_BLACK_OPACITY_2	ImVec4(0.000f, 0.000f, 0.000f, 0.800f)
#define IMGUI_BLACK_OPACITY_3	ImVec4(0.000f, 0.000f, 0.000f, 0.700f)
#define IMGUI_BLACK_OPACITY_4	ImVec4(0.000f, 0.000f, 0.000f, 0.600f)
#define IMGUI_BLACK_OPACITY_5	ImVec4(0.000f, 0.000f, 0.000f, 0.500f)
#define IMGUI_BLACK_OPACITY_6	ImVec4(0.000f, 0.000f, 0.000f, 0.400f)
#define IMGUI_BLACK_OPACITY_7	ImVec4(0.000f, 0.000f, 0.000f, 0.300f)
#define IMGUI_BLACK_OPACITY_8	ImVec4(0.000f, 0.000f, 0.000f, 0.200f)
#define IMGUI_BLACK_OPACITY_9	ImVec4(0.000f, 0.000f, 0.000f, 0.150f)
#define IMGUI_BLACK_OPACITY_10	ImVec4(0.000f, 0.000f, 0.000f, 0.100f)

#define IMGUI_MID_GRAY_OPACITY_1	ImVec4(0.500f, 0.500f, 0.500f, 0.100f)
#define IMGUI_MID_GRAY_OPACITY_2	ImVec4(0.500f, 0.500f, 0.500f, 0.200f)
#define IMGUI_MID_GRAY_OPACITY_3	ImVec4(0.500f, 0.500f, 0.500f, 0.300f)
#define IMGUI_MID_GRAY_OPACITY_4	ImVec4(0.500f, 0.500f, 0.500f, 0.400f)
#define IMGUI_MID_GRAY_OPACITY_5	ImVec4(0.500f, 0.500f, 0.500f, 0.500f)
#define IMGUI_MID_GRAY_OPACITY_6	ImVec4(0.500f, 0.500f, 0.500f, 0.600f)
#define IMGUI_MID_GRAY_OPACITY_7	ImVec4(0.500f, 0.500f, 0.500f, 0.700f)
#define IMGUI_MID_GRAY_OPACITY_8	ImVec4(0.500f, 0.500f, 0.500f, 0.800f)

#define IMGUI_GRAY_1	ImVec4(0.800f, 0.800f, 0.800f, 1.000f)
#define IMGUI_GRAY_2	ImVec4(0.700f, 0.700f, 0.700f, 1.000f)
#define IMGUI_GRAY_3	ImVec4(0.600f, 0.600f, 0.600f, 1.000f)
#define IMGUI_GRAY_4	ImVec4(0.500f, 0.500f, 0.500f, 1.000f)
#define IMGUI_GRAY_5	ImVec4(0.400f, 0.400f, 0.400f, 1.000f)
#define IMGUI_GRAY_6	ImVec4(0.300f, 0.300f, 0.300f, 1.000f)
#define IMGUI_GRAY_7	ImVec4(0.250f, 0.250f, 0.250f, 1.000f)
#define IMGUI_GRAY_8	ImVec4(0.200f, 0.200f, 0.200f, 1.000f)
#define IMGUI_GRAY_9	ImVec4(0.150f, 0.150f, 0.150f, 1.000f)
#define IMGUI_GRAY_10	ImVec4(0.100f, 0.100f, 0.100f, 1.000f)

#define IMGUI_ORANGE				ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
#define IMGUI_ORANGE_LOW_OPACITY	ImVec4(1.000f, 0.391f, 0.000f, 0.750f);

#define IMGUI_GRAY				ImVec4(0.500f, 0.500f, 0.500f, 1.000f)
#define IMGUI_DARK				ImVec4(0.148f, 0.148f, 0.148f, 1.000f)
#define IMGUI_DARKEST			ImVec4(0.100f, 0.100f, 0.100f, 1.000f)
#define IMGUI_DARK_GRAY			ImVec4(0.280f, 0.280f, 0.280f, 1.000f)

#define COLOR_IMGUI(r, g, b, a) ImVec4(r, g, b, a)


namespace Cober {


	void Style() {

		ImGuiStyle* style = &ImGui::GetStyle();
		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_Text] = IMGUI_WHITE;
		colors[ImGuiCol_TextDisabled] = IMGUI_GRAY;
		colors[ImGuiCol_WindowBg] = IMGUI_DARK;
		colors[ImGuiCol_ChildBg] = IMGUI_DARK_GRAY;
		colors[ImGuiCol_PopupBg] = IMGUI_DARK;
		colors[ImGuiCol_Border] = IMGUI_DARK_GRAY;
		colors[ImGuiCol_BorderShadow] = IMGUI_DARKEST;
		colors[ImGuiCol_FrameBg] = IMGUI_GRAY_9;
		colors[ImGuiCol_FrameBgHovered] = IMGUI_GRAY_8;
		colors[ImGuiCol_FrameBgActive] = IMGUI_DARK_GRAY;
		colors[ImGuiCol_TitleBg] = IMGUI_GRAY_9;
		colors[ImGuiCol_TitleBgActive] = IMGUI_GRAY_9;
		colors[ImGuiCol_TitleBgCollapsed] = IMGUI_GRAY_9;
		colors[ImGuiCol_MenuBarBg] = IMGUI_GRAY_9;
		colors[ImGuiCol_ScrollbarBg] = IMGUI_GRAY_9;
		colors[ImGuiCol_ScrollbarGrab] = IMGUI_GRAY_7;
		colors[ImGuiCol_ScrollbarGrabHovered] = IMGUI_GRAY_6;
		colors[ImGuiCol_ScrollbarGrabActive] = IMGUI_ORANGE;
		colors[ImGuiCol_CheckMark] = IMGUI_WHITE;
		colors[ImGuiCol_SliderGrab] = IMGUI_GRAY_5;
		colors[ImGuiCol_SliderGrabActive] = IMGUI_ORANGE;
		colors[ImGuiCol_Button] = IMGUI_FULL_OPACITY;
		colors[ImGuiCol_ButtonHovered] = IMGUI_MID_GRAY_OPACITY_5;
		colors[ImGuiCol_ButtonActive] = IMGUI_MID_GRAY_OPACITY_4;
		colors[ImGuiCol_Header] = IMGUI_GRAY_6;
		colors[ImGuiCol_HeaderHovered] = IMGUI_GRAY_4;
		colors[ImGuiCol_HeaderActive] = IMGUI_GRAY_4;
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = IMGUI_ORANGE;
		colors[ImGuiCol_SeparatorActive] = IMGUI_ORANGE;
		colors[ImGuiCol_ResizeGrip] = IMGUI_WHITE_OPACITY_7;
		colors[ImGuiCol_ResizeGripHovered] = IMGUI_WHITE_OPACITY_3;
		colors[ImGuiCol_ResizeGripActive] = IMGUI_ORANGE;
		colors[ImGuiCol_Tab] = IMGUI_GRAY_10;
		colors[ImGuiCol_TabHovered] = IMGUI_GRAY_6;
		colors[ImGuiCol_TabActive] = IMGUI_GRAY_8;
		colors[ImGuiCol_TabUnfocused] = IMGUI_GRAY_10;
		colors[ImGuiCol_TabUnfocusedActive] = IMGUI_GRAY_8;
		//colors[ImGuiCol_DockingPreview]		= IMGUI_ORANGE_LOG_OPACITY;
		//colors[ImGuiCol_DockingEmptyBg]		= IMGUI_GRAY_9;
		colors[ImGuiCol_PlotLines] = IMGUI_GRAY_4;
		colors[ImGuiCol_PlotLinesHovered] = IMGUI_ORANGE;
		colors[ImGuiCol_PlotHistogram] = IMGUI_GRAY_3;
		colors[ImGuiCol_PlotHistogramHovered] = IMGUI_ORANGE;
		colors[ImGuiCol_TextSelectedBg] = IMGUI_WHITE_OPACITY_8;
		colors[ImGuiCol_DragDropTarget] = IMGUI_ORANGE;
		colors[ImGuiCol_NavHighlight] = IMGUI_ORANGE;
		colors[ImGuiCol_NavWindowingHighlight] = IMGUI_ORANGE;
		colors[ImGuiCol_NavWindowingDimBg] = IMGUI_BLACK_OPACITY_5;

		colors[ImGuiCol_ModalWindowDimBg] = IMGUI_BLACK_OPACITY_5;

		style->ChildRounding = 4.0f;
		style->FrameBorderSize = 1.0f;
		style->FrameRounding = 2.0f;
		style->GrabMinSize = 7.0f;
		style->PopupRounding = 2.0f;
		style->ScrollbarRounding = 12.0f;
		style->ScrollbarSize = 13.0f;
		style->TabBorderSize = 1.0f;
		style->TabRounding = 0.0f;
		style->WindowRounding = 20.0f;
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.07f, 0.07f, 0.07f, 1.0f };
	}
}