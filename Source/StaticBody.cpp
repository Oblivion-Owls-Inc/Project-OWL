/// @file       StaticBody.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      pysics body which can be collided with and does not move
/// @version    0.1
/// @date       2023-10-16
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "pch.h" // precompiled header has to be included first


#include "DebugSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the StaticBody class.
    StaticBody::StaticBody() :
        Component( typeid( StaticBody ) )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Used by the Debug System to display information about this Component
    void StaticBody::Inspector()
    {
        ImGui::DragFloat( "Restitution", &m_Restitution, 0.05f, 0.0f, 100.0f );
        ImGui::DragFloat( "Friction", &m_Friction, 0.05f, -100.0f, 1000000.0f );
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the restitution from json
    /// @param  data    the json data
    void StaticBody::readRestitution( nlohmann::ordered_json const& data )
    {
        m_Restitution = Stream::Read<float>( data );
    }

    /// @brief  reads the friction from json
    /// @param  data    the json data
    void StaticBody::readFriction( nlohmann::ordered_json const& data )
    {
        m_Friction = Stream::Read<float>( data );
    }

    /// @brief the map of read methods for StaticBodys
    ReadMethodMap< StaticBody > StaticBody::s_ReadMethods = {
        { "Restitution", &readRestitution },
        { "Friction"   , &readFriction    }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  Write all StaticBody component data to a JSON file.
    /// @return The JSON file containing the StaticBody component data.
    nlohmann::ordered_json StaticBody::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Restitution" ] = m_Restitution;
        json[ "Friction" ] = m_Friction;

        return json;
    }

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the StaticBody
    /// @param  other   the other StaticBody to copy
    StaticBody::StaticBody( const StaticBody& other ) :
        Component( other ),
        m_Restitution( other.m_Restitution ),
        m_Friction( other.m_Friction )
    {}

//-----------------------------------------------------------------------------
