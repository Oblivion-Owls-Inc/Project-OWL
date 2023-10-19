#include "Engine.h"

int main ()
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



