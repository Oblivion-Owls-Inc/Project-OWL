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

#include "Engine.h"

#include <climits>

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

    Entity* tilemapEntity = Entities()->GetEntity( m_TilemapName );
    m_Tilemap = tilemapEntity->GetComponent< Tilemap< int > >();
    m_Buildings = tilemapEntity->GetComponent< Tilemap< Entity* > >();

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
                m_Transform->SetScale( m_BuildingArchetypes[ i - 1 ]->GetComponent<Transform>()->GetScale() );
            }
        }
    }


    glm::mat4 const& worldToTile = m_Tilemap->GetWorldToTilemapMatrix();
    glm::vec2 mousePos = Input()->GetMousePosWorld();
    glm::ivec2 tilePos = worldToTile * glm::vec4( mousePos, 0, 1 );

    glm::mat4 const& tileToWorld = m_Tilemap->GetTilemapToWorldMatrix();
    glm::vec2 buildPos = tileToWorld * glm::vec4( (glm::vec2)tilePos + glm::vec2( 0.5f ), 0, 1 );

    glm::vec2 offset = buildPos - m_PlayerTransform->GetTranslation();
    float distanceSquared = glm::dot( offset, offset );

    if (
        distanceSquared <= m_PlacementRange * m_PlacementRange &&
        Input()->GetMouseDown( GLFW_MOUSE_BUTTON_1 )
        )
    {

        // TODO: visuals for digging
        // TODO: make it so changing tiles will reset the timer

        if (
            tilePos.x >= 0 && tilePos.x < m_Tilemap->GetDimensions().x &&
            tilePos.y >= 0 && tilePos.y < m_Tilemap->GetDimensions().y &&
            m_Tilemap->GetTile( tilePos ) != 0
            )
        {
            if (
                Input()->GetMouseTriggered( GLFW_MOUSE_BUTTON_1 ) ||
                tilePos != m_CurrentMiningTarget
            )
            {
                m_CurrentMiningTarget = tilePos;
                m_MiningDelay = m_MiningTime;
            }

            m_MiningDelay -= Engine::GetInstance()->GetFixedFrameDuration();

            if ( m_MiningDelay <= 0 )
            {
                m_Tilemap->SetTile( tilePos, 0 );

                // TEMP: gain resources whenever mining a block
                m_CurrentResources += m_MiningResourceGain;

            }
        }
    }

    if ( m_BuildingIndex == -1 )
    {
        m_Sprite->SetOpacity( 0.0f );
        return;            
    }


    m_Transform->SetTranslation( buildPos );


    if (
        distanceSquared <= m_PlacementRange * m_PlacementRange &&
        m_BuildingCosts[ m_BuildingIndex ] <= m_CurrentResources
    )
    {
        m_Sprite->SetOpacity( 0.5f );

        if (
            tilePos.x < 0 || tilePos.x >= m_Tilemap->GetDimensions().x ||
            tilePos.y < 0 || tilePos.y >= m_Tilemap->GetDimensions().y ||
            m_Tilemap->GetTile( tilePos ) != 0 ||
            m_Buildings->GetTile( tilePos ) != nullptr
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
            m_Buildings->SetTile( tilePos, turret );

            m_CurrentResources -= m_BuildingCosts[ m_BuildingIndex ];
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
    inspectBuildingList();

    ImGui::NewLine();

    inspectVariables();

    ImGui::NewLine();

    inspectEntityReferences();
}

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  inspector for the building list
    void ConstructionBehavior::inspectBuildingList()
    {

        ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.6f );
        if ( !ImGui::BeginListBox( "Buildings", ImVec2( 0, 25 * m_BuildingArchetypes.size() + 5 ) ) )
        {
            return;
        }

        for ( int i = 0; i < m_BuildingArchetypes.size(); ++i )
        {
            ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.35f );
            if ( ImGui::BeginCombo( ( std::stringstream() << "Building##" << i ).str().c_str(), m_BuildingArchetypes[i]->GetName().c_str()) )
            {
                for ( auto& [ name, archetype ] : AssetLibrary< Entity >()->GetAssets() )
                {
                    if ( ImGui::Selectable( name.c_str(), archetype == m_BuildingArchetypes[ i ] ) )
                    {
                        m_BuildingArchetypes[ i ] = archetype;
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.1f );
            ImGui::DragInt( (std::stringstream() << "Cost##" << i ).str().c_str(), &m_BuildingCosts[ i ], 0.05f, 0, INT_MAX );

            ImGui::SameLine();
            if ( ImGui::Button(
                ( std::stringstream() << "X##" << i ).str().c_str(),
                ImVec2( 25, 0 )
            ) )
            {
                m_BuildingArchetypes.erase( m_BuildingArchetypes.begin() + i );
                m_BuildingCosts.erase( m_BuildingCosts.begin() + i );
                --i;
                if ( m_BuildingIndex >= m_BuildingArchetypes.size() )
                {
                    m_BuildingIndex = -1;
                }
            }

        }

        ImGui::EndListBox();

        if ( ImGui::BeginCombo( "Add Building", "Select Archetype" ) )
        {
            for ( auto& [ name, archetype ] : AssetLibrary< Entity >()->GetAssets() )
            {
                if ( ImGui::Selectable( name.c_str(), false ) )
                {
                    m_BuildingArchetypes.push_back( archetype );
                    m_BuildingCosts.push_back( 5 );
                }
            }
            ImGui::EndCombo();
        }
    }

    /// @brief  inspector for basic variables
    void ConstructionBehavior::inspectVariables()
    {
        ImGui::DragFloat( "Placement Range", &m_PlacementRange, 0.05f, 0.0f, INFINITY );

        ImGui::DragInt( "Building Index", &m_BuildingIndex, 0.05f, 0, m_BuildingArchetypes.size() - 1, "%i", m_BuildingArchetypes.size() > 1 ? ImGuiSliderFlags_None : ImGuiSliderFlags_NoInput );

        ImGui::DragFloat( "Mining Time", &m_MiningTime, 0.05f, 0.0f, INFINITY );

        ImGui::DragInt( "Current Resources", &m_CurrentResources, 0.25f );
    }

    /// @brief  inspects the references to other entities
    void ConstructionBehavior::inspectEntityReferences()
    {

        if ( ImGui::BeginCombo( "Player Entity", m_PlayerName.c_str() ) )
        {
            for ( Entity const * entity : Entities()->GetEntities() )
            {
                if ( ImGui::Selectable( entity->GetName().c_str(), m_PlayerName == entity->GetName() ) )
                {
                    m_PlayerName = entity->GetName();
                    m_PlayerTransform = entity->GetComponent< Transform >();
                }
            }
            ImGui::EndCombo();
        }
        
        if ( ImGui::BeginCombo( "Tilemap Entity", m_TilemapName.c_str() ) )
        {
            for ( Entity* entity : Entities()->GetEntities() )
            {
                if ( ImGui::Selectable( entity->GetName().c_str(), m_TilemapName == entity->GetName() ) )
                {
                    m_TilemapName = entity->GetName();
                    m_Tilemap = entity->GetComponent< Tilemap<int> >();
                    m_Buildings = entity->GetComponent< Tilemap< Entity* > >();
                }
            }
            ImGui::EndCombo();
        }

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

/// @brief  reads the mining time
/// @param  data    the json data to read from
void ConstructionBehavior::readMiningTime( nlohmann::ordered_json const& data )
{
    Stream::Read( m_MiningTime, data );
}


/// @brief  reads the current resources
/// @param  data    the json data to read from
void ConstructionBehavior::readCurrentResources( nlohmann::ordered_json const& data )
{
    Stream::Read( m_CurrentResources, data );
}

/// @brief  map of the read methods for this Component
ReadMethodMap< ConstructionBehavior > ConstructionBehavior::s_ReadMethods = {
    { "Buildings"       , &readBuildings         },
    { "TilemapName"     , &readTilemapName       },
    { "PlayerName"      , &readPlayerName        },
    { "PlacementRange"  , &readPlacementRange    },
    { "BuildingIndex"   , &readBuildingIndex     },
    { "MiningTime"      , &readMiningTime        },
    { "CurrentResources", &readCurrentResources  }
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

    json[ "PlayerName" ] = Stream::Write( m_PlayerName );
    json[ "TilemapName" ] = Stream::Write( m_TilemapName );
    json[ "PlacementRange" ] = Stream::Write( m_PlacementRange );
    json[ "BuildingIndex" ] = Stream::Write( m_BuildingIndex );
    json[ "MiningTime" ] = Stream::Write( m_MiningTime );
    json[ "CurrentResources" ] = Stream::Write( m_CurrentResources );

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
