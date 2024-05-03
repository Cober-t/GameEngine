
#include <pch.h>
#include "Render/Debug2DPhysics.h"
#include <Render/Primitives/Line.h>
#include <Render/Primitives/Quad.h>
#include <Render/Primitives/Circle.h>

namespace Cober {

    Debug2DPhysics* Debug2DPhysics::s_Instance = nullptr;
    Debug2DPhysics::DebugSettings* Debug2DPhysics::m_DebugSettings = nullptr;


    Debug2DPhysics::Debug2DPhysics()
    {
        s_Instance = this;
        m_DebugSettings = new Debug2DPhysics::DebugSettings();
        UpdateVisibilitySettings();

        LOG_CORE_INFO("Debug 2D Physics Constructor!");
    }


    Debug2DPhysics::~Debug2DPhysics()
    {	
        delete s_Instance;
		s_Instance = nullptr;
        LOG_CORE_INFO("Debug 2D Physics Destructor!");
    }


    void Debug2DPhysics::UpdateVisibilitySettings()
    {
        m_Flags += m_DebugSettings->SHAPES * b2Draw::e_shapeBit;
		m_Flags += m_DebugSettings->JOINTS * b2Draw::e_jointBit;
		m_Flags += m_DebugSettings->AABBS * b2Draw::e_aabbBit;
		m_Flags += m_DebugSettings->PAIR * b2Draw::e_pairBit;
		m_Flags += m_DebugSettings->CENTER_OF_MASS * b2Draw::e_centerOfMassBit;

        SetFlags(m_Flags);
    }


    void Debug2DPhysics::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
    {
        // Primitive::Circle::Draw();
    }

    
    void Debug2DPhysics::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        // Primitive::Quad::DrawRect();
    }
    
    
    void Debug2DPhysics::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        // Primitive::Quad::Draw();
    }
    
    
    void Debug2DPhysics::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
    {
        // Primitive::Circle::Draw();
    }
    
    
    void Debug2DPhysics::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
    {
        // Primitive::Circle::Draw();
    }


    void Debug2DPhysics::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    { 
        glm::vec3 pointA(p1.x, p1.y, 0.00001f);
        glm::vec3 pointB(p2.x, p2.y, 0.00001f);
        glm::vec4 auxColor(color.a, color.b, color.g, color.r);
        Primitive::Line::Draw(pointA, pointB, auxColor, -1);
    }


    void Debug2DPhysics::DrawTransform(const b2Transform& xf)
    {
        b2Vec2 p = xf.p;
        b2Vec2 px = p + (0.5f * xf.q.GetXAxis());
        b2Vec2 py = p + (0.5f * xf.q.GetYAxis());

        DrawSegment(p, px, b2Color(1.0f, 0.0f, 0.0f));
        DrawSegment(p, py, b2Color(1.0f, 1.0f, 0.0f));
    }
}