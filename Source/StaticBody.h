/// @file       StaticBody.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      pysics body which can be collided with and does not move
/// @version    0.1
/// @date       2023-10-16
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "Component.h"

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------

class Transform;
class Collider;

//-----------------------------------------------------------------------------
// class
//-----------------------------------------------------------------------------
class StaticBody : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the StaticBody class.
    StaticBody();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief Get the restitution of the rigidBody.
    /// @return The restitution.
    float GetRestitution() const { return m_Restitution; }

    /// @brief Set the restitution of the rigidBody.
    /// @param restitution The new restitution.
    void SetRestitution( float restitution ) { m_Restitution = restitution; }


    /// @brief Get the friction of the rigidBody.
    /// @return The friction.
    float GetFriction() const { return m_Friction; }

    /// @brief Set the friction of the rigidBody.
    /// @param friction The new friction.
    void SetFriction( float friction ) { m_Friction = friction; }

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief how bouncy this StaticBody is
    float m_Restitution = 0.0f;

    /// @brief how much friction this StaticBody has
    float m_Friction = 0.0f;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the restitution from json
    /// @param  data    the json data
    void readRestitution( nlohmann::ordered_json const& data );

    /// @brief  reads the friction from json
    /// @param  data    the json data
    void readFriction( nlohmann::ordered_json const& data );


    /// @brief the map of read methods for StaticBodys
    static ReadMethodMap< StaticBody > s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief  Write all StaticBody component data to a JSON file.
    /// @return The JSON file containing the StaticBody component data.
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  clones this StaticBody
    /// @return the newly created clone of this StaticBody
    virtual Component* Clone() const override { return new StaticBody( *this ); }

    /// @brief  copy-constructor for the StaticBody
    /// @param  other   the other StaticBody to copy
    StaticBody( const StaticBody& other );


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    // diable = operator
    void operator =( const StaticBody& ) = delete;

//-----------------------------------------------------------------------------
};
