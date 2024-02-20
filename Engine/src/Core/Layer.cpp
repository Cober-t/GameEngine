#include "Core/Layer.h"

namespace Cober {

	Layer::Layer(const std::string& debugName) 
		: _debugName(debugName) 
	{

	}

	Layer::~Layer()
	{
		for (Layer* layer : m_Layers) {
            
			layer->OnDetach();
			delete layer;
		}
	}

	void Layer::PushLayer(Layer* layer) 
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void Layer::PushOverlay(Layer* overlay) 
	{
		m_Layers.emplace_back(overlay);
	}

	void Layer::PopLayer(Layer* layer) 
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (it != m_Layers.end()) {

			layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void Layer::PopOverlay(Layer* overlay) 
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);

		if (it != m_Layers.end()) {

			overlay->OnDetach();
			m_Layers.erase(it);
		}
	}

}