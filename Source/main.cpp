#include "Engine.h"
#pragma warning(disable : 4099) //Disabling warning for missing PDB 
#pragma warning(disable : 33019) //Disabling warning for missing PDB files

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



