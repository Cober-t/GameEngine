#ifndef MEMORY_DEBUG
#define MEMORY_DEBUG

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#ifndef DEBUG_NEW
		#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
		#endif
	#define new DEBUG_NEW
#endif

#endif