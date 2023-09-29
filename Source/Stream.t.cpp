/// @file   Stream.t.cpp
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.
/// -----------------------------------------------------------

#define STREAM_TC

#ifndef STREAM_H
#include "Stream.h"
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include <iostream>
#include <sstream>
#include <map>

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

/// @brief    Reads data into an existing complex type
/// @tparam T The type to read
/// @param    Object the oject to read data into
template < typename T >
void Stream::Read(T* object)
{
    // A map containing the different read methods for the object.
    std::map <std::string, ReadMethod<T>> const& readMethods(object->GetReadMethods());
    // Error checking.
    assert( value.IsObject() );
    // Iterates through the json value searching for a value.
    for ( auto& item : value.GetObject() )
    {
        // Error checking.
        if ( readMethods.find( item.name.GetString() ) == readMethods.end() )
        {
            throw std::runtime_error(
                (
                    std::stringstream() <<
                    "unreconized token \"" <<
                    item.name.GetString() <<
                    "\" encountered while reading " <<
                    typeid( T ).name()
                ).str()
            );
        }
        // Get the read method for the object from the map.
        ReadMethod< T > readMethod = readMethods.at(item.name.GetString());
        // Read the data from the json value into the object. 
        (object->*readMethod)(Stream(item.value));
    }

    auto afterLoad = readMethods.find("AFTERLOAD");
    if (afterLoad != readMethods.end())
    {
        (object->*(afterLoad->second))( Stream( rapidjson::Value() ) );
    }
}
