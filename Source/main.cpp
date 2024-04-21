/// \file     Main.cpp
/// \author   Jax Clayton (jax.clayton@digipen.edu)
/// 
/// \brief    A version of Sprite specifically for rendering tiles.
/// 
/// \copyright Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "Engine.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") // hide console window

#ifdef _WIN32
    #include <Windows.h>
#endif // _WIN32

int main()
{
#ifdef _WIN32
    // Disable the StickyKeys feature
    STICKYKEYS keys = { sizeof(STICKYKEYS), 0 };
    keys.dwFlags = 0;
    SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &keys, 0);
#endif // _WIN32

    GameEngine()->Run();

    return 0;
}

