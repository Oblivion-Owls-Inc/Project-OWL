#include "Engine.h"
#include "PlatformSystem.h"
#include "DebugSystem.h"
#include "SceneSystem.h"
#include "Renderer.h"

int main() {

    Engine * engine = Engine::getInstance();

    // TODO: add Systems to the engine here
    engine->AddSystem( PlatformSystem::getInstance() );
    engine->AddSystem( SceneSystem::getInstance( "SceneName" ) );
    engine->AddSystem(_renderer);
    engine->AddSystem( DebugSystem::getInstance() );

    engine->Run();

    return 0;
}
