/// @file       Popup.h
/// @author    jax.clayton (jax.clayton@digipen.edu)
/// @brief     A popup is a behavior that will show a popup when a button is pressed
/// @version    0.1
/// @date       3/3/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Behavior.h"
#include "AssetReference.h"
#include "ActionReference.h"

/// @brief  A Popup is a behavior that will show a popup when a button is pressed
class Popup : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    Popup();


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

    /// @brief  called once per frame
    virtual void OnFixedUpdate();

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief  the entity that will be used as the popup
    AssetReference< Entity > m_PopupEntity;

    /// @brief  the button that will be used to open the popup
    ActionReference m_PopupButton;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for Popup
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the PopupEntity from JSON
    void ReadPopupEntity(nlohmann::ordered_json const& data);

    /// @brief  reads the PopupButton from JSON
    void ReadPopupButton(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


        /// @brief  gets the map of read methods for this Popup
        /// @return the map of read methods for this Popup
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this Popup to JSON
    /// @return the JSON data of this Popup
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


        /// @brief  clones this Popup
        /// @return the newly created clone of this Popup
    virtual Popup* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


        /// @brief  copy-constructor for the Popup
        /// @param  other   the other Popup to copy
    Popup(Popup const& other);


    // disable assignment operator
    void operator =(Popup const&) = delete;


//-----------------------------------------------------------------------------
};
