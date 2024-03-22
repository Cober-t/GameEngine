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
		ImGui::Text("Mouse Coords: \nX: %i\nY: %i", mouseX, mouseY);

        if (hoveredEntity)
            ImGui::Text("Hovered Entity: %s", hoveredEntity.GetName().c_str());
        else
            ImGui::Text("Hovered Entity: %s", "None");

		ImGui::End();
	}

	//void DataPanel::OnGuiRender(Unique<DataManager> dataManager) {
	//
	//}
}