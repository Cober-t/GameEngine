#include <pch.h>
#include "Core/LayerStack.h"

namespace Cober {

	LayerStack::~LayerStack()
	{
		Clear();
	}

	Layer* LayerStack::PushLayer(Unique<Layer> layer)
	{
		LOG_CORE_ASSERT(layer, "Trying to push a null layer");

		Layer* raw = layer.get();
		raw->OnAttach();

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
		++m_LayerInsertIndex;

		return raw;
	}

	Layer* LayerStack::PushOverlay(Unique<Layer> overlay)
	{
		Layer* raw = overlay.get();
		raw->OnAttach();

		m_Layers.emplace_back(std::move(overlay));
		return raw;
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto endIt = m_Layers.begin() + static_cast<std::ptrdiff_t>(m_LayerInsertIndex);

		auto it = std::find_if(m_Layers.begin(), m_Layers.end(), 
			[layer](const Unique<Layer>& ptr) {
				return ptr.get() == layer;
			});
			
		if (it != endIt)
		{
			(*it)->OnDetach();
			m_Layers.erase(it);
			--m_LayerInsertIndex;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find_if(m_Layers.begin() + static_cast<std::ptrdiff_t>(m_LayerInsertIndex), m_Layers.end(),
			[overlay](const Unique<Layer>& ptr) { return ptr.get() == overlay; });

		if (it != m_Layers.end())
		{
			(*it)->OnDetach();
			m_Layers.erase(it);
		}
	}

	void LayerStack::Clear()
	{
		for (auto& layer : m_Layers)
		{
			layer->OnDetach();
		}

		m_Layers.clear();
		m_LayerInsertIndex = 0;
	}
}