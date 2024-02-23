

#pragma once

#include <glew.h>
#include <glfw/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <atomic>
#include <typeindex>
#include <random>

#include "ISerializable.h"
#pragma warning( disable : 26451 )

static std::default_random_engine __rng;

/// @brief  returns a random float between min and max
/// @param  min the minimum possible value
/// @param  max the maximum possible value
/// @return the random value
__inline float random( float min, float max )
{
    return std::uniform_real_distribution< float >( min, max )( __rng );
}

/// @brief  returns a random int between min and max
/// @param  min the minimum possible value
/// @param  max the maximum possible value
/// @return the random value
__inline int random( int min, int max )
{
    return std::uniform_int_distribution< int >( min, max )( __rng );
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
/// @note   MODIFYED FROM THE ORIGINAL TO USE ATOMIC 
__inline unsigned GetUniqueId()
{
    static std::atomic< unsigned > id{ 0 };
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

template< typename T >
__inline int Sign( T const& value )
{
    return ( value > 0 ) - ( value < 0 );
}