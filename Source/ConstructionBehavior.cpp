/// @file       ConstructionBehavior.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      handles the construction of towers and modification of terrain
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once


#include "pch.h" 
#include "ConstructionBehavior.h"


#include "ComponentReference.t.h"
#include "Transform.h"
#include "Sprite.h"
#include "AudioPlayer.h"
#include "Inventory.h"
#include "Tilemap.h"
#include "TurretBehavior.h"
#include "ResourcesUiManager.h"
#include "UiElement.h"
#include "Generator.h"
#include "Popup.h"


#include "AssetLibrarySystem.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "InputSystem.h"
#include "BehaviorSystem.h"
#include "CameraSystem.h"

#include "Engine.h"

#include "Inspection.h"




//-----------------------------------------------------------------------------
// private: class BuildingInfo
//-----------------------------------------------------------------------------


    //-------------------------------------------------------------------------
    // public: methods
    //-------------------------------------------------------------------------


        /// @brief  initializes this BuildingInfo
        void ConstructionBehavior::BuildingInfo::Init()
        {
            M_Archetype.SetOwnerName( "ConstructionBehavior" );
            M_Archetype.Init();
            M_SelectAction.Init();
        }

        /// @brief  exits this BuildingInfo
        void ConstructionBehavior::BuildingInfo::Exit()
        {
            M_SelectAction.Exit();
        }


    //-------------------------------------------------------------------------
    // public: inspection
    //-------------------------------------------------------------------------


        /// @brief  inspects this BuildingInfo
        /// @return whether this BuildingInfo changed
        bool ConstructionBehavior::BuildingInfo::Inspect()
        {
            bool changed = false;

            changed |= M_Archetype.Inspect( "building prefab" );

            changed |= Inspection::InspectArray< ItemStack >(
                "building cost", &M_Cost,
                []( ItemStack* itemStack ) -> bool
                {
                    return itemStack->Inspect();
                }
            );

            changed |= M_SelectAction.Inspect("Selection Action");

            changed |= ImGui::Checkbox( "unlocked", &M_Unlocked );

            return changed;
        }

        
    //-------------------------------------------------------------------------
    // private: reading
    //-------------------------------------------------------------------------

        
        /// @brief  reads the name of the archetype
        /// @param  data    the json data to read from
        void ConstructionBehavior::BuildingInfo::readArchetype( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_Archetype, data );
        }

        /// @brief  reads the cost of the building
        /// @param  data    the json data to read from
        void ConstructionBehavior::BuildingInfo::readCost( nlohmann::ordered_json const& data )
        {
            Stream::Read< ItemStack >( &M_Cost, data );
        }

        /// @brief  the control Action to select this building
        /// @param  data    the JSON data to read from
        void ConstructionBehavior::BuildingInfo::readSelectAction( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_SelectAction, data );
        }

        /// @brief  reads whether this building is unlocked
        /// @param  data    the JSON data to read from
        void ConstructionBehavior::BuildingInfo::readUnlocked( nlohmann::ordered_json const& data )
        {
            Stream::Read( M_Unlocked, data );
        }


    //-------------------------------------------------------------------------
    // public: reading / writing
    //-------------------------------------------------------------------------


        /// @brief  gets the map of read methods for this ConstructionBehavior
        /// @return the map of read methods for this ConstructionBehavior
        ReadMethodMap< ISerializable > const& ConstructionBehavior::BuildingInfo::GetReadMethods() const
        {
            static ReadMethodMap< BuildingInfo > const readMethods = {
                { "Archetype"   , &BuildingInfo::readArchetype    },
                { "Cost"        , &BuildingInfo::readCost         },
                { "SelectAction", &BuildingInfo::readSelectAction },
                { "Unlocked"    , &BuildingInfo::readUnlocked     }
            };

            return (ReadMethodMap< ISerializable > const&)readMethods;
        }


        /// @brief  Write all ConstructionBehavior data to a JSON file.
        /// @return The JSON file containing the ConstructionBehavior data.
        nlohmann::ordered_json ConstructionBehavior::BuildingInfo::Write() const
        {
            nlohmann::ordered_json json;

            json[ "Archetype"    ] = Stream::Write( M_Archetype    );
            json[ "Cost"         ] = Stream::WriteArray( M_Cost );
            json[ "SelectAction" ] = Stream::Write( M_SelectAction );
            json[ "Unlocked"     ] = Stream::Write( M_Unlocked     );

            return json;
        }


    //-------------------------------------------------------------------------
    
