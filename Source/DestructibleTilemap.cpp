/// @file       DestructibleTilemap.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component responsible for handling the breaking of tiles with varrying strengths
/// @version    0.1
/// @date       2024-03-07
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h"

#include "DestructibleTilemap.h"

#include "ComponentReference.t.h"
#include "Tilemap.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    DestructibleTilemap::DestructibleTilemap() :
        Component( typeid( DestructibleTilemap ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  damages a tile in the tilemap
    /// @param  tilePos the tile to damage
    /// @param  damage  the amount to damage the tile
    /// @return the amount of overkill damage dealt, if the tile was destroyed
    float DestructibleTilemap::DamageTile( glm::ivec2 const& tilePos, float damage )
    {
        if (
            m_Tilemap       == nullptr || m_Tilemap      ->IsPositionWithinBounds( tilePos ) == false ||
            m_HealthTilemap == nullptr || m_HealthTilemap->IsPositionWithinBounds( tilePos ) == false
        )
        {
            return 0.0f;
        }

        float health = m_HealthTilemap->GetTile( tilePos );
        health -= damage;

        if ( health <= 0.0f )
        {
            m_Tilemap->SetTile( tilePos, -1 );
            m_HealthTilemap->SetTile( tilePos, 0.0f );
            return -health;
        }
        else
        {
            m_HealthTilemap->SetTile( tilePos, health );
            return 0.0f;
        }

    }

    /// @brief  gets the health proportion of the specified tile pos
    /// @param  tilePos the position of the tile to get the health of
    /// @return the health of the tile from 1 to 0
    float DestructibleTilemap::GetTileHealthProportion( glm::ivec2 const& tilePos ) const
    {
        if (
            m_Tilemap       == nullptr || m_Tilemap      ->IsPositionWithinBounds( tilePos ) == false ||
            m_HealthTilemap == nullptr || m_HealthTilemap->IsPositionWithinBounds( tilePos ) == false
        )
        {
            return 0.0f;
        }

        float health = m_HealthTilemap->GetTile( tilePos );
        float maxHealth = GetMaxHealth( m_Tilemap->GetTile( tilePos ) );

        return health / maxHealth;
    }


    /// @brief  gets the maximum health of tile type
    /// @param  tileId  the ID of the tile type to get the max heath of
    /// @return the max health of the specified tile type
    float DestructibleTilemap::GetMaxHealth( int tileId ) const
    {
        if ( tileId < 0 )
        {
            return 0.0f;
        }

        return tileId < m_TileHealths.size() ? m_TileHealths[ tileId ] : INFINITY;
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the source Tilemap attached to this Entity
    /// @return the source Tilemap attached to this Entity
    Tilemap< int > const* DestructibleTilemap::GetTilemap() const
    {
        return m_Tilemap;
    }


    /// @brief  gets the health Tilemap attached to this Entity
    /// @return the health Tilemap attached to this Entity
    Tilemap< float > const* DestructibleTilemap::GetHealthTilemap() const
    {
        return m_HealthTilemap;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void DestructibleTilemap::OnInit()
    {
        m_Tilemap.SetOnConnectCallback(
            [ this ]()
            {
                m_Tilemap->AddOnTilemapChangedCallback(
                    GetId(),
                    [ this ]( Tilemap< int >* tilemap, glm::ivec2 const& tilePos, int const& previousValue )
                    {
                        onTilemapChangedCallback( tilemap, tilePos, previousValue );
                    }
                );

                if ( m_HealthTilemap == nullptr )
                {
                    return;
                }

                reloadTileHealths();
            }
        );
        m_Tilemap.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Tilemap->RemoveOnTilemapChangedCallback( GetId() );
            }
        );

        m_HealthTilemap.SetOnConnectCallback(
            [ this ]()
            {
                if ( m_Tilemap == nullptr )
                {
                    return;
                }

                reloadTileHealths();
            }
        );

        m_Tilemap      .Init( GetEntity() );
        m_HealthTilemap.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void DestructibleTilemap::OnExit()
    {
        m_Tilemap      .Exit();
        m_HealthTilemap.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  callback called when the source tilemap changes
    /// @param  tilemap         the tilemap this callback was called from
    /// @param  tilepos         the position of the tile that was changed - will be (-1, -1) if whole tilemap changed
    /// @param  previousValue   the value of the tile before it was changed - garbage if whole tilemap changed
    void DestructibleTilemap::onTilemapChangedCallback(
        Tilemap< int >* tilemap,
        glm::ivec2 const& tilePos,
        int const& previousValue
    )
    {
        // verify that HealthTilemap exists
        if ( m_HealthTilemap == nullptr )
        {
            return;
        }

        // if the whole tilemap changed, reload the whole tilemap
        if ( tilePos == glm::ivec2( -1 ) )
        {
            reloadTileHealths();
            return;
        }

        // if the sizes don't match, something has gone wrong
        if ( m_Tilemap->GetDimensions() != m_HealthTilemap->GetDimensions() )
        {
            Debug() << "WARNING: source tilemap dimensions do not match health tilemap dimensions (" << GetName() << ")" << std::endl;
            return;
        }

        // set the health of the tile to the max health of the changed tile
        m_HealthTilemap->SetTile( tilePos, GetMaxHealth( m_Tilemap->GetTile( tilePos ) ) );
    }


    /// @brief  reloads the health tilemap from the source tilemap
    void DestructibleTilemap::reloadTileHealths()
    {
        glm::ivec2 dimensions = m_Tilemap->GetDimensions();
        int tileCount = dimensions.x * dimensions.y;

        std::vector< float > tileHealths( tileCount );

        for ( int i = 0; i < tileCount; ++i )
        {
            tileHealths[ i ] = GetMaxHealth( m_Tilemap->GetTilemap()[ i ] );
        }

        m_HealthTilemap->SetDimensions( dimensions );
        m_HealthTilemap->SetTilemap( tileHealths );
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for DestructibleTilemap
    void DestructibleTilemap::Inspector()
    {
        if ( m_Tilemap == nullptr )
        {
            ImGui::Text( "WARNING: no Tilemap<int> Component attached" );
        }
        if ( m_HealthTilemap == nullptr )
        {
            ImGui::Text( "WARNING: no Tilemap<float> Component attached" );
        }

        bool changed = Inspection::InspectArray< float >(
            "tile type healths",
            &m_TileHealths,
            []( float* health ) -> bool
            {
                bool changed = false;
                changed |= ImGui::DragFloat( "", health, 0.05f, 0.05f, INFINITY, "%3f" );
                ImGui::SameLine();
                if ( ImGui::SmallButton( "inf" ) )
                {
                    changed = true;
                    *health = INFINITY;
                }

                return changed;
            }
        );

        if ( changed && m_Tilemap != nullptr && m_HealthTilemap != nullptr )
        {
            reloadTileHealths();
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the default health of each type of tile
    /// @param  data    the JSON data to read from
    void DestructibleTilemap::readTileTypeHealths( nlohmann::ordered_json const& data )
    {
        Stream::Read< float >( &m_TileHealths, data );

        for ( float& health : m_TileHealths )
        {
            if ( health == 0.0f )
            {
                health = INFINITY;
            }
        }
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this DestructibleTilemap
    /// @return the map of read methods for this DestructibleTilemap
    ReadMethodMap< ISerializable > const& DestructibleTilemap::GetReadMethods() const
    {
        static ReadMethodMap< DestructibleTilemap > const readMethods = {
            { "TileTypeHealths", &DestructibleTilemap::readTileTypeHealths }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this DestructibleTilemap to JSON
    /// @return the JSON data of this DestructibleTilemap
    nlohmann::ordered_json DestructibleTilemap::Write() const
    {
        nlohmann::ordered_json json;

        nlohmann::ordered_json& tileHealths = json[ "TileTypeHealths" ];
        tileHealths = nlohmann::ordered_json::array();
        for ( float health : m_TileHealths )
        {
            // JSON doesn't support infinity for some reason, so we're using 0 as a placeholder for infinity, since 0 would also break things.
            tileHealths.push_back( health == INFINITY ? 0.0f : health );
        }

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this DestructibleTilemap
    /// @return the newly created clone of this DestructibleTilemap
    DestructibleTilemap* DestructibleTilemap::Clone() const
    {
        return new DestructibleTilemap( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the DestructibleTilemap
    /// @param  other   the other DestructibleTilemap to copy
    DestructibleTilemap::DestructibleTilemap( DestructibleTilemap const& other ) :
        Component( other ),
        m_TileHealths( other.m_TileHealths )
    {}


//-----------------------------------------------------------------------------
