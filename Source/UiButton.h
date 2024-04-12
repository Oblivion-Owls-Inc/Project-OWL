/// @file       UiButton.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui Button that sends an event when clicked
/// @version    0.1
/// @date       2024-01-31
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Behavior.h"

#include "ComponentReference.h"
#include "Sprite.h"
#include "AudioPlayer.h"
#include "UiElement.h"
#include "AssetReference.h"
#include "Sound.h"

/// @brief  Ui Button that sends an event when clicked
class UiButton : public Behavior
{
//-----------------------------------------------------------------------------
private: // types
//-----------------------------------------------------------------------------


    /// @brief  enum of what state the button is in
    enum class ButtonState
    {
        Idle,       /// @brief  the button is up and not hovered
        Hovered,    /// @brief  the button is up and hovered (buttons cannot be hovered if a different button is down)
        Down        /// @brief  the button is down (if a button is down, interactions with all other buttons are blocked)
    };

    
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    UiButton();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  adds a callback to be called whenever the button is clicked
    /// @param  ownerId     the ID of the owner of the callback to add
    /// @param  callback    the callback to add
    void AddOnClickedCallback( unsigned ownerId, std::function< void () > callback );

    /// @brief  removes a callback to be called whenever the button is clicked
    /// @param  ownerId     the ID of the owner of the callback to remove
    void RemoveOnOnClickedCallback( unsigned ownerId );


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


    /// @brief  called every graphics frame
    /// @param  dt  the duration of the frame
    virtual void OnUpdate( float dt ) override;

    /// @brief  called every simulation frame
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // configurable members
//-----------------------------------------------------------------------------


    /// @brief  the name of the event this UiButton broadcasts when it's clicked.
    std::string m_ClickEventName = "";


    /// @brief  whether this UiButton should activate when it's pressed instead of when it's released
    bool m_ShouldActivateOnPress = false;


    /// @brief  the frame the UiButton displays when idle
    unsigned m_IdleFrame = 0;

    /// @brief  the frame the UiButton displays when hovered
    unsigned m_HoveredFrame = 0;

    /// @brief  the frame the UiButton displays when down
    unsigned m_DownFrame = 0;


    /// @brief  the sound this UiButton plays when it is pressed
    AssetReference< Sound > m_PressSound;

    /// @brief  the sound this UiButton plays when it is released
    AssetReference< Sound > m_ReleaseSound;

    /// @brief  the sound this UiButton plays when it is hovered
    AssetReference< Sound > m_HoverSound;


//-----------------------------------------------------------------------------
private: // nonconfigurable members
//-----------------------------------------------------------------------------


    /// @brief  the current state of this Button
    ButtonState m_State = ButtonState::Idle;

    /// @brief  whether this Button was activated since the last FixedUpdate
    bool m_ActivatedSinceLastFixedUpdate = false;

    /// @brief  the Sprite Component attached to this UiButton
    ComponentReference< Sprite > m_Sprite;

    /// @brief  the AudioPlayer Component attached to this UiButton
    ComponentReference< AudioPlayer > m_AudioPlayer;


    /// @brief  callbacks to be called whenever this UiButton is clicked
    std::map< unsigned, std::function< void () > > m_OnClickedCallbacks = {};


//-----------------------------------------------------------------------------
private: // static members
//-----------------------------------------------------------------------------


    /// @brief  the button that is currently down. static to ensure only one button is interacted with at a time
    static UiButton const* s_currentlyDownButton;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  update that runs when this button is idle
    void updateWhenIdle();

    /// @brief  update that runs when this button is hovered
    void updateWhenHovered();

    /// @brief  update that runs when this button is down
    void updateWhenDown();


    /// @brief  checks if the mouse is currently over this button
    /// @return whether the mouse is over this button
    bool isMouseOver() const;


    /// @brief  calls all callbacks and events attached to this Button
    void callCallbacksAndEvents();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for UiButton
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the event this UiButton broadcasts when it's clicked.
    /// @param  data    the JSON data to read from
    void readClickEventName( nlohmann::ordered_json const& data );

    /// @brief  reads whether this UiButton should activate when it's pressed instead of when it's released
    /// @param  data    the JSON data to read from
    void readShouldActivateOnPress( nlohmann::ordered_json const& data );

    /// @brief  reads the frame the UiButton displays when idle
    /// @param  data    the JSON data to read from
    void readIdleFrame( nlohmann::ordered_json const& data );

    /// @brief  reads the frame the UiButton displays when hovered
    /// @param  data    the JSON data to read from
    void readHoveredFrame( nlohmann::ordered_json const& data );

    /// @brief  reads the frame the UiButton displays when down
    /// @param  data    the JSON data to read from
    void readDownFrame( nlohmann::ordered_json const& data );

    /// @brief  reads the sound this UiButton plays when it is pressed
    /// @param  data    the JSON data to read from
    void readPressSound( nlohmann::ordered_json const& data );

    /// @brief  reads the sound this UiButton plays when it is released
    /// @param  data    the JSON data to read from
    void readReleaseSound( nlohmann::ordered_json const& data );

    /// @brief  reads the sound this UiButton plays when it is hovered
    /// @param  data    the JSON data to read from
    void readHoverSound( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all AudioPlayr data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this UiButton
    /// @return the newly created clone of this UiButton
    virtual UiButton* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the UiButton
    /// @param  other   the other UiButton to copy
    UiButton( UiButton const& other );


    // diable = operator
    void operator =( UiButton const& ) = delete;


//-----------------------------------------------------------------------------
};
