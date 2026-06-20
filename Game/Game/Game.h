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
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<Shader> m_Shader;

    struct PositionColorVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };
};

#endif
