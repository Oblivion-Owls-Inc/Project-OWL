/// @file       EntityReference.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to an Entity in the scene
/// @version    0.1
/// @date       2024-02-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "ISerializable.h"

#include <set>

class Entity;
class Component;

class ComponentReferenceBase;


/// @brief  a reference to an Entity in the scene
class EntityReference : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    /// @param  componentReferences references to the Components attached to the Entity to keep track of
    EntityReference( std::initializer_list< ComponentReferenceBase* > componentReferences );


    /// @brief  copy constructor
    /// @param  other               the EntityReference to copy
    /// @param  componentReferences references to the Components attached to the Entity to keep track of
    EntityReference( EntityReference const& other, std::initializer_list< ComponentReferenceBase* > componentReferences );


    /// @brief  disable default assignment operator
    void operator =( EntityReference const& ) = delete;

    /// @brief  disable default copy constructor
    EntityReference( EntityReference const& ) = delete;


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this EntityReference to point to an Entity in the scene
    void Init();


    /// @brief  exits this EntityReference, stopping keeping track of any Entities
    void Exit();


    /// @brief  sets the callback to call when this EntityReference connects to an Entity
    /// @param  callback    the callback to call
    void SetOnConnectCallback( std::function< void () > callback );

    /// @brief  sets the callback to call when this EntityReference disconnects from an Entity
    /// @param  callback    the callback to call
    void SetOnDisconnectCallback( std::function< void () > callback );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the Entity
    /// @return the name of the Entity
    std::string const& GetName() const;


    /// @brief  sets this EntityReference's Owner Name
    /// @param  ownerName   the name of the owner of this EntityReference
    void SetOwnerName( std::string const& ownerName );


    /// @brief  dereference operator
    /// @return the Component this EntityReference refers to
    Entity& operator *() const;

    /// @brief  member dereference operator
    /// @return the Component this EntityReference refers to
    Entity* operator ->() const;

    /// @brief  implicit cast operator
    /// @return this EntityReference's internal pointer
    operator Entity*() const;


    /// @brief  Entity* assignment operator
    /// @param  entity  the Entity to assign to this EntityReference
    void operator =( Entity* entity );

    
//-----------------------------------------------------------------------------
public: // engine methods
//-----------------------------------------------------------------------------


    /// @brief  sets this EntityReference to nullptr
    /// @brief  SHOULD ONLY BE CALLED BY Entity::Exit()
    void Clear();


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the name of the Entity this EntityReference points to
    std::string m_EntityName = "";

    /// @brief  the component this EntityReference is referring to
    Entity* m_Entity = nullptr;


    /// @brief  the name of the owner of this EntityReference (for debug logging)
    std::string m_OwnerName = "";


    /// @brief  ComponentReferences attached to this EntityReference
    std::set< ComponentReferenceBase* > const m_ComponentReferences;


    /// @brief  function that gets called when this EntityReference connects to an Entity
    std::function< void () > m_OnConnectCallback = nullptr;

    /// @brief  function that gets called before this EntityReference connects to an Entity
    std::function< void () > m_OnDisconnectCallback = nullptr;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  inits all attached ComponentReferences
    void initComponentReferencesAndCallbacks();


    /// @brief  exits all attached ComponentReferences
    void exitComponentReferencesAndCallbacks();

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this EntityReference
    /// @param  label   what to label this EntityReference inspector as
    /// @return whether this EntityReference was modified
    bool Inspect( char const* label );

    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Entity this EntityReference points to
    /// @param  data    the JSON data to read from
    void readEntityName( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for Entities
    /// @return the map of read methods for Entities
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all Entity data to a JSON file.
    /// @return The JSON file containing the Entity data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
