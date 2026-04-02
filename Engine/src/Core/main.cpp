#ifdef _DEBUG

#include "Core/MemoryDebug.h"
#include "Core/Core.h"
#include "Core/EngineApp.h"


extern Cober::EngineApp* Cober::CreateApplication(AppCommandLineArgs args);

int main(int argc, char** argv)
{
	//_CrtSetBreakAlloc(5006);
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	{
		Cober::Log::Init();

		// This WILL be detected by ASan (raw leak)
		int* rawLeak = new int(42);

		// This WON'T be detected by ASan (proper cleanup, just late)
		static auto smartLeak = std::make_shared<int>(123);

		auto app = Cober::CreateApplication({ argc, argv });

		app->Start();
		app->Update();
		delete app;
	}
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
}
#endif
