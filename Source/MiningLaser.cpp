/// @file       MiningLaser.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Digging laser controllable by an entity
/// @version    0.1
/// @date       2024-01-14
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "MiningLaser.h"

#include "BehaviorSystem.h"

#include "Transform.h"
#include "AudioPlayer.h"
#include "Health.h"
#include "Emitter.h"
#include "Tilemap.h"

#include "Collider.h"
#include "TilemapCollider.h"

#include "EntitySystem.h"
#include "Engine.h"

#include "RenderSystem.h"

#include "Inspection.h"

// TODO: include health component


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    MiningLaser::MiningLaser() :
        Behavior( typeid( MiningLaser ) )
    {}


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    
    /// @brief  gets the range
    /// @return the range
    float MiningLaser::GetRange() const
    {
        return m_Range;
    }

    /// @brief  sets the range
    /// @param  range   the new range
    void MiningLaser::SetRange( float range )
    {
        m_Range = range;
    }


    /// @brief  gets how quickly the laser breaks blocks
    /// @return how quickly the laser breaks blocks
    float MiningLaser::GetMiningSpeed() const
    {
        return m_MiningSpeed;
    }

    /// @brief  sets how quickly the laser breaks blocks
    /// @param  miningSpeed  how quickly the laser breaks blocks
    void MiningLaser::SetMiningSpeed( float miningSpeed )
    {
        m_MiningSpeed = miningSpeed;
    }


    /// @brief  gets the max in progress tiles
    /// @return the max in progress tiles
    int MiningLaser::GetMaxInProgressTiles() const
    {
        return m_MaxInProgressTiles;
    }

    /// @brief  sets the max in progress tiles
    /// @param  maxInProgressTiles  the max in progress tiles
    void MiningLaser::SetMaxInProgressTiles( int maxInProgressTiles )
    {
        m_MaxInProgressTiles = maxInProgressTiles;
    }


    /// @brief  gets the beam color
    /// @return the beam color
    glm::vec4 const& MiningLaser::GetBeamColor() const
    {
        return m_BeamColor;
    }

    /// @brief  sets the beam color
    /// @param  beamColor   the beam color
    void MiningLaser::SetBeamColor( glm::vec4 const& beamColor )
    {
        m_BeamColor = beamColor;
    }


    /// @brief  gets the beam width
    /// @return the beam width
    float MiningLaser::GetBeamWidth() const
    {
        return m_BeamWidth;
    }

    /// @brief  sets the beam width
    /// @param  beamWidth   the beam width
    void MiningLaser::SetBeamWidth( float beamWidth )
    {
        m_BeamWidth = beamWidth;
    }


    /// @brief  gets how much damage per second the laser deals
    /// @return how much damage per second the laser deals
    float MiningLaser::GetDamageRate() const
    {
        return m_DamageRate;
    }

    /// @brief  sets how much damage per second the laser deals
    /// @param  dps how much damage per second the laser deals
    void MiningLaser::SetDamageRate( float dps )
    {
        m_DamageRate = dps;
    }


    /// @brief  gets which layers the laser collides with
    /// @return which layers the laser collides with
    CollisionLayerFlags MiningLaser::GetCollisionLayers() const
    {
        return m_CollisionLayers;
    }

    /// @brief  sets which layers the laser collides with
    /// @param  collisionLayers which layers the laser collides with
    void MiningLaser::SetCollisionLayers( CollisionLayerFlags collisionLayers )
    {
        m_CollisionLayers = collisionLayers;
    }


    /// @brief  gets the direction that the beam is firing in
    /// @return the direction that the beam is firing in
    glm::vec2 const& MiningLaser::GetDirection() const
    {
        return m_Direction;
    }

    /// @brief  sets the direction that the beam is firing in
    /// @param  direction   the direction that the beam is firing in
    void MiningLaser::SetDirection( glm::vec2 const& direction )
    {
        m_Direction = direction;
    }


    /// @brief  gets whether the beam is firing
    /// @return whether the beam is firing
    bool MiningLaser::GetIsFiring() const
    {
        return m_IsFiring;
    }

    /// @brief  sets whether the beam is firing
    /// @return whether the beam is firing
    void MiningLaser::SetIsFiring( bool isFiring )
    {
        m_IsFiring = isFiring;
    }


    /// @brief  gets the Transform attached to this MiningLaser
    /// @return the Transform attached to this MiningLaser
    Transform* MiningLaser::GetTransform() const
    {
        return m_Transform;
    }

    /// @brief  gets the Tilemap this MiningLaser digs in
    /// @return the Tilemap this MiningLaser digs in
    Tilemap< int >* MiningLaser::GetTilemap() const
    {
        return m_Tilemap;
    }


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


    /// @brief  adds an OnBreakTile callback to this MiningLaser
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback to add
    void MiningLaser::AddOnBreakTileCallback( unsigned ownerId, OnBreakTileCallback callback )
    {
        m_OnBreakTileCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes an OnBreakTile callback from this MiningLaser
    /// @param  ownerId     the ID of the owner of the callback to remove
    void MiningLaser::RemoveOnBreakTileCallback( unsigned ownerId )
    {
        auto it = m_OnBreakTileCallbacks.find( ownerId );
        if ( it != m_OnBreakTileCallbacks.end() )
        {
            m_OnBreakTileCallbacks.erase( it );
        }
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void MiningLaser::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_Transform.Init( GetEntity() );

        m_TilemapEntity.SetOwnerName( GetName() );
        m_TilemapEntity.Init();
    }

    /// @brief  called once when exiting the scene
    void MiningLaser::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_Transform.Exit( GetEntity() );
        m_TilemapEntity.Exit();
    }

    /// @brief  called every graohics frame
    void MiningLaser::OnUpdate( float dt )
    {
        if ( m_IsFiring == false )
        {
            return;
        }

        if ( m_Transform == nullptr )
        {
            return;
        }

        glm::vec2 start = m_Transform->GetTranslation();
        Renderer()->DrawLine( start, start + m_Direction * m_beamLength, m_BeamWidth, m_BeamColor );
    }

    /// @brief  called every simulation frame
    void MiningLaser::OnFixedUpdate()
    {
        if ( m_IsFiring == false )
        {
            return;
        }

        if ( m_Transform == nullptr )
        {
            return;
        }

        RayCastHit hit = Collisions()->RayCast( m_Transform->GetTranslation(), m_Direction, m_Range, m_CollisionLayers );
        m_beamLength = hit.distance;

        if ( hit.colliderHit == nullptr )
        {
            return;
        }

        if (
            hit.colliderHit->GetType() == typeid( TilemapCollider ) &&
            m_Tilemap != nullptr &&
            static_cast< TilemapCollider* >( hit.colliderHit )->GetTilemap() == m_Tilemap
        )
        {
            damageTile( hit.tilePos );
        }
        else
        {
            tryDamageEntity( hit.colliderHit->GetEntity() );
        }

    }


