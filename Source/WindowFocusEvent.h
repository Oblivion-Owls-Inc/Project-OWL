/// @file       WindowFocusEvent.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Broadcasts events when the window gains/loses focus
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"


/// @brief  Broadcasts events when the window gains/loses focus
class WindowFocusEvent : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    WindowFocusEvent();


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

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  event to broadcast when the window gains focus
    std::string m_GainFocusEvent = "";

    /// @brief  event to broadcast when the window loses focus
    std::string m_LoseFocusEvent = "";


    /// @brief  whether to disable to broadcasting of events while the game is paused
    bool m_DisableWhenPaused = false;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for WindowFocusEvent
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads event to broadcast when the window gains focus
    /// @param  data    the JSON data to read from
    void readGainFocusEvent( nlohmann::ordered_json const& data );

    /// @brief  reads event to broadcast when the window loses focus
    /// @param  data    the JSON data to read from
    void readLoseFocusEvent( nlohmann::ordered_json const& data );

    /// @brief  reads whether to disable to broadcasting of events while the game is paused
    /// @param  data    the JSON data to read from
    void readDisableWhenPaused( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this WindowFocusEvent
    /// @return the map of read methods for this WindowFocusEvent
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;
    
    
    /// @brief  writes this WindowFocusEvent to JSON
    /// @return the JSON data of this WindowFocusEvent
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this WindowFocusEvent
    /// @return the newly created clone of this WindowFocusEvent
    virtual WindowFocusEvent* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the WindowFocusEvent
    /// @param  other   the other WindowFocusEvent to copy
    WindowFocusEvent( WindowFocusEvent const& other );


    // disable assignment operator
    void operator =( WindowFocusEvent const& ) = delete;


//-----------------------------------------------------------------------------
};
