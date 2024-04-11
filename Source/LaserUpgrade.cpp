/// @file       LaserUpgrade.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      collectable upgrade to the player's MiningLaser
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h"

#include "LaserUpgrade.h"

#include "ComponentReference.t.h"
#include "MiningLaser.h"
#include "Interactable.h"

// needed indirectly by other included Components
#include "Tilemap.h"
#include "Sprite.h"



//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    LaserUpgrade::LaserUpgrade() :
        Component( typeid( LaserUpgrade ) )
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
    void LaserUpgrade::OnInit()
    {
        m_Interactable.SetOnConnectCallback( [ this ]() {
            m_Interactable->AddOnInteractCallback( GetId(), [ this ]( Interactor* ) {
                upgradeLaser();
                GetEntity()->Destroy();
            } );
        } );
        m_Interactable.SetOnDisconnectCallback( [ this ]() {
            m_Interactable->RemoveOnInteractCallback( GetId() );
        } );
        
        m_Interactable.Init( GetEntity() );

        m_MiningLaserEntity.SetOwnerName( GetName() );
        m_MiningLaserEntity.Init();

    }

    /// @brief  called once when exiting the scene
    void LaserUpgrade::OnExit()
    {
        m_Interactable.Exit();

        m_MiningLaserEntity.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  upgrades the laser
    void LaserUpgrade::upgradeLaser()
    {
        if ( m_MiningLaser == nullptr )
        {
            return;
        }

        m_MiningLaser->SetMiningSpeed ( m_MiningLaser->GetMiningSpeed () * m_SpeedMultiplier      );
        m_MiningLaser->SetDamageRate  ( m_MiningLaser->GetDamageRate  () * m_DamageMultiplier     );

        m_MiningLaser->SetMaxToughness( m_MiningLaser->GetMaxToughness() + m_MaxToughnessIncrease );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for LaserUpgrade
    void LaserUpgrade::Inspector()
    {
        m_MiningLaserEntity.Inspect( "Mining Laser Entity" );

        ImGui::DragFloat( "mining speed multiplier", &m_SpeedMultiplier     , 0.05f, 1.0f, INFINITY );
        ImGui::DragFloat( "damage rate multiplier" , &m_DamageMultiplier    , 0.05f, 1.0f, INFINITY );
        ImGui::DragFloat( "max toughness increase" , &m_MaxToughnessIncrease, 0.05f, 0.0f, INFINITY );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  how much the mining speed is multiplied by
    /// @param  data    the JSON data to read from
    void LaserUpgrade::readSpeedMultiplier( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_SpeedMultiplier, data );
    }

    /// @brief  reads how much the damage of the mining laser is multiplied by
    /// @param  data    the JSON data to read from
    void LaserUpgrade::readDamageMultiplier( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_DamageMultiplier, data );
    }

    /// @brief  how much the max toughness the laser can mine is increased by
    /// @param  data    the JSON data to read from
    void LaserUpgrade::readMaxToughnessIncrease( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MaxToughnessIncrease, data );
    }

    /// @brief  the Entity the MiningLaser is attached to
    /// @param  data    the JSON data to read from
    void LaserUpgrade::readMiningLaserEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MiningLaserEntity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this LaserUpgrade
    /// @return the map of read methods for this LaserUpgrade
    ReadMethodMap< ISerializable > const& LaserUpgrade::GetReadMethods() const
    {
        static ReadMethodMap< LaserUpgrade > const readMethods = {
            { "SpeedMultiplier"     , &LaserUpgrade::readSpeedMultiplier      },
            { "DamageMultiplier"    , &LaserUpgrade::readDamageMultiplier     },
            { "MaxToughnessIncrease", &LaserUpgrade::readMaxToughnessIncrease },
            { "MiningLaserEntity"   , &LaserUpgrade::readMiningLaserEntity    }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this LaserUpgrade to JSON
    /// @return the JSON data of this LaserUpgrade
    nlohmann::ordered_json LaserUpgrade::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "SpeedMultiplier"      ] = Stream::Write( m_SpeedMultiplier      );
        json[ "DamageMultiplier"     ] = Stream::Write( m_DamageMultiplier     );
        json[ "MaxToughnessIncrease" ] = Stream::Write( m_MaxToughnessIncrease );
        json[ "MiningLaserEntity"    ] = Stream::Write( m_MiningLaserEntity    );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this LaserUpgrade
    /// @return the newly created clone of this LaserUpgrade
    LaserUpgrade* LaserUpgrade::Clone() const
    {
        return new LaserUpgrade( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the LaserUpgrade
    /// @param  other   the other LaserUpgrade to copy
    LaserUpgrade::LaserUpgrade( LaserUpgrade const& other ) :
        Component( other ),
        m_SpeedMultiplier     ( other.m_SpeedMultiplier      ),
        m_DamageMultiplier    ( other.m_DamageMultiplier     ),
        m_MaxToughnessIncrease( other.m_MaxToughnessIncrease ),
        m_MiningLaserEntity   ( other.m_MiningLaserEntity, { &m_MiningLaser } )
    {}


//-----------------------------------------------------------------------------
