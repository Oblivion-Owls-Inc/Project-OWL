#include "Engine.h"
#include "PlatformSystem.h"
#include "DebugSystem.h"
#include "SceneSystem.h"
#include "RenderSystem.h"
#pragma warning(disable : 4099) //Disabling warning for missing PDB files


int main() {

    Engine * engine = Engine::getInstance();

    // engine->AddSystem( PlatformSystem::getInstance() );
    // engine->AddSystem( SceneSystem::getInstance( "SceneName" ) );
    // engine->AddSystem( RenderSystem::getInstance() );
    // engine->AddSystem( DebugSystem::getInstance() );

    engine->Run();

    return 0;
}
