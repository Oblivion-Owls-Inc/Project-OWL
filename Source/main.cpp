#include "Engine.h"
#include "PlatformSystem.h"
#include "DebugSystem.h"
#include "Renderer.h"

int main() {

    Engine engine;

    // TODO: add Systems to the engine here
    engine.AddSystem(PlatformSystem::getInstance());
    engine.AddSystem(Renderer::getInstance());
    engine.AddSystem(DebugSystem::getInstance());

    engine.Run();

    return 0;
}
