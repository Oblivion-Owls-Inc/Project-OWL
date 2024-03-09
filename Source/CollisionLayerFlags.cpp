/// @file       CollisionLayerFlags.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      struct that determines which collision layers to interact with
/// @version    0.1
/// @date       2024-02-16
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h" // precompiled header has to be included first
#include "CollisionLayerFlags.h"

#include "CollisionSystem.h"


//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------

    
    /// @brief  constructor
    /// @param  flags   the bitflags of this CollisionLayerFlags
    CollisionLayerFlags::CollisionLayerFlags( unsigned flags ) :
        m_Flags( flags )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  Checks whether this CollisionLayerFlags contains a collisionLayer
    /// @param  collisionLayer  the layer to check if these flags include
    /// @return whether the given layer is included
    bool CollisionLayerFlags::Includes( unsigned collisionLayer ) const
    {
        return m_Flags & ( 1 << collisionLayer );
    }

    
//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  implicit cast to unsigned
    /// @return the internal bitflags
    CollisionLayerFlags::operator unsigned() const
    {
        return m_Flags;
    }

    
    /// @brief  assignment operator
    /// @param  bitflags    the bitflags to assign to this CollisionLayerFlags
    void CollisionLayerFlags::operator =( unsigned bitflags )
    {
        m_Flags = bitflags;
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this CollisionLayerFlags
    /// @param  label   the label of this Inspector
    /// @return whether a change was made
    bool CollisionLayerFlags::Inspect( char const* label )
    {
        std::vector< std::string > flagNames = Collisions()->GetLayerNames( *this );
        std::string flagNamesString = "";
        if ( flagNames.size() > 0 )
        {
            flagNamesString = flagNames[ 0 ];
            for ( int i = 1; i < flagNames.size(); ++i )
            {
                flagNamesString += ", " + flagNames[ i ];
            }
        }

        bool changed = false;

        if ( ImGui::BeginCombo( label, flagNamesString.c_str() ) )
        {
            for (int i = 0; i < Collisions()->GetLayerNames().size(); ++i)
            {
                if ( ImGui::Selectable( Collisions()->GetLayerNames()[ i ].c_str(), Includes( i ), ImGuiSelectableFlags_DontClosePopups) )
                {
                    m_Flags ^= 1 << i;
                    changed = true;
                }
            }
            ImGui::EndCombo();
        }

        return true;
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the bitflags of which CollisionLayers are included
    /// @param  data    the JSON data to read from
    void CollisionLayerFlags::readBitFlags( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Flags, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this CollisionLayerFlags
    /// @return the map of read methods for this CollisionLayerFlags
    ReadMethodMap< ISerializable > const& CollisionLayerFlags::GetReadMethods() const
    {
        static ReadMethodMap< CollisionLayerFlags > const readMethods = {
            { "Flags", &CollisionLayerFlags::readBitFlags }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all CollisionLayerFlags data to a JSON file.
    /// @return The JSON file containing the CollisionLayerFlags data.
    nlohmann::ordered_json CollisionLayerFlags::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Flags" ] = Stream::Write( m_Flags );

        return json;
    }


//-----------------------------------------------------------------------------
