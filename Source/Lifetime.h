/// @file       Lifetime.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that destroys it's Enity after a given length of time
/// @version    0.1
/// @date       2024-02-13
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once


#include "Pool.h"

/// @brief  Component that destroys it's Enity after a given length of time
class Lifetime : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    Lifetime();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets this Lifetime's lifetime pool
    /// @return this Lifetime's lifetime pool
    Pool< float >* GetLifetime();


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called every simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the amount of time until this Component's Entity will be destroyed
    Pool< float > m_Lifetime = Pool< float >( 10.0f );


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for Lifetime
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the amount of time until this Component's Entity will be destroyed
    /// @param  data    the JSON data to read from
    void readLifetime( nlohmann::ordered_json const& data );


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


    /// @brief  clones this Lifetime
    /// @return the newly created clone of this Lifetime
    virtual Lifetime* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Lifetime
    /// @param  other   the other Lifetime to copy
    Lifetime( Lifetime const& other );


    // diable = operator
    void operator =( Lifetime const& ) = delete;


//-----------------------------------------------------------------------------
};
