#ifndef DEBUG_2D_PHYSICS_H
#define DEBUG_2D_PHYSICS_H

#include <b2Draw.h>

namespace Cober {

    class Debug2DPhysics : public b2Draw
    {
    public:
        Debug2DPhysics();
        ~Debug2DPhysics();

        inline static Debug2DPhysics& Get() { return *s_Instance; }

        void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override;
        void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
        void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
        void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
        void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
        void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
        void DrawTransform(const b2Transform& xf) override;

        void UpdateVisibilitySettings();
        uint32_t GetFlags() { return m_Flags; }

    public:
        struct DebugSettings
        {
            bool LINES = false;
            bool QUADS = false;
            bool CIRCLES = false;
            bool TRANSFORMS = false;

            bool SHAPES = true;
            bool JOINTS = true;
            bool AABBS = true;
            bool PAIR = true;
            bool CENTER_OF_MASS = true;
        };

    public:
        static DebugSettings& Settings() { return *m_DebugSettings; }
        
    private:
        uint32_t m_Flags = 0;
        static Debug2DPhysics* s_Instance;
        static DebugSettings* m_DebugSettings;
    };
}

#endif