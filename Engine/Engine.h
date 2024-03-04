#ifndef ENGINE_H
#define ENGINE_H


//[++++++++++++++++++++++++++]
//[++++++++++ CORE ++++++++++]
//[++++++++++++++++++++++++++]
#include "Core/PlatformDetection.h"
#include "Core/Core.h"
#include "Core/EngineApp.h"
#include "Core/Log.h"
#include "Core/Timer.h"
#include "Core/Timestep.h"
#include "Core/Input.h"
#include "Core/Layer.h"
#include "Core/Window.h"


//[++++++++++++++++++++++++++]
//[+++++++++ EVENTS +++++++++]
//[++++++++++++++++++++++++++]
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/Codes/KeyCodes.h"
#include "Events/Codes/MouseCodes.h"


//[++++++++++++++++++++++++++]
//[++++++++ SYSTEMS +++++++++]
//[++++++++++++++++++++++++++]


//[++++++++++++++++++++++++++]
//[++++++++ ENTITIES ++++++++]
//[++++++++++++++++++++++++++]


//[++++++++++++++++++++++++++]
//[+++++++++ RENDER +++++++++]
//[++++++++++++++++++++++++++]
#include "Render/GraphicsContext.h"
#include "Render/RenderAPI.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"
#include "Render/Buffer.h"
#include "Render/VertexArray.h"
#include "Render/Shader.h"
#include "Render/Texture.h"
#include "Render/Camera/Camera.h"
#include "Render/Camera/GameCamera.h"


//[++++++++++++++++++++++++++++]
//[+++++++++ Platform +++++++++]
//[++++++++++++++++++++++++++++]
#include <Platforms/OpenGL/OpenGLContext.h>
#include <Platforms/OpenGL/OpenGLRenderAPI.h>
#include <Platforms/OpenGL/OpenGLBuffer.h>
#include <Platforms/OpenGL/OpenGLVertexArray.h>
#include <Platforms/OpenGL/OpenGLTexture.h>
#include <Platforms/OpenGL/OpenGLShader.h>



//[++++++++++++++++++++++++++]
//[+++++++++ IMGUI ++++++++++]
//[++++++++++++++++++++++++++]
//#include <imgui/imgui.h>
//#include <imgui/imconfig.h>
//#include <imgui/backends/imgui_impl_glfw.h>
//#include <imgui/backends/imgui_impl_opengl3.h>
//#include <imgui/imgui_internal.h>
//#include <imgui/imstb_rectpack.h>
//#include <imgui/imstb_textedit.h>
//#include <imgui/imstb_truetype.h>


//[++++++++++++++++++++++++++]
//[+++++++++ BOX 2D +++++++++]
//[++++++++++++++++++++++++++]
// 
//#include <box2D/b2_world.h>
//#include <box2D/b2_body.h>
//#include <box2D/b2_fixture.h>
//#include <box2D/b2_polygon_shape.h>
//#include <box2D/b2_draw.h>
//class b2World;

#endif