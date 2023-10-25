/// @file   Stream.cpp
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @brief  JSON File reading/writing
/// @version 0.1
/// @date 2023-09-10
///
/// @copyright  © 2023 DigiPen (USA) Corporation.
/// ------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files
//------------------------------------------------------------------------------


#define STREAM_T_CPP

#ifndef STREAM_H
#include "Stream.h"
#endif


//------------------------------------------------------------------------------
// template read method definitions
//------------------------------------------------------------------------------

    /// @brief  reads a basic type from json
    /// @tparam ValueType   the type to read
    /// @param  json        the json data to read from
    /// @return the read data
template< typename ValueType >
ValueType Stream::Read(nlohmann::ordered_json const& json)
{
    // TODO: error handling
    return json.get< ValueType >();
}

/// @brief  reads a basic type from json
/// @tparam ValueType   the type to read
/// @param  value       pointer to where to store the read data
/// @param  json        the json data to read from
template< typename ValueType >
void Stream::Read(ValueType& value, nlohmann::ordered_json const& json)
{
    // call a function depending on whether or not ValueType is a pointer
    Read< ValueType >(
        value, json,
        IsPointer<
        std::is_pointer_v< ValueType >
        >()
    );
}

/// @brief  reads a glm vector from json
/// @tparam size        the size of the vector
/// @tparam ValueType   the value type of the vector
/// @param  json        the json data to read from
/// @return the read vector
template< int size, typename ValueType >
glm::vec< size, ValueType > Stream::Read(nlohmann::ordered_json const& json)
{
    glm::vec< size, ValueType > value;
    Read< size, ValueType >(&value, json);
    return value;
}

/// @brief  reads a glm vector from json
/// @tparam size        the size of the vector
/// @tparam ValueType   the value type of the vector
/// @param  value       the vector to read into
/// @param  json        the json data to read from
template< int size, typename ValueType >
void Stream::Read(glm::vec< size, ValueType >* value, nlohmann::ordered_json const& json)
{
    if (json.is_array() == false)
    {
        std::cerr << "JSON Error: unexpected json type \"" << json.type_name() <<
            "\" encountered (expected an Array instead) while trying to read vector of " <<
            std::to_string(size) << " " << typeid(ValueType).name() << "s" << std::endl;
        return;
    }

    int count = size;
    if (json.size() != size)
    {
        std::cout << "JSON Warning: expected an array of size " << size << " while reading a vector of " << typeid(ValueType).name() <<
            "s but encountered an array of size " << json.size() << " instead" << std::endl;

        count = std::min((int)json.size(), size);
    }

    for (int i = 0; i < count; ++i)
    {
        (*value)[i] = json[i].get< ValueType >();
    }
}

//------------------------------------------------------------------------------
// template write method definitions
//------------------------------------------------------------------------------


    /// @brief  writes data to json
    /// @tparam ValueType   the type of data to write to json
    /// @param  value       the data to write to json
    /// @return the written json data
template< typename ValueType >
nlohmann::ordered_json Stream::Write(ValueType const& value)
{
    // call the appropriate function based on if ValueType is a pointer
    return Write< ValueType >(
        value,
        IsPointer<
        std::is_pointer_v< ValueType >
        >()
    );
}


/// @brief  writes a glm vector to json
/// @tparam ValueType   the type of value of the vector
/// @tparam size        the size of the vector
/// @param  value       the vector to write
/// @return the json data of the vector
template< int size, typename ValueType >
nlohmann::ordered_json Stream::Write(glm::vec< size, ValueType > const& value)
{
    nlohmann::ordered_json json;

    for (int i = 0; i < size; ++i)
    {
        json[i] = value[i];
    }

    return json;
}

//-----------------------------------------------------------------------------
// private: pointer-aware methods
//-----------------------------------------------------------------------------

    /// @brief  reads data into a value by pointer
    /// @tparam ValueType   the type of value to read
    /// @param  value       the value to read data into
    /// @param  json        the json data to read from
