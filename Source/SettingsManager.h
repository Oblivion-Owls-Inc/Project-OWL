/// @file       SettingsManager.h
/// @author     Aidan Straker (aidan.straker@digipen.edu)
/// @brief      Settings Manager Class
/// @version    0.1
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
    #include "Component.h"
    #include "ComponentReference.h"
    #include "EntityReference.h"

//-----------------------------------------------------------------------------
// Forward References
//-----------------------------------------------------------------------------
    class UiSlider;
    class UiElement;
    class UiBarSprite;


/// @brief  Settings Manager Class
class SettingsManager : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    SettingsManager();


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

    /// @brief The slider responsible for the volume of the game
    ComponentReference<UiSlider> m_MasterVolumeslider;

    /// @brief The slider responsible for the volume of special effects
    ComponentReference<UiSlider> m_SFXVolumeSlider;

    /// @brief The slider responsible for the volume of the in-game music
    ComponentReference<UiSlider> m_MusicVolumeSlider;

    /// @brief The entity for the master volume slider
    EntityReference m_MasterVolumeEntity = EntityReference({ &m_MasterVolumeslider });

    /// @brief The entity for the sfx slider.
    EntityReference m_SFXEntity = EntityReference({ &m_SFXVolumeSlider });

    /// @brief The entity for the music slider
    EntityReference m_MusicEntity = EntityReference({ &m_MusicVolumeSlider });

    /// @brief Name of the SFX audio channel
    std::string m_SFXChannelName = "";

    /// @brief Name of the music audio channel
    std::string m_MusicChannelName = "";


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for SettingsManager
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief Reads the volume slider to keep track of
    /// @param data The JSON file to read from
    void readMasterVolumeSlider(nlohmann::ordered_json const& data);

    /// @brief Reads the sfx slider to keep track of
    /// @param data The JSON to read from.
    void readSFXSlider(nlohmann::ordered_json const& data);

    /// @brief Reads the music slider
    /// @param data The JSON file to read from
    void readMusicSlider(nlohmann::ordered_json const& data);


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this SettingsManager
    /// @return the map of read methods for this SettingsManager
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this SettingsManager to JSON
    /// @return the JSON data of this SettingsManager
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this SettingsManager
    /// @return the newly created clone of this SettingsManager
    virtual SettingsManager* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the SettingsManager
    /// @param  other   the other SettingsManager to copy
    SettingsManager( SettingsManager const& other );


    // disable assignment operator
    void operator =( SettingsManager const& ) = delete;


//-----------------------------------------------------------------------------
};