//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  constructor
    ConstructionBehavior::ConstructionBehavior() :
        Behavior( typeid( ConstructionBehavior ) )
    {}


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the placement range
    /// @return the placement range
    float ConstructionBehavior::GetPlacementRange() const
    {
        return m_PlacementRange;
    }

    /// @brief  sets the placement range
    /// @param  range   the placement range
    void ConstructionBehavior::SetPlacementRange( float range )
    {
        m_PlacementRange = range;
    }


    /// @brief  gets the building index
    /// @return the building index
    int ConstructionBehavior::GetBuildingIndex() const
    {
        return m_BuildingIndex;
    }

    /// @brief  sets the building index
    /// @param  buildingIndex   the building index
    void ConstructionBehavior::SetBuildingIndex( int buildingIndex )
    {
        m_BuildingIndex = buildingIndex;
        if ( buildingIndex == -1 )
        {
            return;
        }

        // update preview sprite
        if ( m_BuildingInfos[ m_BuildingIndex ].M_Archetype == nullptr )
        {
            Debug() << "WARNING: ConstructionManager building archetype is NULL" << std::endl;
            return;
        }

        setupCostUi();

        if ( m_Sprite == nullptr || m_Transform == nullptr )
        {
            return;
        }

        m_Sprite->SetTexture( m_BuildingInfos[ m_BuildingIndex ].M_Archetype->GetComponent< Sprite >()->GetTexture() );
        m_Transform->SetScale( m_BuildingInfos[ m_BuildingIndex ].M_Archetype->GetComponent< Transform >()->GetScale() );
        if (m_RadiusTransform != nullptr)
        {
            if ( m_BuildingInfos[ m_BuildingIndex ].M_Archetype == nullptr )
            {
                m_RadiusTransform->SetScale( glm::vec2( 0.0f ) );
                return;
            }
            TurretBehavior const* turretBehavior = m_BuildingInfos[ m_BuildingIndex ].M_Archetype->GetComponent< TurretBehavior >();
            float scale = turretBehavior == nullptr ? 0 : turretBehavior->GetRange() * 2;
            m_RadiusTransform->SetScale( glm::vec2( scale ) );
        }
    }


    /// @brief  gets whether buildings should be able to be constructed for free
    /// @return whether buildings should be able to be constructed for free
    bool ConstructionBehavior::GetIgnoreCosts() const
    {
        return m_IgnoreCosts;
    }

    /// @brief  sets whether buildings should be able to be constructed for free
    /// @param  ignoreCosts whether buildings should be able to be constructed for free
    void ConstructionBehavior::SetIgnoreCosts( bool ignoreCosts )
    {
        m_IgnoreCosts = ignoreCosts;
    }

    
