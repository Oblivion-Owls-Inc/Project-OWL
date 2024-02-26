/// @file   Stream.t.cpp
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
// template clipboard method definitions
//------------------------------------------------------------------------------

    /// @brief  copies a value to the clipbaord
    /// @tparam ValueType   the type of value to copy to the clipboard
    /// @param  value       the value to copy to the clipboard
    template< typename ValueType >
    void Stream::CopyToClipboard(ValueType const& value)
    {
        s_Clipboard = Write(value);
    }

    /// @brief  pastes the clipboard into a value
    /// @tparam ValueType   the type of value to paste the clipboard into
    /// @param  value       the value to paste the clipboard into
    template< typename ValueType >
    void Stream::PasteFromClipboard(ValueType& value)
    {
        Read(value, s_Clipboard);
    }

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
            std::cerr << "JSON Error: unexpected json type \"" << json.type_name()
                << "\" encountered (expected an Array instead) while trying to read vector of "
                << std::to_string(size) << " " << typeid(ValueType).name()
                << "s at " << GetDebugLocation() << std::endl;
            return;
        }

        int count = size;
        if (json.size() != size)
        {
            std::cerr << "JSON Warning: expected an array of size " << size << " while reading a vector of " << typeid(ValueType).name()
                << "s but encountered an array of size " << json.size() << " instead "
                << " at " << GetDebugLocation() << std::endl;

            count = std::min((int)json.size(), size);
        }

        for (int i = 0; i < count; ++i)
        {
            (*value)[i] = json[i].get< ValueType >();
        }
    }

    /// @brief Reads a vector of a standard type from a JSON file.
    /// @tparam ValueType - the type of data to read into the vector,
    /// @param vector     - the vector to read into.
    /// @param json       - the JSON to read from.
    template<typename ValueType>
    void Stream::Read(std::vector<ValueType>* vec_data, nlohmann::ordered_json const& json)
    {
        // Error checking
        if (json.is_array() == false)
        {
            std::cerr << "JSON Error: unexpected json type \"" << json.type_name()
                << "\" encountered (expected an Array instead) while trying to read vector of " << typeid(ValueType).name()
                << " at " << GetDebugLocation() << std::endl;
            return;
        }

        // Resize the vector to match the JSON size.
        vec_data->resize(json.size());
        // Read in the data from the JSON.
        for (int i = 0; i < vec_data->size(); i++)
        {
            Read( (*vec_data)[ i ], json[ i ] );
        }
    }

    /// @brief Reads an array of a standard type from a JSON file.
    /// @tparam ValueType - the data type of the array.
    /// @tparam Size      - the size of the array.
    /// @param array      - the array to read into.
    template<typename ValueType, int Size>
    void Stream::Read(ValueType* array_data, nlohmann::ordered_json const& json)
    {
        // JSON Error checking
        if (json.is_array() == false)
        {
            std::cerr << "JSON Error: unexpected json type \"" << json.type_name()
                << "\" encountered (expected an Array instead) while trying to read vector of " << typeid(ValueType).name()
                << " at " << GetDebugLocation() << std::endl;
            return;
        }

        // If there is a size mismatch throw an error
        if (Size != json.size())
        {
            std::cerr << "Error: array size mismatch encountered. JSON size: " << json.size() <<
                "\"  Array size: " << Size << std::endl;
            return;
        }

        // Read in data from JSON.
        for (int i = 0; i < Size; i++)
        {
            array_data[i] = Read<ValueType>(json[i]);
        }
    }

    /// @brief Reads a map with a string key and any standard data type from a JSON.
    /// @tparam ValueType - the type of map value.
    /// @param map_data   - pointer tot he map to read into.
    /// @param json       - the JSON to read from.
    template<typename ValueType>
    void Stream::Read(std::map<std::string, ValueType>* map_data, nlohmann::ordered_json const& json)
    {
        // JSON Error checking
        if (json.is_object() == false)
        {
            std::cerr << "JSON Error: unexpected json type \"" << json.type_name()
                << "\" encountered (expected an Object instead) while trying to read object of " << typeid(ValueType).name()
                << " at " << GetDebugLocation() << std::endl;
            return;
        }

        // Cleaar the map
        map_data->clear();

        // Iterate through the JSON object.
        for ( auto const& [ key, value ] : json )
        {
            // Initialise the element.
            ValueType element();
            // Read the value from the JSON to the element
            Read(element, value);
            // Add the key & element from the JSON to the map
            map_data->emplace(key, element);
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
        // JSON object to write the glm vector to.
        nlohmann::ordered_json json;
        // Write the data to the JSON object.
        for (int i = 0; i < size; ++i)
        {
            json[i] = value[i];
        }

        return json;
    }

    /// @brief Write a vector of any standard data type to a JSON
    /// @tparam ValueType - type of data stored by the vector.
    /// @param vector     - the vector to write to JSON.
    /// @return The JSON object containing the vector data.
    template<typename ValueType>
    nlohmann::ordered_json Stream::WriteArray(std::vector<ValueType> const& vec_data)
    {
        // The JSON object to store the vector in.
        nlohmann::ordered_json data;

        // Write the data to the JSON object.
        for (ValueType const& element : vec_data)
        {
            data.push_back( Stream::Write(element) );
        }

        return data;
    }

    /// @brief Write the data from an array into a JSON object
    /// @tparam ValueType - the type of data in the array
    /// @tparam Size      - the size of the array.
    /// @param array_data - Pointer to the array to write.
    /// @return A JSON object containing the array.
    template<typename ValueType, int Size>
    nlohmann::ordered_json Stream::Write(ValueType* array_data)
    {
        // The JSON object to store the array in.
        nlohmann::ordered_json data;
        // Write the data to a JSON object.
        for (int i = 0; i < Size; i++)
        {
            data[i] = array_data[i];
        }

        return data;
    }

    /// @brief Write a map of string keys and any standard data type to a JSON object.
    /// @tparam ValueType - the data type of the map values.
    /// @param map_data   - the map to write to a JSON object.
    /// @return The JSON object containing the written map data.
    template<typename ValueType>
    nlohmann::ordered_json Stream::Write(std::map<std::string, ValueType> map_data)
    {
        // The JSON object to write to.
        nlohmann::ordered_json data;
        // Write map into JSON object
        for (auto& [key, value] : map_data)
        {
            data[key] = Write(value);
        }

        return data;
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
                typeid(ValueType).name() << " at " << GetDebugLocation() << std::endl;
            return;
        }

        ReadMethodMap< ISerializable > const& readMethods = value.GetReadMethods();
        for (auto& [name, data] : json.items())
        {
            auto it = readMethods.find(name);
            if (it == readMethods.end())
            {
                std::cerr << "JSON Warning: unrecognized token " << name << " encountered while trying to read " <<
                    typeid(ValueType).name() << " at " << GetDebugLocation() << std::endl;
                continue;
            }

            s_DebugLocationStack.push_back( name + "." );

            ReadMethod< ISerializable > const& readMethod = it->second;
            (value.*readMethod)(data);

            s_DebugLocationStack.pop_back();
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
