#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <b2_edge_shape.h>
#include <b2_circle_shape.h>
#include <b2_polygon_shape.h>

#include <filesystem>

#include <sol/sol.hpp>

#include "Core/Core.h"
#include "Render/Texture.h"
#include "Render/Camera/GameCamera.h"
#include "Render/Text/Font.h"

namespace Cober {
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++
	// UUID's for entities
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};


	// ++++++++++++++++++++
	// Components for entities
	struct TransformComponent
    {
		glm::vec3 position	= { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation	= { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale		= { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetTransform() const
		{
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), position)
				* rot
				* glm::scale(glm::mat4(1.0f), scale);
		}

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(glm::vec3 pos, glm::vec3 rot = glm::vec3(0.0f), glm::vec3 sc = glm::vec3(1.0f))
			: position(pos), rotation(rot), scale(sc) {};
	};


	struct TagComponent 
    {
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;;
		TagComponent(const std::string& tag) : tag(tag) {};
	};


	struct CameraComponent
    {
		float distance	= 5.0f;
		int width = 1280;
		int height = 720;
		float nearClip = 0.01f;
		float farClip = 1000.0f;
		float fov = 45.0f;

		bool perspective = true;
		bool mainCamera = false;
		bool debug = true;
		Ref<Camera> camera = CreateRef<GameCamera>(45.0f, width, height, 0.01f, 1000.0f);

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(float f, int w, int h)
		 : fov(f), width(w), height(h) 
		{
		}

		void UpdateCameraValues()
		{
			camera->GetSettings().distance = distance;
			camera->GetSettings().width = width;
			camera->GetSettings().height = height;
			
			camera->GetSettings().nearClip = nearClip;
			camera->GetSettings().farClip = farClip;
			camera->GetSettings().fov = fov;
			
			camera->GetSettings().perspectiveProjection = perspective;
		}

		void SetMain(bool isMain) { mainCamera = isMain; }
	};


	enum class BodyType { Static = 0, Kinematic, Dynamic };

	struct Rigidbody2D 
	{
		bool fixedRotation = false;

		BodyType type = BodyType::Static;

		void* runtimeBody = nullptr;

		Rigidbody2D() = default;
		Rigidbody2D(const Rigidbody2D&) = default;
	};


	struct Collider2D 
	{
		b2Shape* body;

		float density = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;

		// Storage for runtime
		void* runtimeFixture = nullptr;
	};


	struct BoxCollider2D : public Collider2D 
	{
		glm::vec2 offset = { 0.0f, 0.0f };
		glm::vec2 size =   { 1.0f, 1.0f };

		b2PolygonShape shape;
		bool isSensor = false;

		BoxCollider2D() = default;
		BoxCollider2D(const BoxCollider2D&) = default;
	};
        
        
	struct CircleCollider2D : public Collider2D 
	{
		glm::vec2 offset = { 0.0f, 0.0f };
		float radius = 0.5f;

		b2CircleShape shape;
		bool isSensor = false;

		CircleCollider2D() = default;
		CircleCollider2D(const CircleCollider2D&) = default;
	};


	struct PolygonCollider2D : public Collider2D 
	{
		glm::vec2 offset = { 0.0f, 0.0f };

		b2PolygonShape shape;

		PolygonCollider2D() = default;
		PolygonCollider2D(const PolygonCollider2D&) = default;
	};


	struct EdgeCollider2D : public Collider2D 
	{
		glm::vec2 pointA = { 0.0f, 0.0f };
		glm::vec2 pointB = { 5.0f, 0.0f };

		b2EdgeShape shape;

		EdgeCollider2D() = default;
		EdgeCollider2D(const EdgeCollider2D&) = default;
	};


	enum class Shape2D { Line = 0, Quad, Circle, Sprite, N_SHAPE_ITEMS };

	struct Render2DComponent 
	{
		int w, h;
		std::string assetID;
		glm::vec4 color{ 1.0f };

		Shape2D shapeType = Shape2D::Quad;
		bool fill = true;

		glm::vec3 point1 = glm::vec3(-3.0f, 0.0f, 0.0f);
		glm::vec3 point2 = glm::vec3(3.0f, 0.0f, 0.0f);
		float thickness = 1.0f;
		float fade = 0.005f;

		Ref<Texture> texture = nullptr;
		glm::vec4 vertices[4]
        {
            { -1.0f, -1.0f, 0.0f, 1.0f },
            {  1.0f, -1.0f, 0.0f, 1.0f },
            {  1.0f,  1.0f, 0.0f, 1.0f },
            { -1.0f,  1.0f, 0.0f, 1.0f }
        };
		Ref<SubTexture> subTexture = CreateRef<SubTexture>();
		bool isSubTexture;
		glm::vec2 subTextureIndex = { 0, 0 };
		glm::vec2 subTextureCellSize = { 16, 16 };
		glm::vec2 subTextureSpriteSize = { 1, 1 };

		Render2DComponent() = default;
		Render2DComponent(const Render2DComponent&) = default;
	};


	struct ScriptComponent
	{
		std::vector<sol::function> scripts;
		// sol::function script = sol::lua_nil;

		void AddScript(sol::function func) { scripts.push_back(func); }

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
		ScriptComponent(std::vector<sol::function> funcs) : scripts(funcs) {}
		// ScriptComponent(sol::function funcs = sol::lua_nil) : script(funcs) {}
	};

	class Entity;
	class Scene;
	struct NativeScriptComponent
	{
		std::string className = "None";

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent&) = default;
	};


	struct AudioComponent
	{
		std::filesystem::path audioPath;
		std::string audioName = "None";
		bool loop = false;

		AudioComponent() = default;
		AudioComponent(const AudioComponent&) = default;
	};


	struct TextComponent
	{
		std::string Text;
		Ref<Font> FontAsset = Font::GetDefault();
		glm::vec4 Color{ 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;
	};


	/*
	struct Animation2D {
		int numFrames;
		int currentFrame;
		int frameRateSpeed;
		bool shouldLoop;
		int startTime;

		//Animation() = default;
		Animation2D(const Animation2D&) = default;
		Animation2D(int numfrm = 1, int frmRateSpeed = 1, bool loop = true)
			: numFrames(numfrm), currentFrame(1), frameRateSpeed(frmRateSpeed), shouldLoop(loop), startTime(SDL_GetTicks()) {}
	};
	*/

   	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<TransformComponent, CameraComponent,
		Render2DComponent,
		AudioComponent,
		TextComponent,
		Rigidbody2D, BoxCollider2D, CircleCollider2D, EdgeCollider2D, PolygonCollider2D,
		ScriptComponent, NativeScriptComponent>;
}

#endif
