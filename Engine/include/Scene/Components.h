#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <b2PolygonShape.h>

#include <sol/sol.hpp>

#include "Core/Core.h"
#include "Render/Texture.h"

namespace Cober {
	
	// ++++++++++++++++++++++++++++++++++++++++++++++++
	// UUID's for entities
	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

	// ++++++++++++++++++++++++++++++++++++++++++++++++
	// ID's for componentes
	struct IComponent {
	protected:
		static int nextIndex;
	};

	// Used to assign an index(ID) to a component type
	template <typename T>
	class Component : public IComponent {
	public:
		static int GetComponentID() {
			static int index = nextIndex++;
			return index;
		}
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


	// enum class BodyType { Static = 0, Kinematic, Dynamic };

	// struct Rigidbody2D 
	// {
	// 	glm::vec2 velocity;
	// 	bool fixedRotation = false;

	// 	BodyType type = BodyType::Static;

	// 	void* runtimeBody;

	// 	Rigidbody2D() = default;
	// 	Rigidbody2D(const Rigidbody2D&) = default;
	// 	Rigidbody2D(glm::vec2 vel, bool fxRotation, int bodyType = 1)
	// 		: velocity(vel), fixedRotation(fxRotation), type((BodyType)bodyType), runtimeBody(nullptr) {}
	// };

	// struct Collider2D {

	// };

	// struct BoxCollider2D : public Collider2D 
	// {
	// 	glm::vec2 offset = { 0.0f, 0.0f };
	// 	glm::vec2 size =   { 1.0f, 1.0f };

	// 	b2Shape* body;
	// 	b2PolygonShape shape;

	// 	// Make a physics material maybe
	// 	float density = 1.0f;
	// 	float friction = 0.5f;
	// 	float restitution = 0.0f;
	// 	float restitutionThreshold = 0.5f;

	// 	// Storage for runtime
	// 	void* runtimeFixture = nullptr;

	// 	BoxCollider2D() = default;
	// 	BoxCollider2D(const BoxCollider2D&) = default;
	// 	BoxCollider2D(glm::vec2 Offset, glm::vec2 Size, float Density, float Friction, float Rest, float RestThreshold)
	// 		: offset(Offset), size(Size), 
	// 		  density(Density), friction(Friction), restitution(Rest), restitutionThreshold(RestThreshold) {}
	// };

	/*
	struct Sprite {
		int w, h;
		std::string assetID;
		glm::vec4 color{ 1.0f };
		Ref<Texture> texture = nullptr;

		Sprite() = default;
		Sprite(const Sprite&) = default;
		Sprite(const std::string& ID, int width, int height, glm::vec4 tintColor = glm::vec4(1.0f))
			: assetID(ID), w(width), h(height), color(tintColor) {}
	};

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

	struct CircleCollider2D : public Collider2D {

	};

	struct LineCollider2D : public Collider2D {

	};

	struct CapsuleCollider2D : public Collider2D {

	};
	*/

    /*
	struct Script {

		//std::vector<sol::function> scripts;
		sol::function scripts = sol::lua_nil;

		//void AddScript(sol::function func) { scripts.push_back(func); }

		Script() = default;
		Script(const Script&) = default;
		//Script(std::vector<sol::function> funcs) : scripts(funcs) {}
		Script(sol::function funcs = sol::lua_nil) : scripts(funcs) {}
	};
    */

   	template<typename... Component>
	struct ComponentGroup
	{
	};

	using AllComponents = ComponentGroup<TransformComponent>;
}

#endif
