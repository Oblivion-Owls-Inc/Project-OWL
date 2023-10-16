/// @file       Collider.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Base component for detecting collisions
/// @version    0.1
/// @date       2023-09-29
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology


#include "Collider.h"

#include "Entity.h"
#include "Transform.h"

#include "CollisionSystem.h"

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
        Component( other )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  adds a callback function to be called when this collider collides
    /// @param  callback    the function to be called when this collider collides
    /// @return a handle to the created callback
    /// @note   YOU MUST CLEAR THE CALLBACK USING THE CALLBACK HANDLE WHEN YOU ARE DONE WITH IT
    /// @note   the callback will be called every time this collider collides (but not each time it loops)
    unsigned Collider::AddOnCollisionCallback( std::function< void ( Collider*, CollisionData const& ) > callback )
    {
        unsigned handle = GetUniqueId();
        m_OnCollisionCallbacks.emplace( handle, std::move( callback ) );
        return handle;
    }

    /// @brief  removes a callback function to be called when this collider collides
    /// @param  callbackHandle  the handle of the callback to remove
    void Collider::RemoveOnCollisionCallback( unsigned callbackHandle )
    {
        m_OnCollisionCallbacks.erase( callbackHandle );
    }

    /// @brief  calls all OnCollision callbacks attached to this Collider
    /// @param  other           the other entity this Collider collided with
    /// @param  collisionData   the collisionData of the collision
    void Collider::CallOnCollisionCallbacks( Collider* other, CollisionData const& collisionData )
    {
        for ( auto callback : m_OnCollisionCallbacks )
        {
            callback.second( other, collisionData );
        }
    }


    /// @brief  sets the collision layer of this Collider
    /// @param  layerName   the name of the collision layer to set
    void Collider::SetCollisionLayer( std::string const& layerName )
    {
        m_CollisionLayerId = Collisions()->GetCollisionLayerId( layerName );
    }

    /// @brief  sets the collision layer of this Collider
    /// @param  layerNames  the name of the collision layer to set
    void Collider::SetCollisionLayerNames( std::vector< std::string > const& layerNames )
    {
        m_CollisionLayerFlags = Collisions()->GetLayerFlags( layerNames );
    }

//-----------------------------------------------------------------------------
// protected: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  called when this Component's Entity enters the Scene
    void Collider::OnInit()
    {
        CollisionSystem::GetInstance()->addCollider(this);
        m_Transform = GetParent()->GetComponent<Transform>();
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    void Collider::OnExit()
    {
        CollisionSystem::GetInstance()->removeCollider(this);
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
            SetCollisionLayer( data );
        }
        else if ( data.is_number_unsigned() )
        {
            SetCollisionLayerId( data );
        }
    }

    /// @brief  reads the collision layer flags from json
    /// @param  data    the json data to read from
    void Collider::readCollisionLayerFlags( nlohmann::ordered_json const& data )
    {
        if ( data.is_array() )
        {
            SetCollisionLayerNames( data );
        }
        else if ( data.is_number_unsigned() )
        {
            SetCollisionLayerFlags( data );
        }
    }

//-----------------------------------------------------------------------------
