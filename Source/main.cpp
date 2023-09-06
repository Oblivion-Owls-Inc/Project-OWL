#include "Engine.h"
#include "PlatformSystem.h"

int main() {

    Engine engine;

    // TODO: add Systems to the engine here
    engine.AddSystem(new PlatformSystem);

    engine.Init();

    engine.Run();

    return 0;
}
