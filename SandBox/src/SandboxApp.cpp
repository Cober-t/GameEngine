// Para evitar usar direccciones relativas "GameEngine/Application/h":
// Propiedades -> C/C++ -> General -> Directorios de inlusion adicionales -> $(SolutionDir)GameEngine\src;
#include <GameEngine.h>

class SandBox : public GameEngine::Application
{
public:
	SandBox() { }
	~SandBox() { }
};

/* Movemos este EntryPoint a un archivo aparte en el GameEngine que se incluira
 * Asi modulamos la parte GameEngine y la parte aplicacion
 ------------------------- Entry Point
	void main() 
	{
		Sandbox* sandbox = new Sandbox();
		sandbox->Run();
		delete sandbox;
	}
*/

GameEngine::Application* GameEngine::CreateApplication() {
	// Alojamos el objeto en la cola porque, al ser nuestro proyecto completo, ocupara mucho
	return new SandBox();
}