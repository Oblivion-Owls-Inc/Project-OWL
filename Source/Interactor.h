/// @file       Interactor.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Behavior that can interact with nearby Interactable Components
/// @version    0.1
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Behavior.h"

#include "ComponentReference.h"
class Transform;
class Interactable;


/// @brief  Behavior that can interact with nearby Interactable Components
class Interactor : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    Interactor();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  tries to interact with the currently targeted Interactable
    void TryInteract();


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Transform Component attached to this Interactor
    /// @return the Transform Component attached to this Interactor
    Transform const* GetTransform() const;


    /// @brief  gets the Interactable Component currently being targeted by this Interactor
    /// @return the Interactable Component currently being targeted by this Interactor
    Interactable* GetTargetedInteractable();


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  called every simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the Transform Component attached to this Interactor
    ComponentReference< Transform > m_Transform;


    /// @brief  the Interactable Component currently being targeted by this Interactor
    ComponentReference< Interactable > m_TargetedInteractable;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  targets the nearest Interactable within range
    void updateTargetedInteractable();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    // /// @brief  shows the inspector for Interactor
    // virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    // /// @brief  gets the map of read methods for this Interactor
    // /// @return the map of read methods for this Interactor
    // virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;
    // 
    // 
    // /// @brief  writes this Interactor to JSON
    // /// @return the JSON data of this Interactor
    // virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Interactor
    /// @return the newly created clone of this Interactor
    virtual Interactor* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Interactor
    /// @param  other   the other Interactor to copy
    Interactor( Interactor const& other );


    // disable assignment operator
    void operator =( Interactor const& ) = delete;


//-----------------------------------------------------------------------------
};
