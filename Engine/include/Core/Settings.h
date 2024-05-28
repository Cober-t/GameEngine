#ifndef SETTING_H
#define SETTING_H

#include <stdint.h>
#include <glm/glm.hpp>

namespace Cober {

    struct DebugSettings
    {
        DebugSettings()
        {
            Reset();
        }

        void Reset()
        {
            shapes = true;
            joints = true;
            aabbs = true;
            pair = true;
            center_of_mass = true;
        }

        bool shapes = true;
        bool joints = true;
        bool aabbs = true;
        bool pair = true;
        bool center_of_mass = true;
    };


    namespace DebugColors
    {
        static const glm::vec4 red = glm::vec4(255.0f, 0.0f, 0.0f, 255.0f);
        static const glm::vec4 green = glm::vec4(0.0f, 255.0f, 0.0f, 255.0f);
        static const glm::vec4 cyan = glm::vec4(0.0f, 255.0f, 255.0f, 255.0f);
        static const glm::vec4 blue = glm::vec4(0.0f, 0.0f, 255.0f, 255.0f);
        static const glm::vec4 yellow = glm::vec4(255.0f, 255.0f, 0.0f, 255.0f);
        static const glm::vec4 purple = glm::vec4(255.0f, 0.0f, 255.0f, 255.0f);
        static const glm::vec4 white = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
        static const glm::vec4 black = glm::vec4(0.0f, 0.0f, 0.0f, 255.0f);
    };

    struct PhysicsSettings
    {
        float TimeStep = 1/20.0f;
        float Gravity = -9.8f;
        int32_t VelocityIterations = 8;
        int32_t PositionIterations = 3;
    };

    struct RenderSettings
    {
        RenderSettings()
        {
            Reset();
        }

        ~RenderSettings()
        {
        }

        void Reset()
        {
            DrawCalls = 0;
            QuadCount = 0;
            LineCount = 0;
            CircleCount = 0;
            TextCount = 0;
        }

        uint32_t* GetIndices()
        {
            uint32_t* indices = new uint32_t[MaxIndices];
            uint32_t offset = 0;
			for (uint32_t i = 0; i < MaxIndices; i += 6)
			{
				indices[i + 0] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}
            
            return indices;
        }

        uint32_t MaxPrimitives = 20000;
        uint32_t MaxVertices = MaxPrimitives * 4;
        uint32_t MaxIndices = MaxPrimitives * 6;

        uint32_t MaxLines = 20000;
        uint32_t MaxLineVertices = MaxLines * 2;

        float LineThickness = 1.0f;

        glm::vec4 QuadVertexPositions[4] 
        {
            { -1.0f, -1.0f, 0.0f, 1.0f },
            {  1.0f, -1.0f, 0.0f, 1.0f },
            {  1.0f,  1.0f, 0.0f, 1.0f },
            { -1.0f,  1.0f, 0.0f, 1.0f }
        };

        uint32_t DrawCalls = 0;
        uint32_t QuadCount = 0;
        uint32_t LineCount = 0;
        uint32_t CircleCount = 0;
        uint32_t TextCount = 0;

        uint32_t GetTotalVertexCount() { return QuadCount * 4 + LineCount * 2 + CircleCount * 4 + TextCount * 4; }
        uint32_t GetTotalIndexCount()  { return QuadCount * 6 + CircleCount * 6 + TextCount * 6; }
    };
}

#endif