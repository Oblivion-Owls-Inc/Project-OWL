/// @file       ActionReference.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to an input Action
/// @version    0.1
/// @date       2024-02-12
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "ISerializable.h"

#include "InputSystem.h"


/// @brief  a reference to an Action
class ActionReference : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ActionReference();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this ActionReference
    void Init();

    /// @brief  exits this ActionReference
    void Exit();

    /// @brief  sets this ActionReference to nullptr
    void Clear();


    /// @brief  assignment operator
    /// @param  action  the Action to assign to this ActionReference
    void operator =( InputSystem::Action const* action );


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the asset
    /// @return the name of the asset
    std::string const& GetName() const;


    /// @brief  gets this ActionReference's Owner Name
    /// @return this ActionReference's Owner Name
    std::string const& GetOwnerName() const;

    /// @brief  sets this ActionReference's Owner Name
    /// @param  ownerName   the name of the owner of this ActionReference
    void SetOwnerName( std::string const& ownerName );


    /// @brief  dereference operator
    /// @return the Action this ActionReference refers to
    InputSystem::Action const& operator *() const;

    /// @brief  member dereference operator
    /// @return the Action this ActionReference refers to
    InputSystem::Action const* operator ->() const;

    /// @brief  implicit cast operator
    /// @return the Action this ActionReference refers to
    operator InputSystem::Action const*() const;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  assignment operator
    /// @param  other   the ActionReference to copy into this one
    void operator =( ActionReference const& other );

    /// @brief  copy constructor
    /// @param  other   the ActionReference to copy
    ActionReference( ActionReference const& other );


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the name of the Action this ActionReference points to
    std::string m_ActionName = "";

    /// @brief  the component this ActionReference is referring to
    InputSystem::Action const* m_Action = nullptr;


    /// @brief  the name of the owner of this ActionReference (for debug logging)
    std::string m_OwnerName = "";

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this ActionReference
    /// @param  label   what to label this ActionReference inspector as
    /// @return whether this ActionReference was modified
    bool Inspect( char const* label );

    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Action this ActionReference points to
    /// @param  data    the JSON data to read from
    void readActionName( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for ActionReferences
    /// @return the map of read methods for ActionReferences
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  write all ActionReference data to JSON.
    /// @return the JSON containing the ActionReference data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
