#include "PlatformSystem.h"
#include "glfw3.h"

PlatformSystem::PlatformSystem(const char* name, int w_width, int w_height) : 
	_window(nullptr),
	_width(800), _height(600) {}


GLFWwindow* PlatformSystem::GetWindowHandle() { return _window; }


void PlatformSystem::OnInit()
{
	
}

void PlatformSystem::OnUpdate(float dt)
{

}

void PlatformSystem::OnExit()
{

}

