#ifndef LAYER_H
#define LAYER_H

#include <string>

#include "Core.h"
#include "Timestep.h"
#include "Events/Event.h"

namespace Cober {

	class CB_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Unique<Timestep>& ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}

#endif