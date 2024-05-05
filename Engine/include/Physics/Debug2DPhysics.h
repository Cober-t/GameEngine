#ifndef DEBUG_2D_PHYSICS_H
#define DEBUG_2D_PHYSICS_H

#include <b2_draw.h>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <Core/Settings.h>

namespace Cober {

    class Debug2DPhysics : b2Draw
    {
    public:
        Debug2DPhysics();
        ~Debug2DPhysics();

        inline static Debug2DPhysics& Get() { return *s_Instance; }

        virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
        virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
        virtual void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;
        virtual void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;
        virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
        virtual void DrawTransform(const b2Transform& xf) override;
        virtual void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

        void UpdateFlags();

    public:
        static DebugSettings& Settings() { return *m_DebugSettings; }
        
    private:
        uint32_t m_Flags = 0;
        static Debug2DPhysics* s_Instance;
        static DebugSettings* m_DebugSettings;
    };
}

#endif