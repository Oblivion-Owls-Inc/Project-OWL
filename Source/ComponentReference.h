/// @file       ComponentReference.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to a component in the scene
/// @version    0.1
/// @date       2024-02-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#define COMPONENTREFERENCE_H

#include "Entity.h"


/// @brief  abstract base class for templatized ComponentReferences
class ComponentReferenceBase
{
//-----------------------------------------------------------------------------
public: // virtual methods
//-----------------------------------------------------------------------------


    // abstract destructor
    virtual ~ComponentReferenceBase() {};


    /// @brief  initializes this ComponentReference to point to the appropriate Component attached to the specified Entity
    /// @param  entity  the entity to look for this ComponentReference's ComponentType in
    virtual void Init( Entity* entity ) = 0;

    /// @brief  separates this ComponentReference from the target Entity
    /// @param  entity  the entity this ComponentReference is curently watching
    virtual void Exit( Entity* entity ) = 0;


    /// @brief  sets this ComponentReference to nullptr
    virtual void Clear() = 0;


    /// @brief  tries to set this ComponentReference to point to the component, checking if it's valid
    /// @param  component   the component to try to set this ComponentReference to
    virtual void TrySet( Component* component ) = 0;

    /// @brief  compares the currently pointed to Component with the specified Component, and clears it if they match
    /// @param  component   the component to compare with
    virtual void TryRemove( Component* component ) = 0;


//-----------------------------------------------------------------------------
};


/// @brief  a reference to a component in the scene
/// @tparam ComponentType   the type of Component this ComponentReference refers to
template < class ComponentType >
class ComponentReference : public ComponentReferenceBase
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ComponentReference();


    /// @brief  destructor
    virtual ~ComponentReference() override;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this ComponentReference to point to the Component of its type on the specified Entity
    /// @param  entity  the Entity this ComponentReference's Component is attached to
    virtual void Init( Entity* entity ) override;

    /// @brief  separates this ComponentReference from the target Entity
    /// @param  entity  the entity this ComponentReference is curently watching
    virtual void Exit( Entity* entity ) override;


    /// @brief  sets the callback to call when this ComponentReference connects to a Component
    /// @param  callback    the callback to call
    void SetOnConnectCallback( std::function< void ( ComponentType* component ) > callback );

    /// @brief  sets the callback to call when this ComponentReference disconnects from a Component
    /// @param  callback    the callback to call
    void SetOnDisconnectCallback( std::function< void ( ComponentType* component ) > callback );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  dereference operator
    /// @return the Component this ComponentReference refers to
    ComponentType& operator *() const;

    /// @brief  member dereference operator
    /// @return the Component this ComponentReference refers to
    ComponentType* operator ->() const;

    /// @brief  implicit cast operator
    /// @return this ComponentReference's internal pointer
    operator ComponentType*() const;


    /// @brief  assignment operator
    /// @param  component   the component to assign to this ComponentReference
    void operator =( ComponentType* component );


//-----------------------------------------------------------------------------
public: // engine methods
//-----------------------------------------------------------------------------


    /// @brief  sets this ComponentReference to nullptr
    virtual void Clear() override;


    /// @brief  tries to set this ComponentReference to point to the component, checking if it's valid
    /// @param  component   the component to try to set this ComponentReference to
    virtual void TrySet( Component* component ) override;

    /// @brief  compares the currently pointed to Component with the specified Component, and clears it if they match
    /// @param  component   the component to compare with
    virtual void TryRemove( Component* component ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the component this ComponentReference is referring to
    ComponentType* m_Component = nullptr;


    /// @brief  callback called when this ComponentReference connects to a Component
    std::function< void ( ComponentType* component ) > m_OnConnectCallback;

    /// @brief  callback called when this ComponentReference disconnects from a Component
    std::function< void ( ComponentType* component ) > m_OnDisconnectCallback;


//-----------------------------------------------------------------------------
};

#ifndef COMPONENTREFERENCE_T
#include "ComponentReference.t.cpp"
#endif
