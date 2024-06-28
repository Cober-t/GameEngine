#include <pch.h>
#include "Render/Primitives/Text.h"
#include "Render/RenderGlobals.h"
#include "Render/Render2D.h"
#include "Render/Text/MSDFData.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Cober {

	namespace Primitive {

		static Text::Data data;

		void Text::CleanVertexBuffer() 
		{
			delete[] data.VertexBufferBase;
		}

		void Text::Init() 
		{
			data.VertexArray = VertexArray::Create();

			data.VertexBuffer = VertexBuffer::Create(Render2D::GetStats().MaxVertices * sizeof(Attributes));
			data.VertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color"    },
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Int,    "a_EntityID" }
			});

			data.VertexArray->AddVertexBuffer(data.VertexBuffer);
			data.VertexBufferBase = new Attributes[Render2D::GetStats().MaxVertices];
			
			uint32_t* textIndices = Render2D::GetStats().GetIndices();

			Ref<IndexBuffer> textIB = IndexBuffer::Create(textIndices, Render2D::GetStats().MaxVertices);
			data.VertexArray->SetIndexBuffer(textIB); // The shame as Quads Index Buffer
			delete[] textIndices;

			data.Shader = Shader::Create("Text.glsl");
		}


		void Text::Flush()
		{
			if (data.IndexCount)
			{
				uint32_t dataSize = (uint32_t)((uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase);
				data.VertexBuffer->SetData(data.VertexBufferBase, dataSize);

                auto buf = data.VertexBufferBase;
    			data.FontAtlasTexture->Bind(0);

				data.Shader->Bind();
				RenderGlobals::DrawIndexed(data.VertexArray, data.IndexCount);

				Render2D::GetStats().DrawCalls++;
			}
		}


		void Text::NextBatch()
		{
			Flush();
			StartBatch();
		}


		void Text::EndBatch()
		{
			Flush();
		}


		void Text::StartBatch()
		{
			data.IndexCount = 0;
			data.VertexBufferPtr = data.VertexBufferBase;
		}


		uint32_t Text::GetIndexCount()
		{
			return data.IndexCount;
		}


		void Text::Draw(Entity& entity)
		{
			auto& enttTrans = entity.GetComponent<TransformComponent>();
			auto& enttText = entity.GetComponent<TextComponent>();

			SetAttributes(enttText.Text, enttText.FontAsset, enttTrans.GetTransform(), 
					{ enttText.Color, enttText.Kerning, enttText.LineSpacing }, (int)entity);
		}
		
		void Text::SetAttributes(const std::string& string, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, int entityID)
		{
            const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
			const auto& metrics = fontGeometry.getMetrics();
			Ref<Texture> fontAtlas = font->GetAtlasTexture();

			data.FontAtlasTexture = fontAtlas;

			double x = 0.0;
			double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
			double y = 0.0;

			const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();
			
			for (size_t i = 0; i < string.size(); i++)
			{
				char character = string[i];
				if (character == '\r')
					continue;

				if (character == '\n')
				{
					x = 0;
					y -= fsScale * metrics.lineHeight + textParams.LineSpacing;
					continue;
				}

				if (character == ' ')
				{
					float advance = spaceGlyphAdvance;
					if (i < string.size() - 1)
					{
						char nextCharacter = string[i + 1];
						double dAdvance;
						fontGeometry.getAdvance(dAdvance, character, nextCharacter);
						advance = (float)dAdvance;
					}

					x += fsScale * advance + textParams.Kerning;
					continue;
				}

				if (character == '\t')
				{
					x += 4.0f * (fsScale * spaceGlyphAdvance + textParams.Kerning);
					continue;
				}

				auto glyph = fontGeometry.getGlyph(character);
				if (!glyph)
					glyph = fontGeometry.getGlyph('?');
				if (!glyph)
					return;

				double al, ab, ar, at;
				glyph->getQuadAtlasBounds(al, ab, ar, at);
				glm::vec2 texCoordMin((float)al, (float)ab);
				glm::vec2 texCoordMax((float)ar, (float)at);

				double pl, pb, pr, pt;
				glyph->getQuadPlaneBounds(pl, pb, pr, pt);
				glm::vec2 quadMin((float)pl, (float)pb);
				glm::vec2 quadMax((float)pr, (float)pt);

				quadMin *= fsScale, quadMax *= fsScale;
				quadMin += glm::vec2(x, y);
				quadMax += glm::vec2(x, y);

				float texelWidth = 1.0f / fontAtlas->GetWidth();
				float texelHeight = 1.0f / fontAtlas->GetHeight();
				texCoordMin *= glm::vec2(texelWidth, texelHeight);
				texCoordMax *= glm::vec2(texelWidth, texelHeight);

				// render here
				data.VertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
				data.VertexBufferPtr->Color = textParams.Color;
				data.VertexBufferPtr->TexCoord = texCoordMin;
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;

				data.VertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
				data.VertexBufferPtr->Color = textParams.Color;
				data.VertexBufferPtr->TexCoord = { texCoordMin.x, texCoordMax.y };
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;

				data.VertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
				data.VertexBufferPtr->Color = textParams.Color;
				data.VertexBufferPtr->TexCoord = texCoordMax;
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;

				data.VertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
				data.VertexBufferPtr->Color = textParams.Color;
				data.VertexBufferPtr->TexCoord = { texCoordMax.x, texCoordMin.y };
				data.VertexBufferPtr->EntityID = entityID;
				data.VertexBufferPtr++;

				data.IndexCount += 6;
				Render2D::GetStats().TextCount++;

				if (i < string.size() - 1)
				{
					double advance = glyph->getAdvance();
					char nextCharacter = string[i + 1];
					fontGeometry.getAdvance(advance, character, nextCharacter);

					x += fsScale * advance + textParams.Kerning;
				}
			}
		}
	}
}