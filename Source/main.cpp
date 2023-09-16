#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine.h"
#include "PlatformSystem.h"
#include "DebugSystem.h"
#include "SceneSystem.h"
#include "RenderSystem.h"
#pragma warning(disable : 4099) //Disabling warning for missing PDB files


int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    // *** Initialize the window using glfw *******************************************************

    Engine* engine = Engine::getInstance();

    // engine->AddSystem( PlatformSystem::getInstance() );
    // engine->AddSystem( SceneSystem::getInstance( "SceneName" ) );
    // engine->AddSystem( RenderSystem::getInstance() );
    // engine->AddSystem( DebugSystem::getInstance() );

    engine->Run();

    return 0;

}
