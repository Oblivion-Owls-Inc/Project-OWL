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
#include "AudioPlayer.h"

#include "AssetLibrarySystem.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "InputSystem.h"
#include "BehaviorSystem.h"

#include "Engine.h"

#include <climits>

#include "Text.h"

#include "Inspection.h"

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

        m_TurretPlacementSound = Entities()->GetEntity( "TurretPlacementSound" )->GetComponent< AudioPlayer >();

        Entity* tilemapEntity = Entities()->GetEntity( m_TilemapName );
        m_Tilemap = tilemapEntity->GetComponent< Tilemap< int > >();
        m_Buildings = tilemapEntity->GetComponent< Tilemap< Entity* > >();

        m_Transform = GetEntity()->GetComponent<Transform>();
        m_Sprite = GetEntity()->GetComponent<Sprite>();

        m_ResourcesText = Entities()->GetEntity( m_ResourcesTextName )->GetComponent< Text >();
    }

    /// @brief  called once when exiting the scene
    void ConstructionBehavior::OnExit()
    {
        Behaviors< Behavior >()->RemoveBehavior( this );
    }

    /// @brief  called every simulation frame
    void ConstructionBehavior::OnFixedUpdate()
    {
        updateTargetLocation();
        updateSelectedBuilding();

        tryPlaceBuilding();

        showBuildingPreview();
        updateResourcesText();
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
// private: helper methods
//-----------------------------------------------------------------------------


    /// @brief  updates the targeted tile location
    void ConstructionBehavior::updateTargetLocation()
    {
        glm::mat4 const& worldToTile = m_Tilemap->GetWorldToTilemapMatrix();
        glm::mat4 const& tileToWorld = m_Tilemap->GetTilemapToWorldMatrix();

        glm::vec2 mousePos = Input()->GetMousePosWorld();
        m_TargetTilePos = worldToTile * glm::vec4( mousePos, 0, 1 );

        glm::vec2 tileCenter = (glm::vec2)m_TargetTilePos + glm::vec2( 0.5f );
        m_TargetPos = tileToWorld * glm::vec4( tileCenter, 0, 1 );
    }

    /// @brief  updates which building is currently selected
    void ConstructionBehavior::updateSelectedBuilding()
    {
        if ( Input()->GetKeyReleased( GLFW_KEY_0 ) )
        {
            m_BuildingIndex = -1;
            return;
        }

        for ( int i = 0; i <= m_BuildingInfos.size() ; ++i )
        {
            if ( Input()->GetKeyReleased( GLFW_KEY_1 + i ) == false )
            {
                continue;
            }

            m_BuildingIndex = i;

            // update preview sprite
            m_Sprite->SetTexture( m_BuildingInfos[ i ].archetype->GetComponent<Sprite>()->GetTexture() );
            m_Transform->SetScale( m_BuildingInfos[ i ].archetype->GetComponent<Transform>()->GetScale() );

            return;
        }
    }


    /// @brief  tries to place the currently selected building
    void ConstructionBehavior::tryPlaceBuilding()
    {
        // skip if build button not pressed
        if ( Input()->GetMouseTriggered( GLFW_MOUSE_BUTTON_2 ) == false )
        {
            return;
        }


        if ( isCurrentlyPlaceable() )
        {
            placeBuilding();
        }
        else
        {
            // TODO: give audiovisual feedback that building couldn't be placed
        }

    }

    /// @brief  checks if the currently selected building is placeable
    bool ConstructionBehavior::isCurrentlyPlaceable() const
    {
        // no building selected
        if ( m_BuildingIndex == -1 )
        {
            return false;
        }

        // not enough funds
        if ( m_BuildingInfos[ m_BuildingIndex ].cost > m_CurrentResources )
        {
            return false;
        }

        // target outside of tilemap
        if ( m_Tilemap->IsPositionWithinBounds( m_TargetTilePos ) == false )
        {
            return false;
        }

        // target overlaps a tile
        if ( m_Tilemap->GetTile( m_TargetTilePos ) != 0 )
        {
            return false;
        }

        // target overlaps a building
        if ( m_Buildings->GetTile( m_TargetTilePos ) != nullptr )
        {
            return false;
        }

        // target pos out of range
        if ( glm::distance( m_PlayerTransform->GetTranslation(), m_TargetPos ) > m_PlacementRange )
        {
            return false;
        }

        return true;
    }

    /// @brief  palces the currently selected building
    void ConstructionBehavior::placeBuilding()
    {
        // TODO: give feedback that the building was placed

        Entity* building = m_BuildingInfos[ m_BuildingIndex ].archetype->Clone();
        building->GetComponent<Transform>()->SetTranslation( m_TargetPos );
        
        Entities()->AddEntity( building );
        m_Buildings->SetTile( m_TargetTilePos, building );
        
        m_TurretPlacementSound->Play();

        m_CurrentResources -= m_BuildingInfos[ m_BuildingIndex ].cost;
    }


    /// @brief  displays the building preview
    void ConstructionBehavior::showBuildingPreview()
    {
        float distance = glm::distance( m_PlayerTransform->GetTranslation(), m_TargetPos );

        // no preview if out of range or no building selected
        if ( m_BuildingIndex == -1 || distance >= m_PlacementRange + m_PreviewFadeOutRadius )
        {
            m_Sprite->SetOpacity( 0.0f );
            return;
        }

        m_Transform->SetTranslation( m_TargetPos );

        if ( isCurrentlyPlaceable() )
        {
            m_Sprite->SetColor( m_PreviewColorPlaceable );
            m_Sprite->SetOpacity( m_PreviewAlpha );
        }
        else
        {
            m_Sprite->SetColor( m_PreviewColorNonPlaceable );
            
            // alpha fades away the further from the player it gets
            float alpha = m_PreviewAlpha * ( 1.0f - ( distance - m_PlacementRange ) / m_PreviewFadeOutRadius );
            alpha = std::min( alpha, m_PreviewAlpha );
            m_Sprite->SetOpacity( alpha );
        }

    }

    /// @brief  updates the resources text UI
    void ConstructionBehavior::updateResourcesText()
    {
        m_ResourcesText->SetText( m_ResourcesTextPrefix + std::to_string( m_CurrentResources ) );
    }


//-----------------------------------------------------------------------------
// private: inspector methods
//-----------------------------------------------------------------------------


    /// @brief  inspector for the building list
    void ConstructionBehavior::inspectBuildingList()
    {
        Inspection::InspectArray< BuildingInfo >(
            "Buildings",
            &m_BuildingInfos,
            []( BuildingInfo* buildingInfo ) -> bool
            {
                bool changed = false;

                ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.35f );
                if ( Inspection::SelectAssetFromLibrary( "Archetype", &buildingInfo->archetype ) )
                {
                    changed = true;
                }

                ImGui::SameLine();
                ImGui::PushItemWidth( ImGui::GetWindowWidth() * 0.1f );
                if ( ImGui::DragInt( "Cost", &buildingInfo->cost, 0.5f, 0, INT_MAX ) )
                {
                    changed = true;
                }

                return changed;
            }
        );
    }

    /// @brief  inspector for basic variables
    void ConstructionBehavior::inspectVariables()
    {
        ImGui::DragInt( "Building Index", &m_BuildingIndex, 0.05f, 0, (int)m_BuildingInfos.size() - 1, "%i", m_BuildingInfos.size() > 1 ? ImGuiSliderFlags_None : ImGuiSliderFlags_NoInput );

        ImGui::DragFloat( "Placement Range", &m_PlacementRange, 0.05f, 0.0f, INFINITY );

        ImGui::DragFloat( "Preview Fade-Out Radius", &m_PreviewFadeOutRadius, 0.05f, 0.0f, INFINITY );

        ImGui::DragInt2( "Target Tile", &m_TargetTilePos[ 0 ], 0, 0, 0, "%i", ImGuiSliderFlags_NoInput );

        ImGui::DragInt2( "Currently Mining Tile", &m_CurrentMiningTilePos[ 0 ], 0, 0, 0, "%i", ImGuiSliderFlags_NoInput );

        ImGui::ColorEdit3( "Preview Color - Placeable", &m_PreviewColorPlaceable[ 0 ] );

        ImGui::ColorEdit3( "Preview Color - NonPlaceable", &m_PreviewColorNonPlaceable[ 0 ] );

        ImGui::DragFloat( "Preview Alpha", &m_PreviewAlpha, 0.05f, 0.0f, 1.0f );

        ImGui::DragInt( "Current Resources", &m_CurrentResources, 0.25f, 0, INT_MAX );

        ImGui::InputText( "Resources Text Prefix", &m_ResourcesTextPrefix );
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

        if ( ImGui::BeginCombo( "Resources Text Entity", m_ResourcesTextName.c_str() ) )
        {
            for ( Entity* entity : Entities()->GetEntities() )
            {
                if ( ImGui::Selectable( entity->GetName().c_str(), m_ResourcesTextName == entity->GetName() ) )
                {
                    m_ResourcesTextName = entity->GetName();
                    m_ResourcesText = entity->GetComponent< Text >();
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
            m_BuildingInfos.push_back( {
                AssetLibrary< Entity >()->GetAsset( building[ "Name" ] ),
                building[ "Cost" ]
            } );
        }
    }

    /// @brief  read the building index
    /// @param  data    the json data to read from
    void ConstructionBehavior::readBuildingIndex( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_BuildingIndex, data );
    }


    /// @brief  read the player name
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPlacementRange( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PlacementRange, data );
    }

    /// @brief  read the preview fade out range
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPreviewFadeOutRadius( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PreviewFadeOutRadius, data );
    }


    /// @brief  reads the preview color - placeable
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPreviewColorPlaceable( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_PreviewColorPlaceable, data );
    }

    /// @brief  reads the preview color - nonplaceable
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPreviewColorNonPlaceable( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_PreviewColorNonPlaceable, data );
    }

    /// @brief  reads the preview alpha
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPreviewAlpha( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PreviewAlpha, data );
    }


    /// @brief  reads the current resources
    /// @param  data    the json data to read from
    void ConstructionBehavior::readCurrentResources( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_CurrentResources, data );
    }


    /// @brief  reads the resources text prefix
    /// @param  data    the json data to read from
    void ConstructionBehavior::readResourcesTextPrefix( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ResourcesTextPrefix, data );
    }


    /// @brief  read the tilemap name
    /// @param  data    the json data to read from
    void ConstructionBehavior::readTilemapName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TilemapName, data );
    }

    /// @brief  read the player name
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPlayerName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PlayerName, data );
    }

    /// @brief  reads the resources text entity name
    /// @param  data    the json data to read from
    void ConstructionBehavior::readResourcesTextName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ResourcesTextName, data );
    }


    /// @brief  map of the read methods for this Component
    ReadMethodMap< ConstructionBehavior > ConstructionBehavior::s_ReadMethods = {
        { "Buildings"               , &readBuildings                },
        { "BuildingIndex"           , &readBuildingIndex            },
        { "PlacementRange"          , &readPlacementRange           },
        { "PreviewFadeOutRadius"    , &readPreviewFadeOutRadius     },
        { "PreviewColorPlaceable"   , &readPreviewColorPlaceable    },
        { "PreviewColorNonPlaceable", &readPreviewColorNonPlaceable },
        { "PreviewAlpha"            , &readPreviewAlpha             },
        { "CurrentResources"        , &readCurrentResources         },
        { "ResourcesTextPrefix"     , &readResourcesTextPrefix      },
        { "TilemapName"             , &readTilemapName              },
        { "PlayerName"              , &readPlayerName               },
        { "ResourcesTextName"       , &readResourcesTextName        }
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
        for ( int i = 0; i < m_BuildingInfos.size(); ++i )
        {
            buildings[ i ][ "Name" ] = AssetLibrary< Entity >()->GetAssetName( m_BuildingInfos[ i ].archetype );
            buildings[ i ][ "Cost" ] = m_BuildingInfos[ i ].cost;
        }

        json[ "BuildingIndex"            ] = Stream::Write( m_BuildingIndex            );
        json[ "PlacementRange"           ] = Stream::Write( m_PlacementRange           );
        json[ "PreviewFadeOutRadius"     ] = Stream::Write( m_PreviewFadeOutRadius     );
        json[ "PreviewColorPlaceable"    ] = Stream::Write( m_PreviewColorPlaceable    );
        json[ "PreviewColorNonPlaceable" ] = Stream::Write( m_PreviewColorNonPlaceable );
        json[ "PreviewAlpha"             ] = Stream::Write( m_PreviewAlpha             );
        json[ "CurrentResources"         ] = Stream::Write( m_CurrentResources         );
        json[ "ResourcesTextPrefix"      ] = Stream::Write( m_ResourcesTextPrefix      );
        json[ "TilemapName"              ] = Stream::Write( m_TilemapName              );
        json[ "PlayerName"               ] = Stream::Write( m_PlayerName               );
        json[ "ResourcesTextName"        ] = Stream::Write( m_ResourcesTextName        );

        return json;
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the ConstructionBehavior
    /// @param  other   the other ConstructionBehavior to copy
    ConstructionBehavior::ConstructionBehavior( const ConstructionBehavior& other ) :
        Behavior( other ),
        m_BuildingInfos            ( other.m_BuildingInfos            ),
        m_BuildingIndex            ( other.m_BuildingIndex            ),
        m_PlacementRange           ( other.m_PlacementRange           ),
        m_PreviewFadeOutRadius     ( other.m_PreviewFadeOutRadius     ),
        m_PreviewColorPlaceable    ( other.m_PreviewColorPlaceable    ),
        m_PreviewColorNonPlaceable ( other.m_PreviewColorNonPlaceable ),
        m_PreviewAlpha             ( other.m_PreviewAlpha             ),
        m_CurrentResources         ( other.m_CurrentResources         ),
        m_ResourcesTextPrefix      ( other.m_ResourcesTextPrefix      ),
        m_TilemapName              ( other.m_TilemapName              ),
        m_PlayerName               ( other.m_PlayerName               ),
        m_ResourcesTextName        ( other.m_ResourcesTextName        )
    {}

//-----------------------------------------------------------------------------
