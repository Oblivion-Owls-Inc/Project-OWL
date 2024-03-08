/////////////////////////////////////////////////////////////////////////////////
/// @file       SceneChangeButton.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Component that changes the scene when clicked
/// @version    0.1
/// @date      2/21/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Component.h"
#include "EventListener.h"

/// @brief  Component that changes the scene when clicked
class SceneChangeButton : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    SceneChangeButton();


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

    /// @brief Listener for the button click
    EventListener<std::string> m_Listener;
   
    /// @brief  the name of the scene to change to
    std::string m_SceneName;

    /// @brief  the name of the event to listen for
    std::string m_EventName;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  called when the button is clicked to change the scene
    void onButtonClick() const;

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for SceneChangeButton
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the scene name from the JSON data
    void readSceneName( nlohmann::ordered_json const& data);

    /// @brief  reads the event name from the JSON data
    void readEventName( nlohmann::ordered_json const& data);


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------

    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this SceneChangeButton
    /// @return the newly created clone of this SceneChangeButton
    virtual SceneChangeButton* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the SceneChangeButton
    /// @param  other   the other SceneChangeButton to copy
    SceneChangeButton( SceneChangeButton const& other );


    // diable = operator
    void operator =( SceneChangeButton const& ) = delete;


//-----------------------------------------------------------------------------
};
