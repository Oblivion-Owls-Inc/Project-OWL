#include "Engine.h"
#include "PlatformSystem.h"
#include "DebugSystem.h"

int main() {

    Engine engine;

    // TODO: add Systems to the engine here
    engine.AddSystem(new PlatformSystem);
    engine.AddSystem(new DebugSystem);
    engine.Init();

    engine.Run();

    return 0;
}