//-----------------------------------------------------------------------------
// private: helper methods
//-----------------------------------------------------------------------------

    
    /// @brief  tries to damages the specified entity
    /// @param  entity  the entity to damage
    void MiningLaser::tryDamageEntity( Entity* entity )
    {
        Health* entityHealth = entity->GetComponent<Health>();
        // TODO: try damaging the entity
        if (entityHealth)
        {
            m_AccumulatedDamage += GameEngine()->GetFixedFrameDuration() * m_DamageRate;
            
            int damage = static_cast<int>(m_AccumulatedDamage);
            
            entityHealth->TakeDamage(damage);

            m_AccumulatedDamage -= damage;
        }
    }

    /// @brief  damages the specified tile
    /// @param  tilePos the tile to damage
    void MiningLaser::damageTile( glm::ivec2 const& tilePos )
    {
        auto ipt = getInProgressTile( tilePos );

        // TODO: run damage tile effect?

        ipt->remainingTime -= GameEngine()->GetFixedFrameDuration() * m_MiningSpeed;

        if ( ipt->remainingTime <= 0 )
        {
            breakTile( ipt );
        }
    }

    /// @brief  breaks the specified tile
    /// @param  tilePos the tile to break
    void MiningLaser::breakTile( std::deque< MiningLaser::InProgressTile >::iterator const& ipt )
    {

        // call callbacks
        for ( auto const& [ ownerId, callback ] : m_OnBreakTileCallbacks )
        {
            callback( m_Tilemap, ipt->M_TilePos, m_Tilemap->GetTile( ipt->M_TilePos ) );
        }

        // TODO: play delete tile effect

        m_Tilemap->SetTile( ipt->M_TilePos, -1 );

        m_InProgressTiles.erase( ipt );
    }


    /// @brief  gets an in progress tile, adding it to the queue if it doesn't exist yet
    /// @param  tilePos the position of the tile to get
    /// @return the in progress tile data for that tile
    std::deque< MiningLaser::InProgressTile >::iterator MiningLaser::getInProgressTile( glm::ivec2 tilePos )
    {
        auto it = std::find_if(
            m_InProgressTiles.begin(),
            m_InProgressTiles.end(),
            [ tilePos ]( InProgressTile const& ipt ) -> bool
            {
                return ipt.M_TilePos == tilePos;
            }
        );

        if ( it != m_InProgressTiles.end() )
        {
            return it;
        }

        if ( m_InProgressTiles.size() == m_MaxInProgressTiles )
        {
            m_InProgressTiles.pop_front();
        }

        InProgressTile newIpt;
        newIpt.M_TilePos = tilePos;
        newIpt.remainingTime = 1.0f; // TEMP: this should be based on the type of tile, not constant

        m_InProgressTiles.push_back( newIpt );

        return m_InProgressTiles.end() - 1;
    }


