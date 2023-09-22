#define WIN32_LEAN_AND_MEAN
#include "Engine.h"
#include "DebugSystem.h"
#include "SceneSystem.h"
#include "RenderSystem.h"
#pragma warning(disable : 4099) //Disabling warning for missing PDB 
#pragma warning(disable : 33019) //Disabling warning for missing PDB files

int main ()
{

    // *** Initialize the window using glfw *******************************************************

    Engine* engine = Engine::getInstance();

    // engine->AddSystem( PlatformSystem::getInstance() );
    // engine->AddSystem( SceneSystem::getInstance( "SceneName" ) );
    // engine->AddSystem( RenderSystem::getInstance() );
    // engine->AddSystem( DebugSystem::getInstance() );

    engine->Run();

    return 0;

}



