
#include <pch.h>
#include "Physics/Debug2DPhysics.h"
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
        // Primitive::Point::Draw();
    }

    
    void Debug2DPhysics::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        s_Instance->DrawSegment(vertices[0], vertices[1], color);
        s_Instance->DrawSegment(vertices[1], vertices[2], color);
        s_Instance->DrawSegment(vertices[2], vertices[3], color);
        s_Instance->DrawSegment(vertices[3], vertices[0], color);
    }
    
    
    void Debug2DPhysics::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        glm::vec4 convertedColor(color.r, color.g, color.b, color.r);

        std::vector<glm::vec3> convertedVertices;

        for (int i = 0; i < vertexCount; i++)
            convertedVertices.push_back(glm::vec3(vertices[i].x, vertices[i].y, 0.003f));

        Primitive::Quad::Draw(convertedVertices, (int)vertexCount, 0.5f * convertedColor);
    }
    
    
    void Debug2DPhysics::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
    {
        glm::vec3 convertedCenter(center.x, center.y, 0.003f);
        glm::vec4 convertedColor(color.a, color.b, color.g, 1.0f);

        // Primitive::Circle::Draw(convertedCenter, convertedColor, radius, 0.1);
    }
    
    
    void Debug2DPhysics::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
    {
        glm::vec3 centerPosition(center.x, center.y, 0.003f);
        glm::vec4 convertedColor(color.a, color.b, color.g, 1.0f);

		glm::vec3 rotation	= { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale		= { 1.0f, 1.0f, 1.0f };

        glm::mat4 rot = glm::toMat4(glm::quat(rotation));

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), centerPosition)
            * rot
            * glm::scale(glm::mat4(1.0f), scale);

        Primitive::Circle::Draw(transform, 0.5f * convertedColor, 1.0f);
    }


    void Debug2DPhysics::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    { 
        glm::vec3 pointA(p1.x, p1.y, 0.005f);
        glm::vec3 pointB(p2.x, p2.y, 0.005f);
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