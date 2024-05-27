#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "Core/Core.h"
#include "Core/EngineApp.h"


extern Cober::EngineApp* Cober::CreateApplication(AppCommandLineArgs args);

int main(int argc, char** argv)
{
	Cober::Log::Init();

	auto app = Cober::CreateApplication({ argc, argv });

	app->Start();

	app->Update();
	
	delete app;
}

#endif