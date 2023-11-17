#include "Engine.h"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

    // *** Initialize the window using glfw *******************************************************

    Engine* engine = Engine::GetInstance();

    // engine->AddSystem( PlatformSystem::GetInstance() );
    // engine->AddSystem( SceneSystem::GetInstance( "SceneName" ) );
    // engine->AddSystem( RenderSystem::GetInstance() );
    // engine->AddSystem( DebugSystem::GetInstance() );

    engine->Run();


    return 0;
}