//-----------------------------------------------------------------------------
// private: inspector methods
//-----------------------------------------------------------------------------

    
    /// @brief  displays this MiningLaser in the Inspector
    void MiningLaser::Inspector()
    {
        if ( m_Transform == nullptr )
        {
            ImGui::Text( "WARNING: no Transform attached" );
        }
        if ( m_Tilemap == nullptr )
        {
            ImGui::Text( "WARNING: target Entity doesn't exist or doesn't have a tilemap" );
        }

        m_TilemapEntity.Inspect( "Target tilemap Entity" );


        ImGui::DragFloat( "Max Range", &m_Range, 0.05f, 0.0f, INFINITY );
        
        ImGui::DragFloat( "Mining Speed", &m_MiningSpeed, 0.05f, 0.0f, INFINITY );
        
        ImGui::DragInt( "Max In-Progress Tiles", &m_MaxInProgressTiles, 0.05f, 1, INT_MAX );
        
        ImGui::ColorEdit4( "Beam Color", &m_BeamColor[0] );
        
        ImGui::DragFloat( "Beam Width", &m_BeamWidth, 0.05f, 0.0f, INFINITY );
        
        ImGui::DragFloat( "Damage Per Second", &m_DamageRate, 0.05f );

        Inspection::InspectCollisionLayerFlags( "Collsion Layers", &m_CollisionLayers );

        float angle = std::atan2( m_Direction.y, m_Direction.x );
        if ( ImGui::SliderAngle( "Direction", &angle, -180.0f, 180.0f ) )
        {
            m_Direction = glm::vec2( std::cos( angle ), std::sin( angle ) );
        }

        ImGui::Checkbox( "Is Firing", &m_IsFiring );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Tilemap entity
    /// @param  data the json data to read from
    void MiningLaser::readTilemapEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TilemapEntity, data );
    }


    /// @brief  reads the range of the laser
    /// @param  data the json data to read from
    void MiningLaser::readRange( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Range, data );
    }


    /// @brief  reads how quickly the mining laser breaks tiles
    /// @param  data the json data to read from
    void MiningLaser::readMiningSpeed( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MiningSpeed, data );
    }


    /// @brief  reads how many tiles the laser can damage at once
    /// @param  data the json data to read from
    void MiningLaser::readMaxInProgressTiles( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_MaxInProgressTiles, data );
    }


    /// @brief  reads the color of the beam
    /// @param  data the json data to read from
    void MiningLaser::readBeamColor( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_BeamColor, data );
    }

    /// @brief  reads the width of the beam
    /// @param  data the json data to read from
    void MiningLaser::readBeamWidth( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_BeamWidth, data );
    }


    /// @brief  reads the damage rate
    /// @param  data the json data to read from
    void MiningLaser::readDamageRate( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_DamageRate, data );
    }


    /// @brief  reads the collision layers the laser hits
    /// @param  data the json data to read from
    void MiningLaser::readCollideWithLayers( nlohmann::ordered_json const& data )
    {
        if ( data.is_string() )
        {
            m_CollisionLayers = Collisions()->GetLayerFlags( data );
        }
        else if ( data.is_number_unsigned() )
        {
            Stream::Read( m_CollisionLayers, data );
        }
    }


    /// @brief  reads the direction the laser fires in
    /// @param  data the json data to read from
    void MiningLaser::readDirection( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_Direction, data );
    }

    /// @brief  reads whether the beam is firing
    /// @param  data the json data to read from
    void MiningLaser::readIsFiring( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_IsFiring, data );
    }



    /// @brief  map of the read methods for this Component
    ReadMethodMap< MiningLaser > MiningLaser::s_ReadMethods = {
        { "TilemapEntity"       , &readTilemapEntity        },
        { "Range"               , &readRange                },
        { "MiningSpeed"         , &readMiningSpeed          },
        { "MaxInProgressTiles"  , &readMaxInProgressTiles   },
        { "BeamColor"           , &readBeamColor            },
        { "BeamWidth"           , &readBeamWidth            },
        { "DamageRate"          , &readDamageRate           },
        { "CollideWithLayers"   , &readCollideWithLayers    },
        { "Direction"           , &readDirection            },
        { "IsFiring"            , &readIsFiring             }
    };


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  Write all MiningLaser data to a JSON file.
    /// @return The JSON file containing the MiningLaser data.
    nlohmann::ordered_json MiningLaser::Write() const
    {
        nlohmann::ordered_json json;

        json[ "TilemapEntity"       ] = Stream::Write( m_TilemapEntity      );
        json[ "Range"               ] = Stream::Write( m_Range              );
        json[ "MiningSpeed"         ] = Stream::Write( m_MiningSpeed        );
        json[ "MaxInProgressTiles"  ] = Stream::Write( m_MaxInProgressTiles );
        json[ "BeamColor"           ] = Stream::Write( m_BeamColor          );
        json[ "BeamWidth"           ] = Stream::Write( m_BeamWidth          );
        json[ "DamageRate"          ] = Stream::Write( m_DamageRate         );
        json[ "CollideWithLayers"   ] = Stream::Write( m_CollisionLayers    );
        json[ "Direction"           ] = Stream::Write( m_Direction          );
        json[ "IsFiring"            ] = Stream::Write( m_IsFiring           );

        return json;
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor for the MiningLaser
    /// @param  other   the other MiningLaser to copy
    MiningLaser::MiningLaser( const MiningLaser& other ) :
        Behavior( typeid( MiningLaser ) ),
        m_Range             ( other.m_Range              ),
        m_MiningSpeed       ( other.m_MiningSpeed        ),
        m_MaxInProgressTiles( other.m_MaxInProgressTiles ),
        m_BeamColor         ( other.m_BeamColor          ),
        m_BeamWidth         ( other.m_BeamWidth          ),
        m_DamageRate        ( other.m_DamageRate         ),
        m_CollisionLayers   ( other.m_CollisionLayers    ),
        m_Direction         ( other.m_Direction          ),
        m_IsFiring          ( other.m_IsFiring           ),
        m_AccumulatedDamage ( other.m_AccumulatedDamage  ),

        m_TilemapEntity( other.m_TilemapEntity, { &m_Tilemap } )
    {}

//-----------------------------------------------------------------------------
