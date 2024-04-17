#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"

#include "Render/RenderGlobals.h"

namespace Cober  {

    class Primitive
    {
    public:
    
        Primitive() = default;
        virtual ~Primitive() = default;
        virtual void Init();
		virtual void Flush();
		virtual void StartBatch();
		virtual void NextBatch();
		virtual void EndBatch();
    };
}


#endif