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
    /// @param  entity  the entity this ComponentReference is curently watching
    template < class ComponentType, bool required >
    void ComponentReference< ComponentType, required >::Exit( Entity* entity )
    {
        if ( m_Component != nullptr && m_OnDisconnectCallback )
        {
            m_OnDisconnectCallback();
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

        if ( m_Component != nullptr && m_OnConnectCallback )
        {
            m_OnConnectCallback();
        }
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
