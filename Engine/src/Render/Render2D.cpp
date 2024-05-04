#include <pch.h>

#include "Render/Render2D.h"

#include "Render/Primitives/Line.h"
#include "Render/Primitives/Quad.h"
#include "Render/Primitives/Circle.h"
#include "Render/Primitives/Cube.h"

#include <memory>

namespace Cober {

	struct CameraData
	{
		glm::mat4 Projection;
		glm::mat4 View;
	};
	static CameraData CameraBuffer;
	static Ref<UniformBuffer> CameraUniformBuffer;

	static Render2D::Statistics stats;


	void Render2D::Start() 
	{
		Primitive::Quad::Init();
		Primitive::Line::Init();
		Primitive::Circle::Init();
		// Primitive::Cube::Init()

		CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
	}


	void Render2D::BeginScene(const Ref<Camera>& camera)
	{
		CameraBuffer.View = camera->GetViewMatrix();
		CameraBuffer.Projection = camera->GetProjectionMatrix();
		CameraUniformBuffer->SetData(&CameraBuffer, sizeof(CameraData));

		Render2D::StartBatch();
	}


	void Render2D::Flush()
	{
		if (Primitive::Quad::GetIndexCount())
		{
			Primitive::Quad::Flush();
			stats.DrawCalls++;
		}

		if (Primitive::Line::GetVertexCount())
		{
			Primitive::Line::Flush();
			stats.DrawCalls++;	
		}

		if (Primitive::Circle::GetIndexCount())
		{
			Primitive::Circle::Flush();
			stats.DrawCalls++;	
		}

		// if (Cube::IndexCount)
		// 	Cube::Flush();
	}


	void Render2D::StartBatch() 
	{
		Primitive::Quad::StartBatch();
		Primitive::Line::StartBatch();
		Primitive::Circle::StartBatch();
		// Primitive::Cube::StartBatch();
	}


	void Render2D::NextBatch() 
	{
		Primitive::Quad::NextBatch();
		Primitive::Line::NextBatch();
		Primitive::Circle::NextBatch();
		// Primitive::Cube::NextBatch();
	}


	void Render2D::ResetStats() 
	{
		memset(&stats, 0, sizeof(Statistics));
	}


	Render2D::Statistics Render2D::GetStats() 
	{
		return stats;
	}


	void Render2D::EndScene() 
	{
		Flush();
	}


	void Render2D::Shutdown() 
	{
	}


	void Render2D::DrawQuad(Entity& entity) 
	{
		if (entity.GetComponent<Render2DComponent>().fill)
		{
			Primitive::Quad::Draw(entity);
			stats.QuadCount++;
		}
		else
		{
			Primitive::Quad::DrawRect(entity);
			stats.LineCount += 4;
		}
	}


	void Render2D::DrawLine(Entity& entity) 
	{
		Primitive::Line::Draw(entity);
		stats.LineCount++;
	}


	void Render2D::DrawCircle(Entity& entity) 
	{
		Primitive::Circle::Draw(entity);
		stats.CircleCount++;
	}

	
	// void Render2D::DebugDrawQuad(Entity& entity)
	// {
	// 	if (!entity.HasComponent<BoxCollider2D>())
	// 		return;

	// 	auto& shape = entity.GetComponent<BoxCollider2D>().shape;
	// 	b2Color color(200.0f, 100.0f, 0.0f, 255.0f);

	// 	if (entity.GetComponent<Render2DComponent>().fill)
	// 	{
	// 		// Debug2DPhysics::Get().DrawSolidPolygon(shape.m_vertices, sizeof(shape.m_vertices) / sizeof(shape.m_vertices[0]), color);
	// 	}
	// 	else
	// 	{
	// 		// Debug2DPhysics::Get().DrawPolygon();
	// 	}
	// }


	// void Render2D::DebugDrawLine(Entity& entity)
	// {
	// 	// b2Vec2 p1 = b2Vec2(0.0f, 5.0f);
	// 	// b2Vec2 p2 = b2Vec2(0.0f, 0.0f);
	// 	// b2Color color = b2Color(0.0f, 1.0f, 0.0f);
	// 	// Debug2DPhysics::Get().DrawSegment(p1, p2, color);
	// 	// stats.LineCount++;
	// }


	// void Render2D::DebugDrawCircle(Entity& entity)
	// {
	// 	// glm::vec4 color = entity.GetComponent<Render2DComponent>().color;
	// 	glm::mat4 transform = entity.GetComponent<TransformComponent>().GetTransform();
	// 	glm::vec3 lineVertices[4];

	// 	// for (size_t i = 0; i < 4; i++)
	// 	//     lineVertices[i] = transform * Primitive::Quad.VertexPositions[i];

	// 	// if (entity.GetComponent<Render2DComponent>().fill)
	// 	// {
	// 	// 		Debug2DPhysics::Get().DrawSolidCircle();
	// 	// }
	// 	// else
	// 	// {
	// 	// 		Debug2DPhysics::Get().DrawCircle();
	// 	// }
	// }


	// void Render2D::DebugDrawTransform(Entity& entity)
	// {
	// 	glm::vec3 pos = entity.GetComponent<TransformComponent>().position;
	// 	glm::vec3 rot = entity.GetComponent<TransformComponent>().rotation;

	// 	b2Vec2 position(pos.x, pos.y);
	// 	b2Rot rotation(rot.z);
	// 	b2Transform transform(position, rotation);

	// 	Debug2DPhysics::Get().DrawTransform(transform);
	// 	stats.LineCount +=2;
	// }
}