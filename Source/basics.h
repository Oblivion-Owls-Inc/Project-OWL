#pragma once
#include "glew.h"
#include "glfw/glfw3.h"
#include "glm/glm.hpp"

#include <iostream>
#include <cassert>
#include <cstdlib>
#pragma warning(disable : 26451)

__inline float random( float min, float max )
{
    return rand() * (max - min) / RAND_MAX + min;
}

template< typename T >
__inline T lerp( T const& a, T const& b, float time )
{
    return b * time + a * (1 - time);
}
