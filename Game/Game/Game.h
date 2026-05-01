#ifndef Game_H
#define Game_H

#include <Engine.h>

using namespace Cober;


class Game : public Layer 
{
public:
	Game();
	virtual ~Game() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(const Timestep& ts) override;
	void OnEvent(Event& event) override;
	
private:
	Ref<Scene> m_ActiveScene;
	Ref<Camera> m_DefaultCamera;

	Ref<Framebuffer> m_Fbo;
	glm::vec2 m_MousePosition;

	SDL_GPUGraphicsPipeline* Pipeline = nullptr;
	Ref<VertexBuffer> VertexBuffer = nullptr;

	struct PositionColorVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
	};
	
	Ref<Shader> m_shader; // TEST
};

#endif
