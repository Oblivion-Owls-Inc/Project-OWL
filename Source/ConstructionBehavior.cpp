/// @file       ConstructionBehavior.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the construction of towers and modification of terrain
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "ConstructionBehavior.h"

#include "Transform.h"
#include "Tilemap.h"
#include "Sprite.h"

#include "AssetLibrarySystem.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "InputSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    ConstructionBehavior::ConstructionBehavior() :
        Behavior( typeid( ConstructionBehavior ) )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called once when entering the scene
    void ConstructionBehavior::OnInit()
    {
        Behaviors< Behavior >()->AddBehavior( this );
        m_PlayerTransform = Entities()->GetEntity( m_PlayerName )->GetComponent< Transform >();
        m_Tilemap = Entities()->GetEntity( m_TilemapName )->GetComponent< Tilemap< int > >();
        m_Transform = GetParent()->GetComponent<Transform>();
        m_Sprite = GetParent()->GetComponent<Sprite>();
    }

    /// @brief  called once when exiting the scene
    void ConstructionBehavior::OnExit()
    {
        Behaviors< Behavior >()->RemoveBehavior( this );
    }

    /// @brief  called every simulation frame
    void ConstructionBehavior::OnFixedUpdate()
    {
        for ( int i = 0; i <= m_BuildingArchetypes.size() ; ++i )
        {
            if ( Input()->GetKeyReleased( GLFW_KEY_0 + i ) )
            {
                m_BuildingIndex = i - 1;
                if ( i != 0 )
                {
                    m_Sprite->SetTexture( m_BuildingArchetypes[ i - 1 ]->GetComponent<Sprite>()->GetTexture() );
                }
            }
        }

        if ( m_BuildingIndex == -1 )
        {
            return;
        }

        // TODO: check if we have enough money

        glm::mat4 const& worldToTile = m_Tilemap->GetWorldToTilemapMatrix();
        glm::vec2 mousePos = Input()->GetMousePosWorld();
        glm::ivec2 tilePos = worldToTile * glm::vec4( mousePos, 0, 1 );

        glm::mat4 const& tileToWorld = m_Tilemap->GetTilemapToWorldMatrix();
        glm::vec2 buildPos = tileToWorld * glm::vec4( (glm::vec2)tilePos + glm::vec2( 0.5f ), 0, 1 );

        glm::vec2 offset = buildPos - m_PlayerTransform->GetTranslation();
        float distanceSquared = glm::dot( offset, offset );

        m_Transform->SetTranslation( buildPos );

        if ( distanceSquared <= m_PlacementRange * m_PlacementRange )
        {
            m_Sprite->SetOpacity( 0.5f );

            if (
                tilePos.x < 0 || tilePos.x >= m_Tilemap->GetTilemapWidth() ||
                tilePos.y < 0 || tilePos.y >= m_Tilemap->GetTilemapHeight() ||
                m_Tilemap->GetTile( tilePos ) != 0
            )
            {
                m_Sprite->SetColor( { 0.5f, 0, 0.0f, 0 } );
                return;
            }

            // TODO: pointcast to ensure ovarlapping turrets are prevented

            m_Sprite->SetColor( { 0, 0.5f, 0, 0 } );

            if ( Input()->GetMouseTriggered( GLFW_MOUSE_BUTTON_2 ) )
            {
                Entity* turret = m_BuildingArchetypes[ m_BuildingIndex ]->Clone();
                turret->GetComponent<Transform>()->SetTranslation( buildPos );
                Entities()->AddEntity( turret );
            }
        }
        else
        {
            offset += mousePos - buildPos;
            float distance = std::sqrt( glm::dot( offset, offset ) );
            m_Sprite->SetColor( { 0.5f, 0, 0, 0 } );
            m_Sprite->SetOpacity( std::lerp( 0.5f, 0.0f, std::clamp( distance - m_PlacementRange, 0.0f, 1.0f ) ) );
        }
    }

    /// @brief  displays this ConstructionBehavior in the Inspector
    void ConstructionBehavior::Inspector()
    {
        // TODO: ConstructionBevior::Inspector()
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  read the buildings
    /// @param  data    the json data to read from
    void ConstructionBehavior::readBuildings( nlohmann::ordered_json const& data )
    {
        for ( nlohmann::json const& building : data )
        {
            m_BuildingArchetypes.push_back( AssetLibrary< Entity >()->GetAsset( building[ "Name" ] ) );
            m_BuildingCosts.push_back( building[ "Cost" ] );
        }
    }

    /// @brief  read the tilemap name
    /// @param  data    the json data to read from
    void ConstructionBehavior::readTilemapName( nlohmann::ordered_json const& data )
    {
        m_TilemapName = data;
    }

    /// @brief  read the player name
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPlayerName( nlohmann::ordered_json const& data )
    {
        m_PlayerName = data;
    }

    /// @brief  read the player name
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPlacementRange( nlohmann::ordered_json const& data )
    {
        m_PlacementRange = data;
    }

    /// @brief  read the building index
    /// @param  data    the json data to read from
    void ConstructionBehavior::readBuildingIndex( nlohmann::ordered_json const& data )
    {
        m_BuildingIndex = data;
    }

    /// @brief  map of the read methods for this Component
    ReadMethodMap< ConstructionBehavior > ConstructionBehavior::s_ReadMethods = {
        { "Buildings"     , &readBuildings      },
        { "TilemapName"   , &readTilemapName    },
        { "PlayerName"    , &readPlayerName     },
        { "PlacementRange", &readPlacementRange },
        { "BuildingIndex" , &readBuildingIndex  }
    };

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  Write all ConstructionBehavior data to a JSON file.
    /// @return The JSON file containing the ConstructionBehavior data.
    nlohmann::ordered_json ConstructionBehavior::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& buildings = json[ "Buildings" ];
        for ( int i = 0; i < m_BuildingArchetypes.size(); ++i )
        {
            buildings[ i ][ "Name" ] = AssetLibrary< Entity >()->GetAssetName( m_BuildingArchetypes[ i ] );
            buildings[ i ][ "Cost" ] = m_BuildingCosts[ i ];
        }

        json[ "PlayerName" ] = m_PlayerName;
        json[ "TilemapName" ] = m_TilemapName;
        json[ "PlacementRange" ] = m_PlacementRange;
        json[ "BuildingIndex" ] = m_BuildingIndex;

        return json;
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the ConstructionBehavior
    /// @param  other   the other ConstructionBehavior to copy
    ConstructionBehavior::ConstructionBehavior( const ConstructionBehavior& other ) :
        Behavior( other ),
        m_BuildingArchetypes( other.m_BuildingArchetypes ),
        m_BuildingCosts( other.m_BuildingCosts ),
        m_PlayerName( other.m_PlayerName ),
        m_TilemapName( other.m_TilemapName )
    {}

//-----------------------------------------------------------------------------
