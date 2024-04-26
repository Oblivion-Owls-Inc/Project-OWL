/// @file       Collider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Base component for detecting collisions
/// @version    0.1
/// @date       2023-09-29
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first


#include "DebugSystem.h"
#include "CollisionSystem.h"

#include "ComponentReference.t.h"




#include "Engine.h"

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

    /// @brief  gets this Collider's RigidBody
    /// @return this Collider's RigidBody
    RigidBody* Collider::GetRigidBody() const
    {
        return m_RigidBody;
    }

    /// @brief  gets this Collider's StaticBody
    /// @return this Collider's StaticBody
    StaticBody* Collider::GetStaticBody() const
    {
        return m_StaticBody;
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


    /// @brief  gets the list of colliders this Collider is currently colliding with
    /// @return the list of colliders this Collider is currently colliding with
    std::map< Collider*, int > const& Collider::GetContacts() const
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
        return m_Contacts.contains( other );
    }


    /// @brief  adds a callback function to be called when this collider collides
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void Collider::AddOnCollisionCallback( unsigned ownerId, OnCollisionCallback callback )
    {
        m_OnCollisionCallbacks.push_back( { ownerId, std::move( callback ) } );
    }

    /// @brief  removes a callback function to be called when this collider collides
    /// @param  ownerId the ID of the owner of the callback to remove
    void Collider::RemoveOnCollisionCallback( unsigned ownerId )
    {
        auto it = std::find_if(
            m_OnCollisionCallbacks.begin(),
            m_OnCollisionCallbacks.end(),
            [ ownerId ]( auto const& pair ) -> bool
            {
                return pair.first == ownerId;
            }
        );
        if ( it != m_OnCollisionCallbacks.end() )
        {
            m_OnCollisionCallbacks.erase( it );
        }
    }

    /// @brief  gets whether this Collider has any OnCollisionCallbacks
    /// @return whether this Collider has any OnCollisionCallbacks
    bool Collider::HasOnCollisionCallbacks() const
    {
        return m_OnCollisionCallbacks.empty() == false;
    }


    /// @brief  adds a callback function to be called when a collision begins
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void Collider::AddOnCollisionEnterCallback( unsigned ownerId, OnCollisionStateChangeCallback callback )
    {
        m_OnCollisionEnterCallbacks.push_back( { ownerId, std::move( callback ) } );
    }

    /// @brief  adds a callback function to be called when a collision begins
    /// @param  ownerId the ID of the owner of the callback to remove
    void Collider::RemoveOnCollisionEnterCallback( unsigned ownerId )
    {
        auto it = std::find_if(
            m_OnCollisionEnterCallbacks.begin(),
            m_OnCollisionEnterCallbacks.end(),
            [ ownerId ]( auto const& pair ) -> bool
            {
                return pair.first == ownerId;
            }
        );
        if ( it != m_OnCollisionEnterCallbacks.end() )
        {
            m_OnCollisionEnterCallbacks.erase( it );
        }
    }


    /// @brief  adds a callback function to be called when a collision ends
    /// @param  callback    the function to be called when this collider collides
    /// @param  ownerId     the ID of the owner of the callback
    /// @note   YOU MUST REMOVE THE CALLBACK WHEN YOU ARE DONE WITH IT
    void Collider::AddOnCollisionExitCallback( unsigned ownerId, OnCollisionStateChangeCallback callback )
    {
        m_OnCollisionExitCallbacks.push_back( { ownerId, std::move( callback ) } );
    }

    /// @brief  adds a callback function to be called when a collision ends
    /// @param  ownerId the ID of the owner of the callback to remove
    void Collider::RemoveOnCollisionExitCallback( unsigned ownerId )
    {
        auto it = std::find_if(
            m_OnCollisionExitCallbacks.begin(),
            m_OnCollisionExitCallbacks.end(),
            [ ownerId ]( auto const& pair ) -> bool
            {
                return pair.first == ownerId;
            }
        );
        if ( it != m_OnCollisionExitCallbacks.end() )
        {
            m_OnCollisionExitCallbacks.erase( it );
        }
    }


    /// @brief  calls all OnCollision callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    /// @param  collisionData   the collisionData of the collision
    /// @note   SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    void Collider::CallOnCollisionCallbacks( Collider* other, CollisionData const& collisionData )
    {
        for ( auto& [ id, callback ] : m_OnCollisionCallbacks )
        {
            callback( other, collisionData );
        }
    }


    /// @brief  tries to add a contact to this Collider's contacts array
    /// @param  other           the contact to add
    /// @param  currentFrame    the frame the Contact was added on
    /// @note   SHOULD ONLY BE CALLED BY COLLISIONSYSTEM
    void Collider::TryAddContact( Collider* other, int currentFrame )
    {
        // don't track contacts on Entities without OnCollisionStateChange callbacks
        if ( m_OnCollisionEnterCallbacks.empty() && m_OnCollisionExitCallbacks.empty() )
        {
            return;
        }

        // [] operator adds element if it doesn't exist
        int& previousFrame = m_Contacts[ other ];

        // check if the contact already existed
        if ( previousFrame == 0 )
        {
            CallOnCollisionEnterCallbacks( other );
        }

        previousFrame = currentFrame;
    }


    /// @brief  removes all outdated contacts from this Collider and calls OnCollisionExit callbacks
    void Collider::RemoveOutdatedContacts()
    {
        int currentFrame = GameEngine()->GetFixedFrameCount();
        std::erase_if(
            m_Contacts,
            [ this, currentFrame ]( std::pair< Collider*, int > const& contact ) -> bool
            {
                if ( contact.second != currentFrame )
                {
                    CallOnCollisionExitCallbacks( contact.first );
                    return true;
                }
                return false;
            }
        );
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


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


//-----------------------------------------------------------------------------
// protected: inspection
//-----------------------------------------------------------------------------


    /// @brief  debug inspector for Colliders
    void Collider::Inspector()
    {
        std::vector< std::string > const& collisionLayerNames = Collisions()->GetLayerNames();
        // collision layer
        if ( ImGui::BeginCombo(
            ( std::string( "Collision Layer##" ) + std::to_string( GetId() ) ).c_str(),
            m_CollisionLayerId >= collisionLayerNames.size() ? "Unknown Layer" : collisionLayerNames[m_CollisionLayerId].c_str() // Default displayed
        ) )
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
        m_CollisionLayerFlags.Inspect( "Collision Layer Flags" );
    }


//-----------------------------------------------------------------------------
// protected: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the collision layer from json
    /// @param  data    the json data to read from
    void Collider::readCollisionLayer( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_CollisionLayerId, data );
    }

    /// @brief  reads the collision layer flags from json
    /// @param  data    the json data to read from
    void Collider::readCollisionLayerFlags( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_CollisionLayerFlags, data );
    }


//-----------------------------------------------------------------------------
