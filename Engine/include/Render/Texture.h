#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <stb_image.h>
#include "Core/Core.h"


namespace Cober {

	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	class CB_API Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth()  const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;


		virtual std::string GetName()   const = 0;
		virtual std::string GetFormat() const = 0;
		virtual const std::string& GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;

		static Ref<Texture> Create(uint32_t width, uint32_t height);
		static Ref<Texture> Create(const std::string& path);
	};
}

#endif