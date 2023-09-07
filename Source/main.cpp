#include "Engine.h"
#include "PlatformSystem.h"
#include "DebugSystem.h"

int main() {

    Engine engine;

    // TODO: add Systems to the engine here
    engine.AddSystem(PlatformSystem::getInstance());
    engine.AddSystem(new DebugSystem);

    engine.Run();

    return 0;
}