//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  checks whether the player can afford the specified building
    /// @param  buildingIndex   the index of the building to check whether can be afforded
    /// @return whether the building can be afforded
    bool ConstructionBehavior::CanAffordBuilding( int buildingIndex ) const
    {
        if ( m_PlayerInventory == nullptr || buildingIndex < 0 || buildingIndex >= m_BuildingInfos.size() )
        {
            return false;
        }

        return m_IgnoreCosts || m_PlayerInventory->ContainsItemStacks( m_BuildingInfos[ buildingIndex ].M_Cost );
    }


    /// @brief  gets whether the specified building is unlocked
    /// @param  buildingIndex   the index of the building to check if is locked
    /// @return whether the specified building is unlocked
    bool ConstructionBehavior::BuildingIsUnlocked( int buildingIndex ) const
    {
        if ( buildingIndex < 0 || buildingIndex >= m_BuildingInfos.size() )
        {
            return false;
        }

        return m_BuildingInfos[ buildingIndex ].M_Unlocked;
    }

    /// @brief  sets whether the specified building is unlocked
    /// @param  buildingIndex   the index of the building to set whether is locked
    /// @param  unlocked        whether the building should be unlocked
    void ConstructionBehavior::SetBuildingUnlocked( int buildingIndex, bool unlocked )
    {
        if ( buildingIndex < 0 || buildingIndex >= m_BuildingInfos.size() )
        {
            return;
        }

        m_BuildingInfos[ buildingIndex ].M_Unlocked = unlocked;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called once when entering the scene
    void ConstructionBehavior::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_PlayerEntity .SetOwnerName( GetName() );
        m_TilemapEntity.SetOwnerName( GetName() );
        m_CostUiEntity .SetOwnerName( GetName() );
        m_PlayerEntity .Init();
        m_TilemapEntity.Init();
        m_CostUiEntity .Init();

        m_Transform           .Init( GetEntity() );
        m_Sprite              .Init( GetEntity() );
        m_TurretPlacementSound.Init( GetEntity() );
        m_CostInventory       .Init( GetEntity() );
        m_Popup               .Init( GetEntity() );

        m_PlaceAction    .SetOwnerName( GetName() );
        m_PlaceAction    .Init();

        if (GetEntity()->GetChildren().size() != 0)
        {
            m_RadiusSprite   .Init( GetEntity()->GetChildren()[ 0 ] );
            m_RadiusTransform.Init( GetEntity()->GetChildren()[ 0 ] );
        }
        else
        {
            Debug() << "WARNING: no children attached to ConstructionBehavior to display preview radius" << std::endl;
        }

        for ( BuildingInfo& buildingInfo : m_BuildingInfos )
        {
            buildingInfo.M_SelectAction.SetOwnerName(GetName());
            buildingInfo.Init();
        }
    }

    /// @brief  called once when exiting the scene
    void ConstructionBehavior::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_PlayerEntity .Exit();
        m_TilemapEntity.Exit();
        m_CostUiEntity .Exit();

        m_Transform           .Exit();
        m_Sprite              .Exit();
        m_TurretPlacementSound.Exit();
        m_CostInventory       .Exit();
        m_Popup               .Exit();

        m_RadiusSprite        .Exit();
        m_RadiusTransform     .Exit();

        m_PlaceAction.Exit();

        for (BuildingInfo& buildingInfo : m_BuildingInfos)
        {
            buildingInfo.Exit();
        }
    }

    /// @brief  called every simulation frame
    void ConstructionBehavior::OnFixedUpdate()
    {
        if ( m_Tilemap == nullptr )
        {
            return;
        }

        updateTargetLocation();
        updateSelectedBuilding();

        tryPlaceBuilding();

        showBuildingPreview();
    }


    /// @brief  called whenever a child is added to this Entity
    /// @param  newChild    the child that was added
    void ConstructionBehavior::OnAddChild(Entity* child)
    {
        if (m_RadiusSprite.GetEntity() != nullptr)
        {
            return;
        }
        m_RadiusSprite.Init(child);
        m_RadiusTransform.Init(child);
    }

    /// @brief  called whenever a child is about to be removed from this Entity
    /// @param  newChild    the child that is about to be removed
    void ConstructionBehavior::OnRemoveChild(Entity* child)
    {
        if (m_RadiusSprite.GetEntity() == child)
        {
            m_RadiusSprite.Exit();
            m_RadiusTransform.Exit();
        }
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
        for ( int i = 0; i < m_BuildingInfos.size(); ++i )
        {
            if (
                m_BuildingInfos[ i ].M_Unlocked == false ||
                m_BuildingInfos[ i ].M_SelectAction == nullptr ||
                m_BuildingInfos[ i ].M_SelectAction->GetReleased() == false
            )
            {
                continue;
            }

            if ( m_Popup != nullptr )
            {
                m_Popup->SetOpen( true );
            }

            SetBuildingIndex( i );

            return;
        }

        if ( m_Popup != nullptr && m_Popup->GetOpen() == false )
        {
            m_BuildingIndex = -1;
        }
    }


    /// @brief  tries to place the currently selected building
    void ConstructionBehavior::tryPlaceBuilding()
    {
        // skip if build button not pressed
        if ( m_PlaceAction != nullptr && m_PlaceAction->GetTriggered() == false)
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

        // building is locked
        if ( BuildingIsUnlocked( m_BuildingIndex ) == false )
        {
            return false;
        }

        // not enough funds
        if ( CanAffordBuilding( m_BuildingIndex ) == false )
        {
            return false;
        }

        // target outside of tilemap
        if ( m_Tilemap->IsPositionWithinBounds( m_TargetTilePos ) == false )
        {
            return false;
        }

        // target overlaps a tile
        if ( m_Tilemap->GetTile( m_TargetTilePos ) != -1 )
        {
            return false;
        }

        // target overlaps a building
        if ( m_Buildings->GetTile( m_TargetTilePos ) != nullptr )
        {
            return false;
        }

        // target pos out of range
        if ( m_PlayerTransform == nullptr || glm::distance( m_PlayerTransform->GetTranslation(), m_TargetPos ) > m_PlacementRange )
        {
            return false;
        }

        for (Generator* generator : Behaviors< Generator >()->GetComponents())
        {
            float distance = glm::distance(
                generator->GetTransform()->GetTranslation(),
                m_TargetPos
            );
            if (distance <= generator->GetPowerRadius())
            {
                return true;
            }
        }

        return false;
    }

    /// @brief  palces the currently selected building
    void ConstructionBehavior::placeBuilding()
    {
        Entity* building = m_BuildingInfos[ m_BuildingIndex ].M_Archetype->Clone();

        Transform* buildingTransform = building->GetComponent< Transform >();
        if ( buildingTransform == nullptr )
        {
            return;
        }
        buildingTransform->SetTranslation( m_TargetPos );
        
        building->AddToScene();
        
        if ( m_IgnoreCosts == false )
        {
            m_PlayerInventory->RemoveItemStacks( m_BuildingInfos[ m_BuildingIndex ].M_Cost );
        }

        if ( m_TurretPlacementSound == nullptr )
        {
            Debug() << "Warning: ConstructionBehavior does not have TurretPlacementSound" << std::endl;
            return;
        }

        // TODO: give audiovisual feedback that the building was placed
        m_TurretPlacementSound->Play();
    }


    /// @brief  displays the building preview
    void ConstructionBehavior::showBuildingPreview()
    {

        float distance = m_PlayerTransform == nullptr ? INFINITY : glm::distance( m_PlayerTransform->GetTranslation(), m_TargetPos );

        // no preview if out of range or no building selected
        if ( m_BuildingIndex == -1 || distance >= m_PlacementRange + m_PreviewFadeOutRadius )
        {
            if ( m_Sprite != nullptr )
            {
                m_Sprite->SetOpacity( 0.0f );
            }
            if ( m_RadiusSprite != nullptr )
            {
                m_RadiusSprite->SetOpacity( 0.0f );
            }
            if ( m_CostResourcesUiManager != nullptr )
            {
                m_CostResourcesUiManager->SetOpacity( 0.0f );
            }
            return;
        }

        m_Transform->SetTranslation( m_TargetPos );
        if (m_RadiusTransform != nullptr)
        {
            m_RadiusTransform->SetTranslation(m_TargetPos);
        }

        moveCostUi();
        
        if ( isCurrentlyPlaceable() )
        {
            if ( m_Sprite != nullptr )
            {
                m_Sprite->SetColor( m_PreviewColorPlaceable );
                m_Sprite->SetOpacity( m_PreviewAlpha );
            }

            if (m_RadiusSprite != nullptr)
            {
                m_RadiusSprite->SetOpacity(m_PreviewAlpha);
            }
            if ( m_CostResourcesUiManager != nullptr )
            {
                m_CostResourcesUiManager->SetOpacity( 1.0f );
            }
        }
        else
        {
            // alpha fades away the further from the player it gets
            float alpha = m_PreviewAlpha * ( 1.0f - ( distance - m_PlacementRange ) / m_PreviewFadeOutRadius );
            alpha = std::min( alpha, m_PreviewAlpha );

            if ( m_Sprite != nullptr )
            {
                m_Sprite->SetColor( m_PreviewColorNonPlaceable );
                m_Sprite->SetOpacity( alpha );
            }
            if (m_RadiusSprite != nullptr)
            {
                m_RadiusSprite->SetOpacity(alpha / 2);
            }
            if ( m_CostResourcesUiManager != nullptr )
            {
                m_CostResourcesUiManager->SetOpacity( alpha / m_PreviewAlpha );
            }
        }

    }


    /// @brief  sets up the cost Ui with the currently selected turret's cost
    void ConstructionBehavior::setupCostUi()
    {
        if ( m_CostInventory == nullptr )
        {
            return;
        }

        m_CostInventory->Clear();

        if ( m_BuildingIndex == -1 )
        {
            return;
        }

        m_CostInventory->AddItemStacks( m_BuildingInfos[ m_BuildingIndex ].M_Cost );
    }

    /// @brief  moves the cost Ui to align with the mouse
    void ConstructionBehavior::moveCostUi()
    {
        if ( m_CostResourcesUiManager == nullptr || m_CostResourcesUiManager->GetUiElement() == nullptr )
        {
            return;
        }

        m_CostResourcesUiManager->GetUiElement()->SetAnchor(
            Cameras()->GetMat_WorldToClip() * glm::vec4( m_TargetPos, 0.0f, 1.0f )
        );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


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
                return buildingInfo->Inspect();
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

        ImGui::ColorEdit4( "Preview Color - Placeable", &m_PreviewColorPlaceable[ 0 ] );

        ImGui::ColorEdit4( "Preview Color - NonPlaceable", &m_PreviewColorNonPlaceable[ 0 ] );

        ImGui::DragFloat( "Preview Alpha", &m_PreviewAlpha, 0.05f, 0.0f, 1.0f );

        m_PlaceAction.Inspect("Place Action");
    }

    /// @brief  inspects the references to other entities
    void ConstructionBehavior::inspectEntityReferences()
    {

        m_PlayerEntity.Inspect( "player entity" );
        
        m_TilemapEntity.Inspect( "tilemap entity" );

        m_CostUiEntity.Inspect( "Cost Ui Entity" );

    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  read the buildings
    /// @param  data    the json data to read from
    void ConstructionBehavior::readBuildings( nlohmann::ordered_json const& data )
    {
        m_BuildingInfos.clear();
        for ( nlohmann::json const& buildingData : data )
        {
            BuildingInfo buildingInfo;
            Stream::Read( buildingInfo, buildingData );
            m_BuildingInfos.push_back( buildingInfo );
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


    /// @brief  read the tilemap entity
    /// @param  data    the json data to read from
    void ConstructionBehavior::readTilemapEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TilemapEntity, data );
    }

    /// @brief  read the player entity
    /// @param  data    the json data to read from
    void ConstructionBehavior::readPlayerEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_PlayerEntity, data );
    }

    /// @brief  read the cost ui entity
    /// @param  data    the json data to read from
    void ConstructionBehavior::readCostUiEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_CostUiEntity, data );
    }

    /// @brief  the control Action to place a building
    /// @param  data    the JSON data to read from
    void ConstructionBehavior::readPlaceAction(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_PlaceAction, data);
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& ConstructionBehavior::GetReadMethods() const
    {
        static ReadMethodMap< ConstructionBehavior > const readMethods = {
            { "Buildings"               , &ConstructionBehavior::readBuildings                },
            { "BuildingIndex"           , &ConstructionBehavior::readBuildingIndex            },
            { "PlacementRange"          , &ConstructionBehavior::readPlacementRange           },
            { "PreviewFadeOutRadius"    , &ConstructionBehavior::readPreviewFadeOutRadius     },
            { "PreviewColorPlaceable"   , &ConstructionBehavior::readPreviewColorPlaceable    },
            { "PreviewColorNonPlaceable", &ConstructionBehavior::readPreviewColorNonPlaceable },
            { "PreviewAlpha"            , &ConstructionBehavior::readPreviewAlpha             },
            { "TilemapEntity"           , &ConstructionBehavior::readTilemapEntity            },
            { "PlayerEntity"            , &ConstructionBehavior::readPlayerEntity             },
            { "CostUiEntity"            , &ConstructionBehavior::readCostUiEntity             },
            { "PlaceAction"             , &ConstructionBehavior::readPlaceAction              },
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------

    /// @brief  Write all ConstructionBehavior data to a JSON file.
    /// @return The JSON file containing the ConstructionBehavior data.
    nlohmann::ordered_json ConstructionBehavior::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& buildingData = json[ "Buildings" ];
        for ( BuildingInfo const& building : m_BuildingInfos )
        {
            buildingData.push_back( Stream::Write( building ) );
        }

        json[ "BuildingIndex"            ] = Stream::Write( m_BuildingIndex            );
        json[ "PlacementRange"           ] = Stream::Write( m_PlacementRange           );
        json[ "PreviewFadeOutRadius"     ] = Stream::Write( m_PreviewFadeOutRadius     );
        json[ "PreviewColorPlaceable"    ] = Stream::Write( m_PreviewColorPlaceable    );
        json[ "PreviewColorNonPlaceable" ] = Stream::Write( m_PreviewColorNonPlaceable );
        json[ "PreviewAlpha"             ] = Stream::Write( m_PreviewAlpha             );
        json[ "TilemapEntity"            ] = Stream::Write( m_TilemapEntity            );
        json[ "PlayerEntity"             ] = Stream::Write( m_PlayerEntity             );
        json[ "CostUiEntity"             ] = Stream::Write( m_CostUiEntity             );
        json[ "PlaceAction"              ] = Stream::Write( m_PlaceAction              );

        return json;
    }

//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ConstructionBehavior
    /// @return the newly created clone of this ConstructionBehavior
    ConstructionBehavior* ConstructionBehavior::Clone() const
    {
        return new ConstructionBehavior( *this );
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

        m_TilemapEntity( other.m_TilemapEntity, { &m_Tilemap, &m_Buildings               } ),
        m_PlayerEntity ( other.m_PlayerEntity , { &m_PlayerTransform, &m_PlayerInventory } ),
        m_CostUiEntity ( other.m_CostUiEntity , { &m_CostResourcesUiManager              } )
    {}


//-----------------------------------------------------------------------------
