#include <pch.h>
#include "ImGui/ImGuiLayer.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>


namespace Cober {

	//uint32_t _width, _height;
	//std::string _fontsPath = SOLUTION_DIR + (std::string)"assets/fonts/";

	ImGuiLayer::ImGuiLayer(const char* glVersion)
		: glsl_version(glVersion)
	{

	}


	ImGuiLayer::~ImGuiLayer() 
	{

	}


	void ImGuiLayer::OnAttach() 
	{

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		float fontSize = 18.0f;// *2.0f;

		std::string fontPath = (std::string)ASSETS_DIR + "fonts\\opensans\\";
		std::string fontFile = fontPath + "OpenSans-Bold.ttf";
		io.Fonts->AddFontFromFileTTF(fontFile.c_str(), fontSize, NULL, io.Fonts->GetGlyphRangesDefault());
		fontFile = fontPath + "OpenSans-Regular.ttf";
		io.FontDefault = io.Fonts->AddFontFromFileTTF(fontFile.c_str(), fontSize, NULL, io.Fonts->GetGlyphRangesDefault());

		// Setup Dear ImGui style
		//Style();
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		EngineApp& app = EngineApp::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}


	void ImGuiLayer::OnDetach() 
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	void ImGuiLayer::OnEvent(Event& event) 
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}


	void ImGuiLayer::Begin() 
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End() {

		ImGuiIO& io = ImGui::GetIO();
		EngineApp& app = EngineApp::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}