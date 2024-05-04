
#include <pch.h>
#include "Render/Debug2DPhysics.h"
#include <Render/Primitives/Line.h>
#include <Render/Primitives/Quad.h>
#include <Render/Primitives/Circle.h>

namespace Cober {

    // Debug2DPhysics* s_Instance = nullptr;
    Debug2DPhysics::DebugSettings* Debug2DPhysics::m_DebugSettings = nullptr;


    Debug2DPhysics::Debug2DPhysics()
    {
        // s_Instance = new b2Draw();
        m_DebugSettings = new Debug2DPhysics::DebugSettings();
        UpdateVisibilitySettings();

        LOG_CORE_INFO("Debug 2D Physics Constructor!");
    }


    Debug2DPhysics::~Debug2DPhysics()
    {	
        // delete s_Instance;
		// s_Instance = nullptr;

        delete m_DebugSettings;
        m_DebugSettings = nullptr;
        LOG_CORE_INFO("Debug 2D Physics Destructor!");
    }


    void Debug2DPhysics::UpdateVisibilitySettings()
    {
        m_Flags += m_DebugSettings->SHAPES * b2Draw::e_shapeBit;
		m_Flags += m_DebugSettings->JOINTS * b2Draw::e_jointBit;
		m_Flags += m_DebugSettings->AABBS * b2Draw::e_aabbBit;
		m_Flags += m_DebugSettings->PAIR * b2Draw::e_pairBit;
		m_Flags += m_DebugSettings->CENTER_OF_MASS * b2Draw::e_centerOfMassBit;

        // s_Instance->SetFlags(m_Flags);
    }


    // void Debug2DPhysics::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
    // {
    //     // Primitive::Circle::Draw();
    // }

    
    // void Debug2DPhysics::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    // {
    //     // Primitive::Quad::DrawRect();
    // }
    
    
    // void Debug2DPhysics::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    // {
    //     // Primitive::Quad::Draw();
    //     // static_cast<b2Draw*>(s_Instance)->DrawSolidPolygon(vertices, vertexCount, color);

    //     // TEST
    //     //set up vertex array
    //     GLfloat glverts[16]; //allow for polygons up to 8 vertices
    //     glVertexPointer(2, GL_FLOAT, 0, glverts); //tell OpenGL where to find vertices
    //     glEnableClientState(GL_VERTEX_ARRAY); //use vertices in subsequent calls to glDrawArrays
        
    //     //fill in vertex positions as directed by Box2D
    //     for (int i = 0; i < vertexCount; i++) {
    //     glverts[i*2]   = vertices[i].x;
    //     glverts[i*2+1] = vertices[i].y;
    //     }
        
    //     //draw solid area
    //     glColor4f( color.r, color.g, color.b, 1);
    //     glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
    
    //     //draw lines
    //     glLineWidth(3); //fat lines
    //     glColor4f( 1, 0, 1, 1 ); //purple
    //     glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
    // }
    
    
    // void Debug2DPhysics::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
    // {
    //     // Primitive::Circle::Draw();
    // }
    
    
    // void Debug2DPhysics::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
    // {
    //     // Primitive::Circle::Draw();
    // }


    // void Debug2DPhysics::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
    // { 
    //     glm::vec3 pointA(p1.x, p1.y, 0.0f);
    //     glm::vec3 pointB(p2.x, p2.y, 0.0f);
    //     // glm::vec4 auxColor(color.a, color.b, color.g, color.r);
    //     glm::vec4 auxColor(255.0f, 0.0f, 0.0f, 1.0f);
    //     Primitive::Line::Draw(pointA, pointB, auxColor, -1);
    // }


    // void Debug2DPhysics::DrawTransform(const b2Transform& xf)
    // {
    //     b2Vec2 p = xf.p;
    //     b2Vec2 px = p + (0.5f * xf.q.GetXAxis());
    //     b2Vec2 py = p + (0.5f * xf.q.GetYAxis());

    //     DrawSegment(p, px, b2Color(1.0f, 0.0f, 0.0f));
    //     DrawSegment(p, py, b2Color(1.0f, 1.0f, 0.0f));
    // }
}