template< typename ValueType >
void Stream::Read(ValueType& value, nlohmann::ordered_json const& json, IsPointer<true>)
{
    // recursively remove pointers until all pointers have been removed
    Read<
        std::remove_pointer_t< ValueType >
    >(
        *value, json,
        IsPointer<
        std::is_pointer_v<
        std::remove_pointer_t< ValueType >
        >
        >()
    );
}

/// @brief  reads data into a value by reference
/// @tparam ValueType   the type of value to read
/// @param  value       the value to read data into
/// @param  json        the json data to read from
template< typename ValueType >
void Stream::Read(ValueType& value, nlohmann::ordered_json const& json, IsPointer<false>)
{
    // call the appropriate function based on if ValueType is derived from ISerializable
    Read< ValueType >(
        value, json,
        IsISerializable<
        std::derived_from< ValueType, ISerializable >
        >()
    );
}


/// @brief  writes data into a value by pointer
/// @tparam ValueType   the type of value to write
/// @param  value       the value to write
/// @return the written json data
template< typename ValueType >
nlohmann::ordered_json Stream::Write(ValueType const& value, IsPointer<true>)
{
    if (value == nullptr)
    {
        // write null
        return nlohmann::ordered_json();
    }

    // recursively remove pointers
    return Write< std::remove_pointer_t< ValueType > >(*value, IsPointer< std::is_pointer_v< std::remove_pointer_t< ValueType > > >());
}

/// @brief  writes data into a value by pointer
/// @tparam ValueType   the type of value to write
/// @param  value       the value to write
/// @return the written json data
template< typename ValueType >
nlohmann::ordered_json Stream::Write(ValueType const& value, IsPointer<false>)
{
    // call the appropriate function based on if ValueType is ISerializable
    return Write< ValueType >(
        value,
        IsISerializable<
        std::derived_from< ValueType, ISerializable >
        >()
    );
}

//-----------------------------------------------------------------------------
// private: ISerializable-aware methods
//-----------------------------------------------------------------------------


    /// @brief  reads an ISerializable
    /// @tparam ValueType   the type to read
    /// @param  value       the value to read into
    /// @param  json        the json data to read from
template< typename ValueType >
void Stream::Read(ValueType& value, nlohmann::ordered_json const& json, IsISerializable<true>)
{
    if (json.is_object() == false)
    {
        std::cerr << "JSON Error: unexpected type " << json.type_name() << " encountered while trying to read json Object of " <<
            typeid(ValueType).name() << std::endl;
        return;
    }

    ReadMethodMap< ISerializable > const& readMethods = value.GetReadMethods();
    for (auto& [name, data] : json.items())
    {
        auto it = readMethods.find(name);
        if (it == readMethods.end())
        {
            std::cerr << "JSON Error: unrecognized token " << name << " encountered while trying to read " <<
                typeid(ValueType).name() << std::endl;
            continue;
        }

        ReadMethod< ISerializable > const& readMethod = it->second;
        (value.*readMethod)(data);
    }

    value.AfterLoad();
}

/// @brief  reads a basic type
/// @tparam ValueType   the type to read
/// @param  value       the value to read into
/// @param  json        the json data to read from
template< typename ValueType >
void Stream::Read(ValueType& value, nlohmann::ordered_json const& json, IsISerializable<false>)
{
    value = json.get< ValueType >();
}


/// @brief  write an ISerializable
/// @tparam ValueType   the type to write
/// @param  value       the value to write
/// @return the written json data
template< typename ValueType >
nlohmann::ordered_json Stream::Write(ValueType const& value, IsISerializable<true>)
{
    return value.Write();
}

/// @brief  write a basic type
/// @tparam ValueType   the type to write
/// @param  value       the value to write
/// @return the written json data
template< typename ValueType >
nlohmann::ordered_json Stream::Write(ValueType const& value, IsISerializable<false>)
{
    return nlohmann::ordered_json(value);
}

//------------------------------------------------------------------------------
