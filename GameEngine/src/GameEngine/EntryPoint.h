#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern GameEngine::Application* GameEngine::CreateApplication();

int main(int argc, char** argv)
{
	printf("Game Engine \n");
	auto app = GameEngine::CreateApplication(); // return new Sandbox()
	app->Run();
	delete app;
}



#endif // GE_PLATFORM_WINDOWS
