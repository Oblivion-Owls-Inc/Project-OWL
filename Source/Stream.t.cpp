/// @file   Stream.t.cpp
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.

#define STREAM_TC

#ifndef STREAM_H
#include "Stream.h"
#endif

#include <iostream>
#include <map>

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

/// @brief reads data into an existing complex type
/// @tparam T the type to read
/// @param object the oject to read data into
template < typename T >
void Stream::Read( T* object )
{
    // A map containing the different read methods for the object.
    std::map < std::string, ReadMethod< T > > const& readMethods;
    readMethods = object->getReadMethods();
    // Error checking.
    assert( value.IsObject() );
    // Iterates through the json value searching for a value.
    for ( auto& item : value.GetObject() )
    {
        // Error checking.
        assert( readMethods.find( item.name.GeString() ) != readMethods.end() );
        // Get the read method for the object from the map.
        ReadMethod< T > readMethod = readMethods.at( item.name.GetString() );
        // Read the data from the json value into the object. 
        object->readMethod( item.value );
    }
}
