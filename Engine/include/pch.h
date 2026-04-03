#ifndef PRECOMPILE_H    
#define PRECOMPILE_H

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <string.h>	// For memset and memcpy Linux
#include <sstream>
#include <filesystem>
#include <array>
#include <stack>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

/* --- CUSTOM HEADERS --- */
#include <SDL3/SDL.h>

#include <Core/MemoryDebug.h>
#include <Core/Core.h>
#include <Core/PathService.h>
#include <Core/Log.h>
#include <Events/KeyCodes.h>


#endif