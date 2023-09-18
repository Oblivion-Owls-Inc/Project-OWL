/// @file ExampleSystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Example System meant to be copy-pasted when creating new Systems
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "ExampleSystem.h"

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// private methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// private static methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// class specific Read methods
//-----------------------------------------------------------------------------

    /// @brief Reads the data for exampleMember from JSON into this System
    /// @param jsonValue the json value to be read into this System
    void ExampleSystem::ReadExampleMember( rapidjson::Value const& jsonValue )
    {
        exampleMember = jsonValue.GetInt();
    }

    /// @brief the Read Methods used in this System
    std::map< std::string, ReadMethod< ExampleSystem > > const ExampleSystem::ReadMethods = {
        { "exampleMember", &ReadExampleMember }
    };

//-----------------------------------------------------------------------------
// default Read method stuff
//-----------------------------------------------------------------------------

    /// @brief Gets the read methods of this System
    /// @return the map of read methods of this System
    std::map< std::string, ReadMethod< System > > const& ExampleSystem::GetReadMethods()
    {
        return (std::map< std::string, ReadMethod< System > > const&)ReadMethods;
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

    /// @brief Constructs the ExampleSystem
    ExampleSystem::ExampleSystem() :
        exampleMember( 0 )
    {}

    /// @brief The singleton instance of ExampleSystem
    ExampleSystem * ExampleSystem::instance = nullptr;

    /// @brief gets the instance of ExampleSystem
    /// @return the instance of the ExampleSystem
    ExampleSystem * ExampleSystem::getInstance()
    {
        if ( instance == nullptr )
        {
            instance = new ExampleSystem();
        }
        return instance;
    }

//-----------------------------------------------------------------------------
