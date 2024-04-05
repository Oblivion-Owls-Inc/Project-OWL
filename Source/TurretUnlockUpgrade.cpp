/// @file       TurretUnlockUpgrade.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      collectable upgrade which unlocks a turret
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h"

#include "TurretUnlockUpgrade.h"

#include "ComponentReference.t.h"
#include "ConstructionBehavior.h"
#include "Interactable.h"

// includes required indirectly from other Components
#include "Inventory.h"
#include "Tilemap.h"
#include "ResourcesUiManager.h"
#include "Sprite.h"
#include "AudioPlayer.h"
#include "Popup.h"
#include "UiElement.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    TurretUnlockUpgrade::TurretUnlockUpgrade() :
        Component( typeid( TurretUnlockUpgrade ) )
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
    void TurretUnlockUpgrade::OnInit()
    {
        m_Interactable.SetOnConnectCallback( [ this ]() {
            m_Interactable->AddOnInteractCallback( GetId(), [ this ]( Interactor* ) {
                unlockBuilding();
                GetEntity()->Destroy();
            } );
        } );
        m_Interactable.SetOnDisconnectCallback( [ this ]() {
            m_Interactable->RemoveOnInteractCallback( GetId() );
        } );

        m_Interactable.Init( GetEntity() );

        m_ConstructionEntity.SetOwnerName( GetName() );
        m_ConstructionEntity.Init();
    }

    /// @brief  called once when exiting the scene
    void TurretUnlockUpgrade::OnExit()
    {
        m_Interactable.Exit();

        m_ConstructionEntity.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  unlocks this upgrade's building
    void TurretUnlockUpgrade::unlockBuilding()
    {
        if ( m_ConstructionBehavior == nullptr )
        {
            return;
        }

        m_ConstructionBehavior->SetBuildingUnlocked( m_UnlockBuildingIndex, true );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for TurretUnlockUpgrade
    void TurretUnlockUpgrade::Inspector()
    {
        m_ConstructionEntity.Inspect( "Construction Entity" );

        ImGui::NewLine();

        ImGui::DragInt( "Building Index", &m_UnlockBuildingIndex, 0.05f, 0, INT_MAX );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the index of the building to unlock
    void TurretUnlockUpgrade::readUnlockBuildingIndex( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_UnlockBuildingIndex, data );
    }

    /// @brief  reads the Entity the ConstructionBehavior is attached to
    void TurretUnlockUpgrade::readConstructionEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ConstructionEntity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this TurretUnlockUpgrade
    /// @return the map of read methods for this TurretUnlockUpgrade
    ReadMethodMap< ISerializable > const& TurretUnlockUpgrade::GetReadMethods() const
    {
        static ReadMethodMap< TurretUnlockUpgrade > const readMethods = {
            { "UnlockBuildingIndex", &TurretUnlockUpgrade::readUnlockBuildingIndex }, 
            { "ConstructionEntity" , &TurretUnlockUpgrade::readConstructionEntity  }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this TurretUnlockUpgrade to JSON
    /// @return the JSON data of this TurretUnlockUpgrade
    nlohmann::ordered_json TurretUnlockUpgrade::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "UnlockBuildingIndex" ] = Stream::Write( m_UnlockBuildingIndex );
        json[ "ConstructionEntity"  ] = Stream::Write( m_ConstructionEntity  );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TurretUnlockUpgrade
    /// @return the newly created clone of this TurretUnlockUpgrade
    TurretUnlockUpgrade* TurretUnlockUpgrade::Clone() const
    {
        return new TurretUnlockUpgrade( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TurretUnlockUpgrade
    /// @param  other   the other TurretUnlockUpgrade to copy
    TurretUnlockUpgrade::TurretUnlockUpgrade( TurretUnlockUpgrade const& other ) :
        Component( other ),
        m_UnlockBuildingIndex( m_UnlockBuildingIndex ),
        m_ConstructionEntity ( m_ConstructionEntity, { &m_ConstructionBehavior } )
    {}


//-----------------------------------------------------------------------------
