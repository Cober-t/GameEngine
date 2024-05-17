#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <Engine.h>

#include "common.h"

namespace Cober {

	namespace Script {

		extern "C" __declspec(dllexport) void InitScripts()
		{
			// if  (Input::IsKeyPressed(Key::A))
			// {
			std::cout  << "Init dlls!" << std::endl;
			// }
		}


		extern "C" __declspec(dllexport) void UpdateScripts()
		{
			std::cout  << "Hello from the dll" << std::endl;
		}


		extern "C" __declspec(dllexport) void NotifyBeginContact(Entity* entityA, Entity* entityB)
		{
			auto colorAux = entityA->GetComponent<Render2DComponent>().color;
			entityA->GetComponent<Render2DComponent>().color = entityB->GetComponent<Render2DComponent>().color;
			entityB->GetComponent<Render2DComponent>().color = colorAux;
		}
	}
}



// #include <Windows.h>
//https://learn.microsoft.com/en-us/windows/win32/dlls/dllmain
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpvReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	// Initialize once for each new process.
	// Return FALSE to fail DLL load.
	break;

	case DLL_THREAD_ATTACH:
	// Do thread-specific initialization. (called when the process creates a new thread)
	//	and made in the context of the new thread.
	break;

	case DLL_THREAD_DETACH:
	// Do thread-specific cleanup.
	break;

	case DLL_PROCESS_DETACH:

	if (lpvReserved != nullptr)
	{
		break; // do not do cleanup if process termination scenario
	}


	// Perform any necessary cleanup.
	break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
