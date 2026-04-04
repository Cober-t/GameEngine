#ifndef LAYER_STACK_H
#define LAYER_STACK_H

#include <vector>
#include "Core.h"
#include "Layer.h"

namespace Cober {

	class CB_API LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		Layer* PushLayer(Unique<Layer> layer);
		Layer* PushOverlay(Unique<Layer> overlay);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);
		void Clear();

		std::vector<Unique<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<Unique<Layer>>::iterator end() { return m_Layers.end(); }
		std::vector<Unique<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Unique<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Unique<Layer>>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Unique<Layer>>::const_iterator end() const { return m_Layers.end(); }
		std::vector<Unique<Layer>>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Unique<Layer>>::const_reverse_iterator rend() const { return m_Layers.rend(); }

	private:
		std::vector<Unique<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}

#endif