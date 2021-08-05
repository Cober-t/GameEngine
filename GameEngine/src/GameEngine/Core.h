#pragma once
// Macros se incluyen en: Propiedades -> C/C++ -> Preprocesador
#ifdef GE_PLATFORM_WINDOWS
	// Se incluye este macro solo en GameEngine
	#ifdef GE_BUILD_DLL // Si intentamos compilar el DLL de GameEngine
		#define GameEngine_API __declspec(dllexport)
	#else // GE_BUILD_DLL
		#define GameEngine_API __declspec(dllimport)
	#endif
#else
	#error GameEngine only supports Windows!
#endif // GE_PLATFORM_WINDOWS


