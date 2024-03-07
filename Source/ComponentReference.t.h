/// @file       ComponentReference.t.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to a component in the scene
/// @version    0.1
/// @date       2024-02-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once


#include "pch.h" 
#include "ComponentReference.h"

#include "Entity.h"

// I don't know why these are needed here, but it complains if we don't have them
#include "Collider.h"
#include "StaticBody.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Emitter.h"

#include "DebugSystem.h"


//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    template < class ComponentType, bool required >
    ComponentReference< ComponentType, required >::ComponentReference() = default;


    /// @brief  default destructor
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    template < class ComponentType, bool required >
    ComponentReference< ComponentType, required >::~ComponentReference() = default;


    /// @brief  move constructor
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  other   the ComponentReference to move into this one
    template < class ComponentType, bool required >
    ComponentReference< ComponentType, required >::ComponentReference( ComponentReference&& other ) noexcept :
        m_Entity   ( other.m_Entity    ),
        m_Component( other.m_Component ),
        m_OnConnectCallback   ( std::move( other.m_OnConnectCallback    ) ),
        m_OnDisconnectCallback( std::move( other.m_OnDisconnectCallback ) )
    {
        other.m_Entity = nullptr;
        other.m_Component = nullptr;

        if ( m_Entity != nullptr )
        {
            m_Entity->RemoveComponentReference( &other );
            m_Entity->AddComponentReference( this );
        }
    }

    /// @brief  move-assignment operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  other   the ComponentReference to move into this one
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::operator =( ComponentReference&& other ) noexcept
    {
        m_Entity    = other.m_Entity;
        m_Component = other.m_Component;
        m_OnConnectCallback    = std::move( other.m_OnConnectCallback    );
        m_OnDisconnectCallback = std::move( other.m_OnDisconnectCallback );

        other.m_Entity = nullptr;
        other.m_Component = nullptr;

        if ( m_Entity != nullptr )
        {
            m_Entity->RemoveComponentReference( &other );
            m_Entity->AddComponentReference( this );
        }
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this ComponentReference to point to the Component of its type on the specified Entity
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  entity  the Entity this ComponentReference's Component is attached to
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::Init( Entity* entity )
    {
        if ( entity == nullptr )
        {
            return;
        }

        m_Entity = entity;
        m_Component = entity->GetComponent< ComponentType >();

        if ( required && m_Component == nullptr )
        {
            Debug() << "WARNING: Could not find Component of type \"" << PrefixlessName( typeid( ComponentType ) )
                << "\" attached to Entity \"" << entity->GetName() << "\"\n" << std::endl;
        }

        if ( m_Component != nullptr && m_OnConnectCallback )
        {
            m_OnConnectCallback();
        }
        
        entity->AddComponentReference( this );
    }

    /// @brief  separates this ComponentReference from the target Entity
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::Exit()
    {
        if ( m_Component != nullptr && m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback();
        }

        m_Component = nullptr;

        if ( m_Entity == nullptr )
        {
            return;
        }

        m_Entity->RemoveComponentReference( this );
        m_Entity = nullptr;
    }


    /// @brief  sets the callback to call when this ComponentReference connects to a Component
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  callback    the callback to call
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::SetOnConnectCallback(
        std::function< void () > callback
    )
    {
        m_OnConnectCallback = callback;
    }

    /// @brief  sets the callback to call when this ComponentReference disconnects from a Component
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  callback    the callback to call
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::SetOnDisconnectCallback(
        std::function< void () > callback
    )
    {
        m_OnDisconnectCallback = callback;
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  dereference operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @return the Component this ComponentReference refers to
    template < class ComponentType, bool required >
    ComponentType& ComponentReference< ComponentType, required >::operator *() const
    {
        return *m_Component;
    }

    /// @brief  member dereference operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @return the Component this ComponentReference refers to
    template < class ComponentType, bool required >
    ComponentType* ComponentReference< ComponentType, required >::operator ->() const
    {
        return m_Component;
    }

    /// @brief  implicit cast operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @return this ComponentReference's internal pointer
    template < class ComponentType, bool required >
    ComponentReference< ComponentType, required >::operator ComponentType*() const
    {
        return m_Component;
    }


    /// @brief  assignment operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  component   the component to assign to this ComponentReference
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::operator =( ComponentType* component )
    {
        if ( m_Component != nullptr && m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback();
        }

        m_Component = component;
        m_Entity = m_Component != nullptr ? m_Component->GetEntity() : nullptr;

        if ( m_Component != nullptr && m_OnConnectCallback )
        {
            m_OnConnectCallback();
        }
    }


    /// @brief  gets the Entity this ComponentReference watches
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @return the Entity this ComponentReference watches
    template < class ComponentType, bool required >
    Entity const* ComponentReference< ComponentType, required >::GetEntity() const
    {
        return m_Entity;
    }


//-----------------------------------------------------------------------------
// public: engine methods
//-----------------------------------------------------------------------------


    /// @brief  sets this ComponentReference to nullptr
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::Clear()
    {
        if ( m_Component != nullptr && m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback();
        }

        m_Component = nullptr;
        m_Entity = nullptr;
    }


    /// @brief  tries to set this ComponentReference to point to the component, checking if it's valid
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  component   the component to try to set this ComponentReference to
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::TrySet( Component* component )
    {
        if ( m_Component != nullptr )
        {
            return;
        }

        // dynamic_cast will be nullptr if not valid
        m_Component = dynamic_cast< ComponentType* >( component );

        if ( m_Component != nullptr && m_OnConnectCallback )
        {
            m_OnConnectCallback();
        }
    }

    /// @brief  compares the currently pointed to Component with the specified Component, and clears it if they match
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @tparam required        whether this ComponentReference is required or optional (for debug logging purposes)
    /// @param  component   the component to compare with
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::TryRemove( Component* component )
    {
        if ( static_cast< ComponentType* >( component ) == m_Component )
        {
            if ( m_Component != nullptr && m_OnDisconnectCallback )
            {
                m_OnDisconnectCallback();
            }

            m_Component = nullptr;
        }
    }


//-----------------------------------------------------------------------------
