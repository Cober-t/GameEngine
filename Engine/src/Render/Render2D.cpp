#include <pch.h>

#include "Render/Render2D.h"

#include "Render/Primitives/Primitive.h"
#include "Render/Primitives/Line.h"
#include "Render/Primitives/Quad.h"
#include "Render/Primitives/Circle.h"
#include "Render/Primitives/Cube.h"

#include <memory>

namespace Cober {

	struct Primitives 
	{
		Unique<Quad> quad = CreateUnique<Quad>();
		Unique<Line> line = CreateUnique<Line>();
		Unique<Circle> circle = CreateUnique<Circle>();
		// Unique<Cube> cube;
		// Unique<Mesh> mesh;
	};
	static Primitives primitives;

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
		primitives.quad->Init();
		primitives.line->Init();
		primitives.circle->Init();
		// primitives.cube->Init();
		

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
		if (primitives.quad->GetIndexCount())
		{
			primitives.quad->Flush();
			stats.DrawCalls++;
		}

		if (primitives.line->GetVertexCount())
		{
			primitives.line->Flush();
			stats.DrawCalls++;	
		}

		if (primitives.circle->GetIndexCount())
		{
			primitives.circle->Flush();
			stats.DrawCalls++;	
		}

		// if (primitives.cube->IndexCount)
		// 	primitives.cube->Flush();
	}


	void Render2D::StartBatch() 
	{
		primitives.quad->StartBatch();
		primitives.line->StartBatch();
		primitives.circle->StartBatch();
		// primitives.cube->StartBatch();
	}


	void Render2D::NextBatch() 
	{
		primitives.quad->NextBatch();
		primitives.line->NextBatch();
		primitives.circle->NextBatch();
		// primitives.cube->NextBatch();
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
		primitives.quad->Draw(entity);
		stats.QuadCount++;
	}


	void Render2D::DrawLine(Entity& entity) 
	{
		primitives.line->Draw(entity);
		stats.LineCount++;
	}


	void Render2D::DrawCircle(Entity& entity) 
	{
		primitives.circle->Draw(entity);
		stats.CircleCount++;
	}
}