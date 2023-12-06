#include "Engine.h"
#define SOL_ALL_SAFETIES_ON 1

int main()
{
    // *** Initialize the window using glfw *******************************************************

    Engine* engine = Engine::GetInstance();

    engine->Run();
    return 0;
}

