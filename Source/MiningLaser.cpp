/// @file       MiningLaser.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Digging laser controllable by an entity
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "MiningLaser.h"

#include "BehaviorSystem.h"

#include "Transform.h"
#include "AudioPlayer.h"
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
    {
        // TODO: constructor    
    }


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


    /// @brief  gets the laser source offset
    /// @return the laser source offset
    glm::vec2 const& MiningLaser::GetSourceOffset() const
    {
        return m_SourceOffset;
    }

    /// @brief  sets the laser source offset
    /// @param  sourceOffset    the laser source offset
    void MiningLaser::SetSourceOffset( glm::vec2 const& sourceOffset )
    {
        m_SourceOffset = sourceOffset;
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


//-----------------------------------------------------------------------------
// public:  methods
//-----------------------------------------------------------------------------


    /// @brief  gets the world emission position
    /// @return the world emission position
    glm::vec2 MiningLaser::GetWorldEmissionPosition() const
    {
        glm::vec2 worldOffset = m_Transform->GetMatrix() * glm::vec4( m_SourceOffset, 0.0f, 0.0f );
        return m_Transform->GetTranslation() + worldOffset;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void MiningLaser::OnInit()
    {
        Behaviors< Behavior >()->AddBehavior( this );

        m_Transform = GetEntity()->GetComponent< Transform >();
        m_AudioPlayer = GetEntity()->GetComponent< AudioPlayer >();
        m_ParticleEmitter = GetEntity()->GetComponent< Emitter >();

        Entity* tilemapEntity = Entities()->GetEntity( m_TilemapName );
        if ( tilemapEntity != nullptr )
        {
            m_Tilemap = tilemapEntity->GetComponent< Tilemap< int > >();
        }
    }

    /// @brief  called once when exiting the scene
    void MiningLaser::OnExit()
    {
        Behaviors< Behavior >()->RemoveBehavior( this );
    }

    /// @brief  called every graohics frame
    void MiningLaser::OnUpdate( float dt )
    {
        if ( m_IsFiring == false )
        {
            return;
        }

        glm::vec2 start = GetWorldEmissionPosition();
        Renderer()->DrawLine( start, start + m_Direction * m_beamLength, m_BeamWidth, m_BeamColor );
    }

    /// @brief  called every simulation frame
    void MiningLaser::OnFixedUpdate()
    {
        if ( m_IsFiring == false )
        {
            return;
        }

        RayCastHit hit = Collisions()->RayCast( GetWorldEmissionPosition(), m_Direction, m_Range, m_CollideWithLayers );
        m_beamLength = hit.distance;

        if ( hit.colliderHit == nullptr )
        {
            return;
        }

        if (
            hit.colliderHit->GetType() == typeid( TilemapCollider ) &&
            static_cast< TilemapCollider* >( hit.colliderHit )->GetTilemap() == m_Tilemap
        )
        {
            damageTile( getTargettedTile( hit ) );
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
    void MiningLaser::tryDamageEntity( Entity* entity ) const
    {
        // TODO: try damaging the entity
    }


    /// @brief  gets the tile position of a raycast hit
    /// @param  hit the RayCastHit to get the tile position of
    /// @return the tile position of the raycast hit
    glm::ivec2 MiningLaser::getTargettedTile( RayCastHit const& hit ) const
    {
        return m_Tilemap->WorldPosToTileCoord( GetWorldEmissionPosition() + m_Direction * hit.distance);
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
        // TODO: play delete tile effect

        // TODO: gain resources

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
        Entity* tilemapEntity = m_Tilemap != nullptr ? m_Tilemap->GetEntity() : nullptr;
        if ( Inspection::SelectEntityFromScene( "Tilemap Entity", &tilemapEntity ) )
        {
            m_Tilemap = tilemapEntity->GetComponent< Tilemap< int > >();
            m_TilemapName = tilemapEntity->GetName();
        }


        ImGui::DragFloat( "Max Range", &m_Range, 0.05f, 0.0f, INFINITY );


        ImGui::DragFloat2( "Beam Offset", &m_SourceOffset[ 0 ], 0.05f);
        
        ImGui::DragFloat( "Mining Speed", &m_MiningSpeed, 0.05f, 0.0f, INFINITY );
        
        ImGui::DragInt( "Max In-Progress Tiles", &m_MaxInProgressTiles, 0.05f, 1, INT_MAX );
        
        ImGui::ColorEdit4( "Beam Color", &m_BeamColor[0] );
        
        ImGui::DragFloat( "Beam Width", &m_BeamWidth, 0.05f, 0.0f, INFINITY );
        
        ImGui::DragFloat( "Damage Per Second", &m_DamageRate, 0.05f );

        Inspection::InspectCollisionLayerFlags( "Collsion Layers", &m_CollideWithLayers );

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
    void MiningLaser::readTilemapName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_TilemapName, data );
    }


    /// @brief  reads the range of the laser
    /// @param  data the json data to read from
    void MiningLaser::readRange( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Range, data );
    }


    /// @brief  reads offset of the laser source
    /// @param  data the json data to read from
    void MiningLaser::readSourceOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_SourceOffset, data );
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
            m_CollideWithLayers = Collisions()->GetLayerFlags( data );
        }
        else if ( data.is_number_unsigned() )
        {
            Stream::Read( m_CollideWithLayers, data );
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
        { "TilemapName"         , &readTilemapName          },
        { "Range"               , &readRange                },
        { "SourceOffset"        , &readSourceOffset         },
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

        json[ "TilemapName"         ] = Stream::Write( m_TilemapName        );
        json[ "Range"               ] = Stream::Write( m_Range              );
        json[ "SourceOffset"        ] = Stream::Write( m_SourceOffset       );
        json[ "MiningSpeed"         ] = Stream::Write( m_MiningSpeed        );
        json[ "MaxInProgressTiles"  ] = Stream::Write( m_MaxInProgressTiles );
        json[ "BeamColor"           ] = Stream::Write( m_BeamColor          );
        json[ "BeamWidth"           ] = Stream::Write( m_BeamWidth          );
        json[ "DamageRate"          ] = Stream::Write( m_DamageRate         );
        json[ "CollideWithLayers"   ] = Stream::Write( m_CollideWithLayers  );
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
        m_TilemapName( other.m_TilemapName ),
        m_Range( other.m_Range ),
        m_SourceOffset( other.m_SourceOffset ),
        m_MiningSpeed( other.m_MiningSpeed ),
        m_MaxInProgressTiles( other.m_MaxInProgressTiles ),
        m_BeamColor( other.m_BeamColor ),
        m_BeamWidth( other.m_BeamWidth ),
        m_DamageRate( other.m_DamageRate ),
        m_CollideWithLayers( other.m_CollideWithLayers ),
        m_Direction( other.m_Direction ),
        m_IsFiring( other.m_IsFiring )
    {}
