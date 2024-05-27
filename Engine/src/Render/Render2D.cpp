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

	RenderSettings* Render2D::m_RenderSettings = nullptr;


	void Render2D::Start()
	{
		m_RenderSettings = new RenderSettings();
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
		Primitive::Quad::Flush();
		Primitive::Line::Flush();
		Primitive::Circle::Flush();
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
		m_RenderSettings->Reset();
		// memset(&m_RenderSettings, 0, sizeof(RenderSettings));
	}


	RenderSettings& Render2D::GetStats() 
	{
		return *m_RenderSettings;
	}


	void Render2D::EndScene() 
	{
		Flush();
	}


	void Render2D::Shutdown() 
	{
		Primitive::Quad::CleanVertexBuffer();
		// Primitive::Line::CleanVertexBuffer();
		// Primitive::Circle::CleanVertexBuffer();
		// Primitive::Cube::CleanVertexBuffer();
	}


	void Render2D::DrawQuad(Entity& entity) 
	{
		if (entity.GetComponent<Render2DComponent>().fill)
		{
			Primitive::Quad::Draw(entity);
		}
		else
		{
			Primitive::Quad::DrawRect(entity);
		}
	}


	void Render2D::DrawLine(Entity& entity) 
	{
		Primitive::Line::Draw(entity);
	}


	void Render2D::DrawCircle(Entity& entity) 
	{
		Primitive::Circle::Draw(entity);
	}


	void Render2D::DrawSprite(Entity& entity) 
	{
		Primitive::Quad::DrawTexture(entity);
	}
}