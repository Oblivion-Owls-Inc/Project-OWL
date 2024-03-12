/// @file       ControlPrompt.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Displays a control prompt for an ActionReference
/// @version    0.1
/// @date       2024-03-11
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Component.h"

#include "ActionReference.h"

#include "ControlPromptSystem.h"

#include "ComponentReference.h"
class Sprite;


/// @brief  Displays a control prompt for an ActionReference
class ControlPrompt : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ControlPrompt();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the Sprite Component attached
    /// @return the Sprite Component attached
    Sprite* GetSprite();


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


    /// @brief  the Action to display a prompt for
    ActionReference m_Action;


    /// @brief  the Sprite to use to display the ControlPrompt
    ComponentReference< Sprite > m_Sprite;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  converts from an Action::InputType to a ControlPromptSystem::InputType
    /// @param  inputType   the Action::InputType to convert
    /// @return the ControlPromptSystem::InputType cooresponding to the Action::InputType
    ControlPromptSystem::InputType convertInputType( Action::InputType inputType );


    /// @brief  updates the attached Sprite to match the attached Action
    /// @brief  assumes m_Sprite is non-null, doesn't assume m_Action
    void updateSprite();


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ControlPrompt
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  read the Action to display a prompt for
    /// @param  data    the JSON data to read from
    void readAction( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ControlPrompt
    /// @return the map of read methods for this ControlPrompt
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this ControlPrompt to JSON
    /// @return the JSON data of this ControlPrompt
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ControlPrompt
    /// @return the newly created clone of this ControlPrompt
    virtual ControlPrompt* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ControlPrompt
    /// @param  other   the other ControlPrompt to copy
    ControlPrompt( ControlPrompt const& other );


    // disable assignment operator
    void operator =( ControlPrompt const& ) = delete;


//-----------------------------------------------------------------------------
};
