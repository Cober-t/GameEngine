#pragma once

#ifdef GE_PLATFORM_WINDOWS
	#ifdef GE_BUILD_DLL
		#define GameEngine_API __declspec(dllexport)
	#else // GE_BUILD_DLL
		#define GameEngine_API __declspec(dllimport)
	#endif
#else
	#error GameEngine only supports Windows!
#endif // GE_PLATFORM_WINDOWS


