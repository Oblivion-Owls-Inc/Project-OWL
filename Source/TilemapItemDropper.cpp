/// @file       TilemapItemDropper.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Drops an item whenever a tile is broken
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once


#include "pch.h" // precompiled header has to be included first

#include "TilemapItemDropper.h"

#include "ComponentReference.t.h"
#include "Tilemap.h"

#include "AssetLibrarySystem.h"

#include "RigidBody.h"
#include "ItemComponent.h"

#include "Inspection.h"

#include "EntitySystem.h"
#include "TileInfoSystem.h"


//-----------------------------------------------------------------------------
// public: types
//-----------------------------------------------------------------------------

    
//-----------------------------------------------------------------------------
// private: types
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void TilemapItemDropper::OnInit()
    {
        m_ItemArchetype.SetOwnerName( GetName() );
        m_ItemArchetype.Init();

        m_Tilemap.SetOnConnectCallback(
            [ this ]()
            {
                m_Tilemap->AddOnTilemapChangedCallback(
                    GetId(),
                    std::bind(
                        &TilemapItemDropper::onTilemapChangedCallback,
                        this,
                        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3
                    )
                );
            }
        );

        m_Tilemap.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Tilemap->RemoveOnTilemapChangedCallback( GetId() );
            }
        );

        m_Tilemap.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void TilemapItemDropper::OnExit()
    {
        m_Tilemap.Exit();
    }


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------


    /// @brief  callback called whenever a tile is changed
    /// @param  tilemap         the tilemap that a tile was changed on
    /// @param  tilePos         the position of the changed tile; (-1, -1) if whole tilemap changed
    /// @param  previousValue   the previous value of the changed tile
    void TilemapItemDropper::onTilemapChangedCallback(
        Tilemap< int >* tilemap,
        glm::ivec2 const& tilePos,
        int const& previousValue
    ) const
    {
        if ( tilePos == glm::ivec2( -1, -1 ) )
        {
            return;
        }
        
        TileInfoSystem::TileInfo const* tileInfo = Tiles()->GetInfo( previousValue );
        if ( tileInfo == nullptr )
        {
            return;
        }

        for ( ItemStack const& itemStack : tileInfo->GetLootTable().GenerateLoot() )
        {
            dropItem( itemStack, tilePos );
        }
    }


    /// @brief  drop an ItemStack from the specified tile position
    /// @param  itemStack   the item stack to drop
    /// @param  tilePos     the position to drop the item
    void TilemapItemDropper::dropItem( ItemStack const& itemStack, glm::ivec2 const& tilePos ) const
    {
        if ( m_ItemArchetype == nullptr )
        {
            Debug() << "WARNING: TilemapItemDropper on entity " << GetEntity()->GetName() << " has no ItemArchetype" << std::endl;
            return;
        }

        // get position to spawn the item
        glm::vec2 position = m_Tilemap->TileCoordToWorldPos( tilePos );
        position += glm::vec2(
            random( -m_ItemSpawnRadius, m_ItemSpawnRadius ),
            random( -m_ItemSpawnRadius, m_ItemSpawnRadius )
        );

        // get initial velocity to spawn the item
        float angle = random( 0.0f, 6.283f );
        glm::vec2 velocity = glm::vec2(
            std::cos( angle ),
            std::sin( angle )
        ) * random( 0.0f, m_MaxInitialVelocity );


        // spawn the item
        Entity* itemEntity = m_ItemArchetype->Clone();

        itemEntity->GetComponent< Transform >()->SetTranslation( position );
        itemEntity->GetComponent< RigidBody >()->SetVelocity( velocity );
        itemEntity->GetComponent< ItemComponent >()->SetItemStack( itemStack );

        itemEntity->AddToScene();

    }


//-----------------------------------------------------------------------------
// private: inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this TilemapItemDropper in the Inspector
    void TilemapItemDropper::Inspector()
    {
        m_ItemArchetype.Inspect( "item prefab" );

        ImGui::DragFloat( "max initial velocity", &m_MaxInitialVelocity, 0.05f, 0.0f, INFINITY );

        ImGui::DragFloat( "item spawn radius", &m_ItemSpawnRadius, 0.05f, 0.0f, INFINITY );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Entity to drop items as
    /// @param  data    the json data to read from
    void TilemapItemDropper::readItemArchetype( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ItemArchetype, data );
    }


    /// @brief  reads the maximum initial velocity of a dropped item
    /// @param  data    the json data to read from
    void TilemapItemDropper::readMaxInitialVelocity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MaxInitialVelocity, data );
    }


    /// @brief  reads the square radius aroud the center of the tile to spawn the items in
    /// @param  data    the json data to read from
    void TilemapItemDropper::readItemSpawnRadius( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ItemSpawnRadius, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& TilemapItemDropper::GetReadMethods() const
    {
        static ReadMethodMap< TilemapItemDropper > const readMethods = {
            { "ItemArchetype"     , &TilemapItemDropper::readItemArchetype      },
            { "MaxInitialVelocity", &TilemapItemDropper::readMaxInitialVelocity },
            { "ItemSpawnRadius"   , &TilemapItemDropper::readItemSpawnRadius    }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all TilemapItemDropper data to a JSON file.
    /// @return The JSON file containing the TilemapItemDropper data.
    nlohmann::ordered_json TilemapItemDropper::Write() const
    {
        nlohmann::ordered_json json;

        json[ "ItemArchetype"      ] = Stream::Write( m_ItemArchetype      );
        json[ "MaxInitialVelocity" ] = Stream::Write( m_MaxInitialVelocity );
        json[ "ItemSpawnRadius"    ] = Stream::Write( m_ItemSpawnRadius    );

        return json;
    }


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    TilemapItemDropper::TilemapItemDropper() :
        Component( typeid( TilemapItemDropper ) )
    {}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this TilemapItemDropper
    /// @return the newly created clone of this TilemapItemDropper
    TilemapItemDropper* TilemapItemDropper::Clone() const
    {
        return new TilemapItemDropper( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the TilemapItemDropper
    /// @param  other   the other TilemapItemDropper to copy
    TilemapItemDropper::TilemapItemDropper( const TilemapItemDropper& other ) :
        Component( other ),
        m_MaxInitialVelocity( other.m_MaxInitialVelocity ),
        m_ItemSpawnRadius   ( other.m_ItemSpawnRadius    ),
        m_ItemArchetype     ( other.m_ItemArchetype      )
    {}


//-----------------------------------------------------------------------------
