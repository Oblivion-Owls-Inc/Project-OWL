/////////////////////////////////////////////////////////////////////////////////
/// @file       pause.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      component that pauses the game
/// @version    0.1
/// @date       3/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "behavior.h"
#include "ActionReference.h"
#include "EventListener.h"

/// @brief  DESCRIPTION HERE
class PauseComponent : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    PauseComponent();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief Called at a fixed framestep
    virtual void OnFixedUpdate();

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief The button that pauses the game
    ActionReference m_PauseButton;

    /// @brief The Event Listener for the PauseComponent button
    EventListener<std::string> m_PauseListener;

    /// @brief The name of the event to listen for
    std::string m_EventName;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------

    /// @brief  shows the inspector for PauseComponent
    virtual void Inspector() override;
    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief reads the pause button from JSON
    void readPauseButton(nlohmann::ordered_json const& data);

    /// @brief reads the event name from JSON
    void readEventName(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this PauseComponent
    /// @return the map of read methods for this PauseComponent
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this PauseComponent to JSON
    /// @return the JSON data of this PauseComponent
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this PauseComponent
    /// @return the newly created clone of this PauseComponent
    virtual PauseComponent* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the PauseComponent
    /// @param  other   the other PauseComponent to copy
    PauseComponent( PauseComponent const& other );


    // disable assignment operator
    void operator =( PauseComponent const& ) = delete;


//-----------------------------------------------------------------------------
};
