/// @file       ComponentReference.t.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to a component in the scene
/// @version    0.1
/// @date       2024-02-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#define COMPONENTREFERENCE_T

#ifndef COMPONENTREFERENCE_H
#include "ComponentReference.h"
#endif

#ifndef ENTITY_H
#include "Entity.h"
#endif


//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    template< class ComponentType >
    ComponentReference< ComponentType >::ComponentReference() = default;


    /// @brief  default destructor
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    template< class ComponentType >
    ComponentReference< ComponentType >::~ComponentReference() = default;


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this ComponentReference to point to the Component of its type on the specified Entity
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @param  entity  the Entity this ComponentReference's Component is attached to
    template< class ComponentType >
    void ComponentReference< ComponentType >::Init( Entity* entity )
    {
        if ( entity == nullptr )
        {
            return;
        }

        m_Component = entity->GetComponent< ComponentType >();

        if ( m_Component != nullptr && m_OnConnectCallback )
        {
            m_OnConnectCallback( m_Component );
        }
        
        entity->AddComponentReference( this );
    }

    /// @brief  separates this ComponentReference from the target Entity
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @param  entity  the entity this ComponentReference is curently watching
    template< class ComponentType >
    void ComponentReference< ComponentType >::Exit( Entity* entity )
    {
        if ( m_Component != nullptr && m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback( m_Component );
        }

        m_Component = nullptr;

        if ( entity == nullptr )
        {
            return;
        }

        entity->RemoveComponentReference( this );
    }


    /// @brief  sets the callback to call when this ComponentReference connects to a Component
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @param  callback    the callback to call
    template< class ComponentType >
    void ComponentReference< ComponentType >::SetOnConnectCallback(
        std::function< void( ComponentType* component ) > callback
    )
    {
        m_OnConnectCallback = callback;
    }

    /// @brief  sets the callback to call when this ComponentReference disconnects from a Component
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @param  callback    the callback to call
    template< class ComponentType >
    void ComponentReference< ComponentType >::SetOnDisconnectCallback(
        std::function< void ( ComponentType* component ) > callback
    )
    {
        m_OnDisconnectCallback = callback;
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  dereference operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @return the Component this ComponentReference refers to
    template< class ComponentType >
    ComponentType& ComponentReference< ComponentType >::operator *() const
    {
        return *m_Component;
    }

    /// @brief  member dereference operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @return the Component this ComponentReference refers to
    template< class ComponentType >
    ComponentType* ComponentReference< ComponentType >::operator ->() const
    {
        return m_Component;
    }

    /// @brief  implicit cast operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @return this ComponentReference's internal pointer
    template< class ComponentType >
    ComponentReference< ComponentType >::operator ComponentType*() const
    {
        return m_Component;
    }


    /// @brief  assignment operator
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @param  component   the component to assign to this ComponentReference
    template< class ComponentType >
    void ComponentReference< ComponentType >::operator =( ComponentType* component )
    {
        if ( m_Component != nullptr && m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback( m_Component );
        }

        m_Component = component;

        if ( m_Component != nullptr && m_OnConnectCallback )
        {
            m_OnConnectCallback( m_Component );
        }
    }


//-----------------------------------------------------------------------------
// public: engine methods
//-----------------------------------------------------------------------------


    /// @brief  sets this ComponentReference to nullptr
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    template< class ComponentType >
    void ComponentReference< ComponentType >::Clear()
    {
        if ( m_Component != nullptr && m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback( m_Component );
        }

        m_Component = nullptr;
    }


    /// @brief  tries to set this ComponentReference to point to the component, checking if it's valid
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @param  component   the component to try to set this ComponentReference to
    template< class ComponentType >
    void ComponentReference< ComponentType >::TrySet( Component* component )
    {
        if ( m_Component != nullptr )
        {
            return;
        }

        // dynamic_cast will be nullptr if not valid
        m_Component = dynamic_cast< ComponentType* >( component );

        if ( m_Component != nullptr && m_OnConnectCallback )
        {
            m_OnConnectCallback( m_Component );
        }
    }

    /// @brief  compares the currently pointed to Component with the specified Component, and clears it if they match
    /// @tparam ComponentType   the type of Component this ComponentReference refers to
    /// @param  component   the component to compare with
    template< class ComponentType >
    void ComponentReference< ComponentType >::TryRemove( Component* component )
    {
        if ( static_cast< ComponentType* >( component ) == m_Component )
        {
            if ( m_Component != nullptr && m_OnDisconnectCallback )
            {
                m_OnDisconnectCallback( m_Component );
            }

            m_Component = nullptr;
        }
    }


//-----------------------------------------------------------------------------
