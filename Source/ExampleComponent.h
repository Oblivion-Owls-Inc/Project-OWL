/// @file       ExampleComponent.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      DESCRIPTION HERE
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"


/// @brief  DESCRIPTION HERE
class ExampleComponent : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ExampleComponent();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ExampleComponent
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ExampleComponent
    /// @return the newly created clone of this ExampleComponent
    virtual ExampleComponent* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ExampleComponent
    /// @param  other   the other ExampleComponent to copy
    ExampleComponent( ExampleComponent const& other );


    // diable = operator
    void operator =( ExampleComponent const& ) = delete;


//-----------------------------------------------------------------------------
};
