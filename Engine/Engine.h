//[++++++++++++++++++++++++++]
//[+++++++++ IMGUI ++++++++++]
//[++++++++++++++++++++++++++]
// #if defined __OPENGL__ && !defined __EMSCIPTEN__
 	#include <imgui/imgui.h>
 	#include <imgui/imconfig.h>
	#include <imgui/backends/imgui_impl_glfw.h>
 	#include <imgui/backends/imgui_impl_opengl3.h>
 	#include <imgui/imgui_internal.h>
 	#include <imgui/imstb_rectpack.h>
 	#include <imgui/imstb_textedit.h>
 	#include <imgui/imstb_truetype.h>
// #endif 

#ifndef ENGINE_H
#define ENGINE_H
//[++++++++++++++++++++++++++]
//[++++++++++ CORE ++++++++++]
//[++++++++++++++++++++++++++]
#include "Core/Timestep.h"
#include "Core/EngineApp.h"


//[++++++++++++++++++++++++++]
//[+++++++++ EVENTS +++++++++]
//[++++++++++++++++++++++++++]


//[++++++++++++++++++++++++++]
//[++++++++ SYSTEMS +++++++++]
//[++++++++++++++++++++++++++]


//[++++++++++++++++++++++++++]
//[++++++++ ENTITIES ++++++++]
//[++++++++++++++++++++++++++]


//[++++++++++++++++++++++++++]
//[+++++++++ RENDER +++++++++]
//[++++++++++++++++++++++++++]


//[++++++++++++++++++++++++++]
//[+++++++++ Platform +++++++++]
//[++++++++++++++++++++++++++]
// #if __OPENGLES3__
	//#include <Platforms/OpenGL/OpenGLContext.h>
	//#include <Platforms/OpenGL/OpenGLFramebuffer.h>
	//#include <Platforms/OpenGL/OpenGLRenderAPI.h>
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