
#include <pch.h>
#include "Render/Debug2DPhysics.h"
#include <Render/Primitives/Line.h>
#include <Render/Primitives/Quad.h>
#include <Render/Primitives/Circle.h>

namespace Cober {

    Debug2DPhysics* Debug2DPhysics::s_Instance = nullptr;
    DebugSettings* Debug2DPhysics::m_DebugSettings = nullptr;

    Debug2DPhysics::Debug2DPhysics()
    {
        s_Instance = this;
        m_DebugSettings = new DebugSettings();
        UpdateFlags();

        LOG_CORE_INFO("Debug 2D Physics Constructor!");
    }


    Debug2DPhysics::~Debug2DPhysics()
    {	
        delete s_Instance;
		s_Instance = nullptr;

        delete m_DebugSettings;
        m_DebugSettings = nullptr;
        LOG_CORE_INFO("Debug 2D Physics Destructor!");
    }


    void Debug2DPhysics::UpdateFlags()
    {
        s_Instance->ClearFlags(m_Flags);
        
        m_Flags = 0;
        m_Flags += m_DebugSettings->shapes * b2Draw::e_shapeBit;
		m_Flags += m_DebugSettings->joints * b2Draw::e_jointBit;
		m_Flags += m_DebugSettings->aabbs * b2Draw::e_aabbBit;
		m_Flags += m_DebugSettings->pair * b2Draw::e_pairBit;
		m_Flags += m_DebugSettings->center_of_mass * b2Draw::e_centerOfMassBit;

        s_Instance->SetFlags(m_Flags);
    }


    void Debug2DPhysics::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
    {
        // std::cout << "Draw Debug RECT" << std::endl;
        // Primitive::Circle::Draw();
    }

    
    void Debug2DPhysics::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        // std::cout << "Draw Debug POLYGON" << std::endl;
        // Primitive::Quad::DrawRect();
    }
    
    
    void Debug2DPhysics::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        // std::cout << "Draw Debug SOLID POLYGON" << std::endl;

        // b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

        // for (int32 i = 1; i < vertexCount - 1; ++i)
        // {
        //     m_triangles->Vertex(vertices[0], fillColor);
        //     m_triangles->Vertex(vertices[i], fillColor);
        //     m_triangles->Vertex(vertices[i + 1], fillColor);
        // }

        // b2Vec2 p1 = vertices[vertexCount - 1];
        // for (int32 i = 0; i < vertexCount; ++i)
        // {
        //     b2Vec2 p2 = vertices[i];
        //     m_lines->Vertex(p1, color);
        //     m_lines->Vertex(p2, color);
        //     p1 = p2;
        // }
    }
    
    
    void Debug2DPhysics::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
    {
        // std::cout << "Draw Debug CIRCLE" << std::endl;
        // Primitive::Circle::Draw();
    }
    
    
    void Debug2DPhysics::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
    {
        // std::cout << "Draw Debug SOLID CIRCLE" << std::endl;
        // Primitive::Circle::Draw();
    }


    void Debug2DPhysics::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    { 
        glm::vec3 pointA(p1.x, p1.y, 0.0f);
        glm::vec3 pointB(p2.x, p2.y, 0.0f);
        // glm::vec4 auxColor(color.a, color.b, color.g, color.r);
        glm::vec4 auxColor(255.0f, 0.0f, 0.0f, 1.0f);

        Primitive::Line::Draw(pointA, pointB, auxColor, -1);
    }


    void Debug2DPhysics::DrawTransform(const b2Transform& xf)
    {
        // std::cout << "Draw Debug TRANSFORM" << std::endl;

        b2Vec2 p = xf.p;
        b2Vec2 px = p + (0.5f * xf.q.GetXAxis());
        b2Vec2 py = p + (0.5f * xf.q.GetYAxis());

        DrawSegment(p, px, b2Color(1.0f, 0.0f, 0.0f));
        DrawSegment(p, py, b2Color(1.0f, 1.0f, 0.0f));
    }


    void Debug2DPhysics::DrawAABB(b2AABB* aabb, const b2Color& color)
    {
        // std::cout << "Draw Debug AABBs" << std::endl;
        
        // b2Vec2 p1 = aabb->lowerBound;
        // b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
        // b2Vec2 p3 = aabb->upperBound;
        // b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);

        // s_Instance->DrawSegment(p1, p2, color);
        // s_Instance->DrawSegment(p2, p3, color);
        // s_Instance->DrawSegment(p3, p4, color);
        // s_Instance->DrawSegment(p4, p1, color);
    }
}