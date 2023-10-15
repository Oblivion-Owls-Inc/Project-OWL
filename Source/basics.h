#pragma once

#include <glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <cassert>
#include <cstdlib>

#include <typeindex>

#pragma warning(disable : 26451)

/// @brief  returns a random float between min and max
/// @param  min the minimum possible value
/// @param  max the maximum possible value
/// @return the random value
__inline float random( float min, float max )
{
    return rand() * (max - min) / RAND_MAX + min;
}

/// @brief  linear interpolation between two values
/// @tparam T   the type of value to interpolate
/// @param  a   the value to interpolate from
/// @param  b   the value to interpolate to
/// @param  time    the time at which to inerpolate
/// @return the interpolated value
template< typename T >
__inline T lerp( T const& a, T const& b, float time )
{
    return b * time + a * (1 - time);
}

/// @brief  gets a unique ID value every time this function is called
/// @return the unique ID
/// @note   NOT THREAD SAFE
__inline unsigned GetUniqueId()
{
    static unsigned id = 0;
    return ++id;
}

/// @brief  gets a type name without a prefix
/// @param  type    the type to get the name of
/// @return the prefixless name of the type
__inline std::string PrefixlessName( std::type_index const& type )
{
    std::string name = type.name();
    
    size_t index = name.find( ' ' );
    if ( index == name.npos )
    {
        return name;
    }

    return name.substr( index + 1 );
}