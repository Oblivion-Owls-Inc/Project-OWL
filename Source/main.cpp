#include "Engine.h"
#include "PlatformSystem.h"

int main() {

    Engine engine;

    // TODO: add Systems to the engine here
    engine.AddSystem(PlatformSystem::getInstance());

    engine.Run();

    return 0;
}
