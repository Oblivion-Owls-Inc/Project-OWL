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
#include "UiButton.h"
#include "PlatformSystem.h"


//-----------------------------------------------------------------------------
// public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief Default Constructor
    SettingsManager::SettingsManager()
        :Component(typeid (SettingsManager))
    {}


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Called once entering the scene
    void SettingsManager::OnInit()
    {
        // Change the master volume with the master volume slider
        m_MasterVolumeSlider.SetOnConnectCallback([this]() 
        {
            m_MasterVolumeSlider->SetValue(Audio()->GetVolume());
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
            m_SFXVolumeSlider->SetValue(Audio()->GetVolume(m_SFXChannelName));
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
            m_MusicVolumeSlider->SetValue(Audio()->GetVolume(m_MusicChannelName));
            m_MusicVolumeSlider->AddOnSliderValueChangedCallback(GetId(), [this](float newValue)
            {
                Audio()->SetVolume(m_MusicChannelName, newValue);
            });
        });

        m_MusicVolumeSlider.SetOnDisconnectCallback([this]()
        {
            m_MusicVolumeSlider->RemoveOnSliderValueChangedCallback(GetId());
        });


        m_FullscreenToggleButton.SetOnConnectCallback([this]()
        {
                swapToggleTexture();

                m_FullscreenToggleButton->AddOnClickedCallback(GetId(), [this]()
                {
                        Platform()->SetFullscreen(!Platform()->GetFullscren());

                        swapToggleTexture();
                });
        });

        m_FullscreenToggleButton.SetOnDisconnectCallback([this]()
        {
            m_FullscreenToggleButton->RemoveOnOnClickedCallback(GetId());
        });
     

        m_MasterVolumeEntity.SetOwnerName(GetName());
        m_MasterVolumeEntity.Init();

        m_SFXEntity.SetOwnerName(GetName());
        m_SFXEntity.Init();

        m_MusicEntity.SetOwnerName(GetName());
        m_MusicEntity.Init();

        m_CheckedToggleTexture.SetOwnerName(GetName());
        m_CheckedToggleTexture.Init();
        m_UncheckedToggleTexture.SetOwnerName(GetName());
        m_UncheckedToggleTexture.Init();

        m_FullscreenToggleEntity.SetOwnerName(GetName());
        m_FullscreenToggleEntity.Init();
    }

    /// @brief Called once exiting scene
    void SettingsManager::OnExit()
    {
        m_MasterVolumeEntity    .Exit();
        m_SFXEntity             .Exit();
        m_MusicEntity           .Exit();
        m_FullscreenToggleEntity.Exit();
    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief Swaps the texture of the toggle
    void SettingsManager::swapToggleTexture()
    {
        if (m_FullscreenToggleSprite == nullptr)
        {
            return;
        }

        if (Platform()->GetFullscren())
        {
            m_FullscreenToggleSprite->SetTexture(m_CheckedToggleTexture);
        }
        else
        {
            m_FullscreenToggleSprite->SetTexture(m_UncheckedToggleTexture);
        }
    }

    
//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

    /// @brief The inspector for the Settings Manager
    void SettingsManager::Inspector()
    {
        m_MasterVolumeEntity    .Inspect("Master Volume Slider");
        m_SFXEntity             .Inspect("SFX Volume Slider");
        m_MusicEntity           .Inspect("Msuic Volume Slider");
        m_FullscreenToggleEntity.Inspect("Fullscreen Toggle");
        m_CheckedToggleTexture  .Inspect("Fullscreen Toggle Check Texture");
        m_UncheckedToggleTexture.Inspect("Fullscreen Toggle Unchecked Texture");

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
        Stream::Read(m_MusicChannelName, data);
    }

    /// @brief Reads in the asset for the checked check box
    /// @param data The JSON to read from
    void SettingsManager::readCheckedAsset(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_CheckedToggleTexture, data);
    }

    /// @brief Reads in the asset for the unchecked check box
    /// @param data The JSON to read from
    void SettingsManager::readUncheckedAsset(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_UncheckedToggleTexture, data);
    }

    /// @brief Reads in the data for the fullscreen toggle entity
    /// @param data The JSON to read from
    void SettingsManager::readFullscreenToggle(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_FullscreenToggleEntity, data);
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
            { "MusicChannelName"   , &SettingsManager::readMusicChannelName   },
            { "CheckedBox"         , &SettingsManager::readCheckedAsset       },
            { "UncheckedBox"       , &SettingsManager::readUncheckedAsset     },
            { "FullscreenToggle"   , &SettingsManager::readFullscreenToggle   }
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
        data["CheckedBox"] = m_CheckedToggleTexture.Write();
        data["UncheckedBox"] = m_UncheckedToggleTexture.Write();
        data["FullscreenToggle"] = Stream::Write(m_FullscreenToggleEntity);

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
        , m_FullscreenToggleEntity(other.m_FullscreenToggleEntity, {&m_FullscreenToggleButton, &m_FullscreenToggleSprite})
        , m_MasterVolumeEntity(other.m_MasterVolumeEntity, {&m_MasterVolumeSlider})
        , m_SFXEntity(other.m_SFXEntity, {&m_SFXVolumeSlider})
        , m_MusicEntity(other.m_MusicEntity, {&m_MusicVolumeSlider})
        , m_SFXChannelName(other.m_SFXChannelName)
        , m_MusicChannelName(other.m_MusicChannelName)
        , m_CheckedToggleTexture(other.m_CheckedToggleTexture)
        , m_UncheckedToggleTexture(other.m_UncheckedToggleTexture)
    {}


