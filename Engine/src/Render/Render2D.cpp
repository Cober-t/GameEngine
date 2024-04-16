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
		// Unique<Line> line;
		// Unique<Circle> circle;
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
		// data.primitives.line->Init();
		// data.primitives.circle->Init();
		// data.primitives.cube->Init();
		

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

		// if (data.primitives.line->IndexCount)
		// 	data.primitives.line->Flush();

		// if (data.primitives.circle->IndexCount)
		// 	data.primitives.circle->Flush();

		// if (data.primitives.cube->IndexCount)
		// 	data.primitives.cube->Flush();
	}


	void Render2D::StartBatch() 
	{
		primitives.quad->StartBatch();
		// data.primitives.line->StartBatch();
		// data.primitives.circle->StartBatch();
		// data.primitives.cube->StartBatch();
	}


	void Render2D::NextBatch() 
	{
		primitives.quad->NextBatch();
		// data.primitives.line->NextBatch();
		// data.primitives.circle->NextBatch();
		// data.primitives.cube->NextBatch();
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


	void Render2D::DrawSolidQuad(Entity& entity) 
	{
		primitives.quad->Draw(entity);
		stats.QuadCount++;
	}
}