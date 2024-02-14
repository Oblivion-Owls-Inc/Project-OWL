/// @file       Collider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Base component for detecting collisions
/// @version    0.1
/// @date       2023-09-29
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology


#include "Collider.h"

#include "CollisionSystem.h"
#include "DebugSystem.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// protected: constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    /// @param  type    the type of Component
    Collider::Collider( std::type_index type ) :
        Component( type )
    {}

    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    Collider::Collider( Collider const& other ) :
        Component( other ),
        m_CollisionLayerId(    other.m_CollisionLayerId    ),
        m_CollisionLayerFlags( other.m_CollisionLayerFlags )
    {}

    
//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets this Collider's Transform
    /// @return this Collider's Transform
    Transform* Collider::GetTransform() const
    {
        return m_Transform;
    }


    /// @brief  gets the collision layer of this Collider
    /// @return the collision layer of this Collider
    unsigned Collider::GetCollisionLayer() const
    {
        return m_CollisionLayerId;
    }

    /// @brief  sets the collision layer of this Collider
    /// @param  layerId the collision layer to set
    void Collider::SetCollisionLayer( unsigned layerId )
    {
        m_CollisionLayerId = layerId;
    }

    /// @brief  sets the collision layer of this Collider
    /// @param  layerName   the name of the collision layer to set
    void Collider::SetCollisionLayer( std::string const& layerName )
    {
        m_CollisionLayerId = Collisions()->GetCollisionLayerId( layerName );
    }


    /// @brief  gets the flags of which layers this Collider collides with
    /// @return the flags of which layers this Collider collides with
    CollisionLayerFlags Collider::GetCollisionLayerFlags() const
    {
        return m_CollisionLayerFlags;
    }

    /// @brief  sets the flags of which layers this Collider collides with
    /// @param  layerFlags  the flags of which layers this Collider should collide with
    void Collider::SetCollisionLayerFlags( CollisionLayerFlags layerFlags )
    {
        m_CollisionLayerFlags = layerFlags;
    }

    /// @brief  sets the flags of which layers this Collider collides with
    /// @param  layerFlags  the names of which layers this Collider should collide with
    void Collider::SetCollisionLayerFlags( std::vector< std::string > const& layerNames )
    {
        m_CollisionLayerFlags = Collisions()->GetLayerFlags( layerNames );
    }


    /// @brief  gets the list of colliders this Collider is currently colliding with
    /// @return the list of colliders this Collider is currently colliding with
    std::vector< Collider* > const& Collider::GetContacts() const
    {
        return m_Contacts;
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  checks if the collider is colliding with another collider
    /// @param  other   the other collider to check if is colliding with this one
    /// @return whether the colliders are colliding
    bool Collider::IsColliding( Collider* other )
    {
        return std::find( m_Contacts.begin(), m_Contacts.end(), other ) != m_Contacts.end();
    }


    /// @brief  adds a callback function to be called when this collider collides
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void Collider::AddOnCollisionCallback( unsigned ownerId, OnCollisionCallback callback )
    {
        m_OnCollisionCallbacks.emplace( ownerId, std::move( callback ) );
    }

    /// @brief  removes a callback function to be called when this collider collides
    /// @param  ownerId the ID of the owner of the callback to remove
    void Collider::RemoveOnCollisionCallback( unsigned ownerId )
    {
        m_OnCollisionCallbacks.erase( ownerId );
    }


    /// @brief  adds a callback function to be called when a collision begins
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void Collider::AddOnCollisionEnterCallback( unsigned ownerId, OnCollisionStateChangeCallback callback )
    {
        m_OnCollisionEnterCallbacks.emplace( ownerId, std::move( callback ) );
    }

    /// @brief  adds a callback function to be called when a collision begins
    /// @param  ownerId the ID of the owner of the callback to remove
    void Collider::RemoveOnCollisionEnterCallback( unsigned ownerId )
    {
        m_OnCollisionEnterCallbacks.erase( ownerId );
    }


    /// @brief  adds a callback function to be called when a collision ends
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void Collider::AddOnCollisionExitCallback( unsigned ownerId, OnCollisionStateChangeCallback callback )
    {
        m_OnCollisionExitCallbacks.emplace( ownerId, std::move( callback ) );
    }

    /// @brief  adds a callback function to be called when a collision ends
    /// @param  ownerId the ID of the owner of the callback to remove
    void Collider::RemoveOnCollisionExitCallback( unsigned ownerId )
    {
        m_OnCollisionExitCallbacks.erase( ownerId );
    }


    /// @brief  calls all OnCollision callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    /// @param  collisionData   the collisionData of the collision
    void Collider::CallOnCollisionCallbacks( Collider* other, CollisionData const& collisionData )
    {
        for ( auto& [ id, callback ] : m_OnCollisionCallbacks )
        {
            callback( other, collisionData );
        }
    }

    /// @brief  calls all OnCollisionEnter callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    void Collider::CallOnCollisionEnterCallbacks( Collider* other )
    {
        for ( auto& [ id, callback ] : m_OnCollisionEnterCallbacks )
        {
            callback( other );
        }
    }

    /// @brief  calls all OnCollisionExit callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    void Collider::CallOnCollisionExitCallbacks( Collider* other )
    {
        for ( auto& [ id, callback ] : m_OnCollisionExitCallbacks )
        {
            callback( other );
        }
    }


    /// @brief  adds a contact to this Collider's contacts array
    /// @param  other   the contact to add
    /// @note   SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    bool Collider::TryAddContact( Collider* other )
    {
        if ( std::find( m_Contacts.begin(), m_Contacts.end(), other ) != m_Contacts.end() )
        {
            return false;
        }

        m_Contacts.push_back( other );
        return true;
    }

    /// @brief  removes a contact to this Collider's contacts array
    /// @param  other   the contact to add
    /// @note   SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    bool Collider::TryRemoveContact( Collider* other )
    {
        auto it = std::find( m_Contacts.begin(), m_Contacts.end(), other );
        if ( it == m_Contacts.end() )
        {
            return false;
        }

        m_Contacts.erase( it );
        return true;
    }


//-----------------------------------------------------------------------------
// protected: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called when this Component's Entity enters the Scene
    void Collider::OnInit()
    {
        CollisionSystem::GetInstance()->addCollider( this );

        m_Transform.Init( GetEntity() );
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    void Collider::OnExit()
    {
        CollisionSystem::GetInstance()->removeCollider( this );

        m_Transform.Exit( GetEntity() );
    }

    
//-----------------------------------------------------------------------------
// protected: inspection
//-----------------------------------------------------------------------------


    /// @brief  debug inspector for Colliders
    void Collider::Inspector()
    {
        std::vector< std::string > const& collisionLayerNames = Collisions()->GetLayerNames();

        // collision layer
        if ( ImGui::BeginCombo( ( std::string( "Collision Layer##" ) + std::to_string( GetId() ) ).c_str(), collisionLayerNames[ m_CollisionLayerId ].c_str()) ) // Default displayed
        {
            for (int i = 0; i < collisionLayerNames.size(); ++i)
            {
                if ( ImGui::Selectable( collisionLayerNames[ i ].c_str(), m_CollisionLayerId == i ) )
                {
                    m_CollisionLayerId = i;
                }
            }
            ImGui::EndCombo();
        }

        /// collision layer flags
        Inspection::InspectCollisionLayerFlags( ( std::string( "Collision Layer Flags##" ) + std::to_string( GetId() ) ).c_str(), &m_CollisionLayerFlags );
    }


//-----------------------------------------------------------------------------
// protected: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the collision layer from json
    /// @param  data    the json data to read from
    void Collider::readCollisionLayer( nlohmann::ordered_json const& data )
    {
        if ( data.is_string() )
        {
            SetCollisionLayer( (std::string)data );
        }
        else if ( data.is_number_unsigned() )
        {
            SetCollisionLayer( (unsigned)data );
        }
    }

    /// @brief  reads the collision layer flags from json
    /// @param  data    the json data to read from
    void Collider::readCollisionLayerFlags( nlohmann::ordered_json const& data )
    {
        if ( data.is_array() )
        {
            std::vector< std::string > collisionLayerNames = data;
            SetCollisionLayerFlags( collisionLayerNames );
        }
        else if ( data.is_number_unsigned() )
        {
            SetCollisionLayerFlags( (CollisionLayerFlags)data );
        }
    }


//-----------------------------------------------------------------------------
