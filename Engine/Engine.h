#ifndef ENGINE_H
#define ENGINE_H

//[++++++++++++++++++++++++++]
//[+++++++++ IMGUI ++++++++++]
//[++++++++++++++++++++++++++]
// #if defined __OPENGL__ && !defined __EMSCIPTEN__
 	//#include <imgui/imgui.h>
 	//#include <imgui/imconfig.h>
	//#include <imgui/backends/imgui_impl_glfw.h>
 	//#include <imgui/backends/imgui_impl_opengl3.h>
 	//#include <imgui/imgui_internal.h>
 	//#include <imgui/imstb_rectpack.h>
 	//#include <imgui/imstb_textedit.h>
 	//#include <imgui/imstb_truetype.h>
// #endif 
//[++++++++++++++++++++++++++]
//[++++++++++ CORE ++++++++++]
//[++++++++++++++++++++++++++]
#include "Core/Core.h"
#include "Core/EngineApp.h"
#include "Core/Log.h"
#include "Core/Timer.h"
#include "Core/Timestep.h"
#include "Core/Input.h"
#include "Core/Layer.h"
#include "Core/Window.h"
#include "Core/PlatformDetection.h"


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


//[++++++++++++++++++++++++++]
//[+++++++++ Platform +++++++++]
//[++++++++++++++++++++++++++]
// #if __OPENGLES3__
	#include <Platforms/OpenGL/OpenGLContext.h>
	#include <Platforms/OpenGL/OpenGLRenderAPI.h>
	//#include <Platforms/OpenGL/OpenGLFramebuffer.h>
	//#include <Platforms/OpenGL/OpenGLShader.h>
// #elif defined __OPENGLES3__
	//#include <Platforms/OpenGL/OpenGLES3Context.h>
	//#include <Platforms/OpenGL/OpenGLES3Framebuffer.h>
	//#include <Platforms/OpenGL/OpenGLES3RenderAPI.h>
	//#include <Platforms/OpenGL/OpenGLES3Shader.h>
// #endif

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