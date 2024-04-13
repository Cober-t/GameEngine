#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <stb_image.h>
#include "Core/Core.h"


namespace Cober {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth()  const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetWidth(uint32_t width)   = 0;
		virtual void SetHeight(uint32_t height) = 0;

		virtual uint32_t GetID() const = 0;
		virtual std::string GetName() const = 0;
		virtual std::string GetFormat() const = 0;
		virtual std::string GetPath() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) = 0;

		virtual bool operator==(const Texture& other) const = 0;

		static Ref<Texture> Create(uint32_t width, uint32_t height);
	};

}

#endif