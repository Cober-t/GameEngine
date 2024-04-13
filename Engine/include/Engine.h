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
#include "Core/UUID.h"
#include "Core/Utils.h"


//[++++++++++++++++++++++++++]
//[+++++++++ EVENTS +++++++++]
//[++++++++++++++++++++++++++]
#include "Events/Event.h"
#include "Events/ApplicationEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "Events/KeyCodes.h"


//[++++++++++++++++++++++++++]
//[+++++++++ SCENE ++++++++++]
//[++++++++++++++++++++++++++]
#include "Scene/Components.h"
#include "Scene/ECS.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"


//[++++++++++++++++++++++++++]
//[+++++++++ SYSTEMS ++++++++]
//[++++++++++++++++++++++++++]
#include "Scene/Systems/RenderSystem.h"
#include "Scene/Systems/PhysicsSystem2D.h"
#include "Scene/Systems/ScriptSystem.h"


//[++++++++++++++++++++++++++]
//[+++++++++ RENDER +++++++++]
//[++++++++++++++++++++++++++]
#include "Render/GraphicsContext.h"
#include "Render/RenderAPI.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"
#include "Render/Buffer.h"
#include "Render/VertexArray.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Framebuffer.h"
#include "Render/Camera/Camera.h"
#include "Render/Camera/GameCamera.h"
#include "Render/Camera/EditorCamera.h"


//[++++++++++++++++++++++++++++]
//[+++++++++ Platform +++++++++]
//[++++++++++++++++++++++++++++]
#include <Platforms/OpenGL/OpenGLContext.h>
#include <Platforms/OpenGL/OpenGLRenderAPI.h>
#include <Platforms/OpenGL/OpenGLBuffer.h>
#include <Platforms/OpenGL/OpenGLVertexArray.h>
#include <Platforms/OpenGL/OpenGLTexture.h>
#include <Platforms/OpenGL/OpenGLShader.h>
#include <Platforms/OpenGL/OpenGLFramebuffer.h>


//[++++++++++++++++++++++++++]
//[+++++++++ BOX 2D +++++++++]
//[++++++++++++++++++++++++++]
// #include <b2World.h>
// #include <b2Body.h>
// #include <b2Fixture.h>
// #include <b2PolygonShape.h>
// #include <b2Draw.h>
// class b2World;

#endif