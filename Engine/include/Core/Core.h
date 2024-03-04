#ifndef CORE_H
#define CORE_H

#include "PlatformDetection.h"

#include <memory>

#ifdef __linux__
	#include <signal.h>
	#define LOG_DEBUGBREAK() raise(SIGTRAP)
#else
	#define LOG_DEBUGBREAK() __debugbreak();
#endif


#define LOG_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); LOG_DEBUGBREAK(); } }
#define LOG_CORE_ASSERT(x, ...) { if(!(x)) { LOG_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); LOG_DEBUGBREAK(); } }

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Cober {

	template<typename T>
	using Unique = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Unique<T> CreateUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#endif