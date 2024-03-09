///--------------------------------------------------------------------------//
/// @file   SplashScreen.cpp
/// @brief  SplashScreenController Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   February 2024
///
/// @copyright (c) 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
/// 

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------

#include "pch.h" // precompiled header has to be included first
#include "SplashScreenController.h"
#include "BehaviorSystem.h" // AddBehavior, RemoveBehavior
#include "InputSystem.h"    // GetKeyTriggered, GetGamepadButtonDown
#include "SceneSystem.h"    // GetInstance, SetNextScene
#include "DebugSystem.h"    // ImGui::DragFloat
#include "Transform.h"      // SetScale
#include "Sprite.h"         // SetTexture
#include "ComponentReference.t.h"



//-----------------------------------------------------------------------------
// private: Inspection
//-----------------------------------------------------------------------------

    /// @brief  Inspects the an instance of LogoData
    /// @return Whether or not any data was changed
    bool SplashScreenController::LogoData::Inspect()
    {
        bool dataChanged = false;

        // Logo texture inspector component.
        dataChanged |= M_LogoTexture.Inspect("Logo Texture");

        // Timer inspector component
        dataChanged |= ImGui::DragFloat("Logo Timer", &M_LogoTimer, 0.05f, 0.0f, INFINITY);

        // Logo scale inspector component.
        dataChanged |= ImGui::DragFloat("Logo Scale", &M_LogoScale, 0.05f, 0.0f, INFINITY);

        return dataChanged;
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief Read the data for the timer from a JSON file.
    /// @param data the JSON file to read from.
    void SplashScreenController::LogoData::readTimer(nlohmann::ordered_json const& data)
    {
        Stream::Read(M_LogoTimer, data);
    }

    /// @brief Read the logo texture from a JSON file.
    /// @param data the JSON file to read from.
    void SplashScreenController::LogoData::readLogo(nlohmann::ordered_json const& data)
    {
        Stream::Read(M_LogoTexture, data);
    }

    /// @brief Read the scale of an individual logo.
    /// @param data the JSON file to read from.
    void SplashScreenController::LogoData::readScale(nlohmann::ordered_json const& data)
    {
        Stream::Read(M_LogoScale, data);
    }

    /// @brief Return a map of the read methods for this class.
    /// @return A map of this class' read methods.
    ReadMethodMap<ISerializable> const& SplashScreenController::LogoData::GetReadMethods() const
    {
        static ReadMethodMap<LogoData> const readMethodsMap = {

            // If this does not work include splashscreen controller
            { "Timer"       , &LogoData::readTimer       },
            { "Logo"        , &LogoData::readLogo        },
            { "Scale"       , &LogoData::readScale       }
        };

        return (ReadMethodMap< ISerializable > const&)readMethodsMap;
    }

//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------
    /// @brief Writes a logo's data to a file.
    /// @return The JSON file containing the written ata.
    nlohmann::ordered_json SplashScreenController::LogoData::Write() const
    {
        nlohmann::ordered_json data;

        data["Timer"] = Stream::Write(M_LogoTimer);
        data["Logo"] = Stream::Write(M_LogoTexture);
        data["Scale"] = Stream::Write(M_LogoScale);

        return data;
    }

///----------------------------------------------------------------------------
/// Public: constructor / destructor
///----------------------------------------------------------------------------
SplashScreenController::SplashScreenController()
    : Behavior(typeid(SplashScreenController))
    , m_Index(0)
    , m_Timer(0.0f)
{}

///----------------------------------------------------------------------------
/// Public: virtual override methods
///----------------------------------------------------------------------------

    /// @brief Adds this behavior to the behavior system on init.
    void SplashScreenController::OnInit()
    {
        Behaviors<Behavior>()->AddComponent(this);

        // Initialises all the references.
        m_Transform.Init(GetEntity());
        m_Sprite.Init(GetEntity());

        for (LogoData& data : m_Logos)
        {
            data.M_LogoTexture.SetOwnerName(GetName());
            data.M_LogoTexture.Init();
        }
        m_Timer = m_Logos[m_Index].M_LogoTimer;
    }

    /// @brief Removes this behavior from the behavior system on exit.
    void SplashScreenController::OnExit()
    {
        Behaviors<Behavior>()->RemoveComponent(this);

        m_Transform.Exit();
        m_Sprite.Exit();
    }

    /// @brief Display the logo every frame
    /// @param dt change in time.
    void SplashScreenController::OnUpdate( float dt )
    {
        // Sets the background to black
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        if (
            m_Sprite == nullptr    ||
            m_Transform == nullptr  )
        {
            return;
        }

        // Immediately switches to game.
        //TODO: get any button press functionality.
        if (Input()->GetKeyTriggered(GLFW_KEY_SPACE) || 
            Input()->GetGamepadButtonDown(GLFW_GAMEPAD_BUTTON_START))
        {
            SceneSystem::GetInstance()->SetNextScene(m_NextSceneName);
        }

        m_Timer -= dt;

        // Swaps the logo when the current timer runs out.
        if (m_Timer < 0)
        {
            m_Index++;
            // Move to the next scene.
            if (m_Index >= static_cast<int>(m_Logos.size()))
            {
                Scenes()->SetNextScene(m_NextSceneName);
                return;
            }
            m_Transform->SetScale(glm::vec2(m_Logos[m_Index].M_LogoScale));
            m_Sprite->SetTexture(m_Logos[m_Index].M_LogoTexture);
            m_Timer = m_Logos[m_Index].M_LogoTimer;
        }
    }

//-----------------------------------------------------------------------------
// public: Inspection
//-----------------------------------------------------------------------------
    /// @brief Inspector for this class
    void SplashScreenController::Inspector()
    {
        ImGui::InputText("Next Scene:", &m_NextSceneName);

        // Inspector for the logo data vector.
        Inspection::InspectArray< LogoData >(
            "Logos", &m_Logos,
            [](LogoData* logoData) -> bool
            {
                return logoData->Inspect();
            }
        );
    }

//--------------------------------------------------------------------------------
// private: Reading
//--------------------------------------------------------------------------------

    /// @brief Read in the name of the next scene
    /// @param data The JSON file to read from.
    void SplashScreenController::readSceneName(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_NextSceneName, data);
    }

    void SplashScreenController::readLogos(nlohmann::ordered_json const& data)
    {
        Stream::Read< LogoData >( &m_Logos, data);
    }

    /// @brief A map of the read methods for this class
    /// @return The map of this class's read methods.
    ReadMethodMap<ISerializable> const& SplashScreenController::GetReadMethods() const
    {
        static ReadMethodMap<SplashScreenController> const readMethodsMap = {

            { "NextSceneName" , &SplashScreenController::readSceneName },
            { "Logos"         , &SplashScreenController::readLogos     }
        };

        return (ReadMethodMap< ISerializable > const&)readMethodsMap;
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------
    /// @brief  Writes all data in this class into a JSON file.
    /// @return The JSON object containing the data.
    nlohmann::ordered_json SplashScreenController::Write() const
    {
        nlohmann::ordered_json data;

        data["NextSceneName"] = Stream::Write(m_NextSceneName);
        data["Logos"] = Stream::WriteArray(m_Logos);

        return data;
    }

//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------
    /// @brief  Clones this instance of splash screen controller.
    /// @return A clone of this splash screen controller instance.
    SplashScreenController* SplashScreenController::Clone() const
    {
        return new SplashScreenController(*this);
    }

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------
    /// @brief Copy Constructor.
    /// @param other Reference to another splashscreen controller.
    SplashScreenController::SplashScreenController(SplashScreenController const& other)
        :Behavior(other)
        ,m_NextSceneName(other.m_NextSceneName)
        ,m_Index(0)
        ,m_Timer(0.0f)
    {}


