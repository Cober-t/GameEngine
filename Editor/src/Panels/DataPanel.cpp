#include "Panels/DataPanel.h"
// #include "Panels/SceneHierarchyPanel.h"

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


	void DataPanel::OnGuiRender(Entity& hoveredEntity) 
    {
		// Data
		ImGui::Begin("Data");

		ImGui::Text("Renderer Stats:");
		ImGui::Text("Frames: %d", EngineApp::Get().GetFrames());
		ImGui::Text("Draw Calls: %d", Render2D::GetStats().DrawCalls);
		ImGui::Text("Quads: %d", Render2D::GetStats().QuadCount);
		ImGui::Text("Lines: %d", Render2D::GetStats().LineCount);
		ImGui::Text("Circles: %d", Render2D::GetStats().CircleCount);
		ImGui::Spacing();
		ImGui::Text("Mouse Coords: \nX: %i\nY: %i", mouseX, mouseY);

		std::string name = "None";
        if ((bool)hoveredEntity)
            name =  hoveredEntity.GetName().c_str();
		ImGui::Text("Hovered Entity: %s", name.c_str());

		ImGui::End();
	}

	//void DataPanel::OnGuiRender(Unique<DataManager> dataManager) {
	//
	//}
}