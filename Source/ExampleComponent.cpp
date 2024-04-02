/// @file       ExampleComponent.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      DESCRIPTION HERE
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "ExampleComponent.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ExampleComponent::ExampleComponent() :
        Component( typeid( ExampleComponent ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void ExampleComponent::OnInit()
    {

    }

    /// @brief  called once when exiting the scene
    void ExampleComponent::OnExit()
    {

    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ExampleComponent
    void ExampleComponent::Inspector()
    {

    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ExampleComponent
    /// @return the map of read methods for this ExampleComponent
    ReadMethodMap< ISerializable > const& ExampleComponent::GetReadMethods() const
    {
        static ReadMethodMap< ExampleComponent > const readMethods = {
            
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    
    /// @brief  writes this ExampleComponent to JSON
    /// @return the JSON data of this ExampleComponent
    nlohmann::ordered_json ExampleComponent::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ExampleComponent
    /// @return the newly created clone of this ExampleComponent
    ExampleComponent* ExampleComponent::Clone() const
    {
        return new ExampleComponent( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ExampleComponent
    /// @param  other   the other ExampleComponent to copy
    ExampleComponent::ExampleComponent( ExampleComponent const& other ) :
        Component( other )
    {}


//-----------------------------------------------------------------------------
