/// @file       SettingsManager.h
/// @author     Aidan Straker (aidan.straker@digipen.edu)
/// @brief      Settings Manager Class
/// @version    0.1
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


#include "pch.h"
#include "SettingsManager.h"
#include "ComponentReference.t.h"

#include "UiSlider.h"
#include "UiElement.h"
#include "UiBarSprite.h"
#include "AudioSystem.h"


//-----------------------------------------------------------------------------
// public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief Default Constructor
    SettingsManager::SettingsManager()
        :Component(typeid (SettingsManager))
    {}


//-----------------------------------------------------------------------------
// public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Called once entering the scene
    void SettingsManager::OnInit()
    {
        // Change the master volume with the master volume slider
        m_MasterVolumeSlider.SetOnConnectCallback([this]() 
        {
            m_MasterVolumeSlider->AddOnSliderValueChangedCallback(GetId(), [this](float newValue)
            {
                Audio()->SetVolume(newValue);
            });
        });

        m_MasterVolumeSlider.SetOnDisconnectCallback([this]()
        {
            m_MasterVolumeSlider->RemoveOnSliderValueChangedCallback(GetId());
        });

        // Change the sfx volume with the sfx volume slider
        m_SFXVolumeSlider.SetOnConnectCallback([this]()
        {
            m_SFXVolumeSlider->AddOnSliderValueChangedCallback(GetId(), [this](float newValue)
            {
                Audio()->SetVolume(m_SFXChannelName, newValue);
            });
        });

        m_SFXVolumeSlider.SetOnDisconnectCallback([this]()
        {
            m_SFXVolumeSlider->RemoveOnSliderValueChangedCallback(GetId());
        });

        // Change the music volume with the music volume slider
        m_MusicVolumeSlider.SetOnConnectCallback([this]()
        {
            m_SFXVolumeSlider->AddOnSliderValueChangedCallback(GetId(), [this](float newValue)
            {
                Audio()->SetVolume(m_MusicChannelName, newValue);
            });
        });

        m_MusicVolumeSlider.SetOnDisconnectCallback([this]()
        {
            m_MusicVolumeSlider->RemoveOnSliderValueChangedCallback(GetId());
        });

        m_MasterVolumeEntity.SetOwnerName(GetName());
        m_MasterVolumeEntity.Init();

        m_SFXEntity.SetOwnerName(GetName());
        m_SFXEntity.Init();

        m_MusicEntity.SetOwnerName(GetName());
        m_MusicEntity.Init();
    }

    /// @brief Called once exiting scene
    void SettingsManager::OnExit()
    {
        m_MasterVolumeEntity.Exit();
        m_SFXEntity         .Exit();
        m_MusicEntity       .Exit();
    }


//-----------------------------------------------------------------------------
// public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief The inspector for the Settings Manager
    void SettingsManager::Inspector()
    {
        m_MasterVolumeEntity.Inspect("Master Volume Slider");
        m_SFXEntity         .Inspect("SFX Volume Slider");
        m_MusicEntity       .Inspect("Msuic Volume Slider");

        // The actual audio channels
        Audio()->InspectChannelGroup("SFX Group", &m_SFXChannelName, nullptr);
        Audio()->InspectChannelGroup("Music Group", &m_MusicChannelName, nullptr);
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief Reads the volume slider to keep track of
    /// @param data The JSON file to read from
    void SettingsManager::readMasterVolumeSlider(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_MasterVolumeEntity, data);
    }

    /// @brief Reads the sfx slider to keep track of
    /// @param data The JSON to read from.
    void SettingsManager::readSFXSlider(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_SFXEntity, data);
    }

    /// @brief Reads the music slider
    /// @param data The JSON file to read from
    void SettingsManager::readMusicSlider(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_MusicEntity, data);
    }

    /// @brief Reads in the name of SFX channel
    /// @param data The JSON file to read from
    void SettingsManager::readSFXChannelName(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_SFXChannelName, data);
    }

    /// @brief Reads in the name of the music channel
    /// @param data The JSON file to reda from
    void SettingsManager::readMusicChannelName(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_SFXChannelName, data);
    }

//-----------------------------------------------------------------------------
// reading / writing
//-----------------------------------------------------------------------------

    /// @brief  Gets the map of the read methods for this component
    /// @return The map of read methods for this component.
    ReadMethodMap<ISerializable> const& SettingsManager::GetReadMethods() const
    {
        static ReadMethodMap< SettingsManager > const readMethods = {
            { "MasterVolumeSlider" , &SettingsManager::readMasterVolumeSlider },
            { "SFXVolumeSlider"    , &SettingsManager::readSFXSlider          },
            { "MusicVolumeSlider"  , &SettingsManager::readMusicSlider        },
            { "SFXChannelName"     , &SettingsManager::readSFXChannelName     },
            { "MusicChannelName"   , &SettingsManager::readMusicChannelName   }
        };

        return (ReadMethodMap<ISerializable> const&) readMethods;
    }

    /// @brief  Write all the SettingsManager data to a JSON file.
    /// @return The JSON file containing the SettingsManager data
    nlohmann::ordered_json SettingsManager::Write() const
    {
        nlohmann::ordered_json data;

        data["MasterVolumeSlider"] = Stream::Write(m_MasterVolumeEntity);
        data["SFXVolumeSlider"] = Stream::Write(m_SFXEntity);
        data["MusicVolumeSlider"] = Stream::Write(m_MusicEntity);
        data["SFXChannelName"] = Stream::Write(m_SFXChannelName);
        data["MusicChannelName"] = Stream::Write(m_MusicChannelName);

        return data;
    }

//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------

    /// @brief  Clone this SettingsManager
    /// @return A newly created instance of SettingsManager
    SettingsManager* SettingsManager::Clone() const
    {
        return new SettingsManager(*this);
    }

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief Copy Constructor
    /// @param other Reference to another instance of SettingsManager
    SettingsManager::SettingsManager(const SettingsManager& other)
        : Component(other)
        , m_SFXChannelName(other.m_SFXChannelName)
        , m_MusicChannelName(other.m_MusicChannelName)
    {}


