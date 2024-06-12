#include "Panels/DataPanel.h"
#include "EditorLayer.h"

#include <imgui/imgui.h>


namespace Cober {

	DataPanel* DataPanel::instance = nullptr;

	DataPanel::DataPanel() 
	{
		instance = this;
	}


	DataPanel::~DataPanel() 
	{
		delete instance;
		instance = nullptr;
	}


	void DataPanel::OnGuiRender(const Ref<Scene>& scene) 
    {
		// Data
		ImGui::Begin("Data");

		ImGui::Text("Frames: %i", EngineApp::Get().GetFrames());
		ImGui::Text("DeltaTime: %.3f", EngineApp::Get().GetTimer()->GetDeltaTimePerSecond());
		ImGui::Text("Update: %i", EngineApp::Get().GetTimer()->GetUpdates());
		ImGui::Text("Draw Calls: %i", Render2D::GetStats().DrawCalls);
		ImGui::Text("Quads: %i", Render2D::GetStats().QuadCount);
		ImGui::Text("Lines: %i", Render2D::GetStats().LineCount);
		ImGui::Text("Circles: %i", Render2D::GetStats().CircleCount);
		ImGui::Text("Text: %i", Render2D::GetStats().TextCount);
		ImGui::Spacing();
		ImGui::Text("Entities: %i", scene->GetEntityCount());
		ImGui::Text("Physics Bodies: %i", Physics2D::GetBodyCount());
		ImGui::Spacing();
		ImGui::Text("Screen size:\nX: %i Y: %i", EngineApp::Get().GetWindow().GetWidth(), EngineApp::Get().GetWindow().GetHeight());
		ImGui::Text("Mouse Coords:\nX: %i Y: %i", mouseX, mouseY);

		std::string name = "None";
        if ((bool)Editor::SelectedEntity())
            name =  Editor::SelectedEntity().GetName().c_str();
			
		ImGui::Text("Hovered Entity: %s", name.c_str());

		ImGui::End();
	}

	//void DataPanel::OnGuiRender(Unique<DataManager> dataManager) {
	//
	//}
}