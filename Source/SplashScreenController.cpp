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
#include "SplashScreenController.h"
#include "BehaviorSystem.h" // AddBehavior, RemoveBehavior
#include "InputSystem.h"    // GetKeyDown
#include "SceneSystem.h"    // GetInstance, SetNextScene
#include "DebugSystem.h"    // ImGui::DragFloat

///----------------------------------------------------------------------------
/// Public: constructor / destructor
///----------------------------------------------------------------------------
SplashScreenController::SplashScreenController()
    : Behavior(typeid(SplashScreenController))
{}

///----------------------------------------------------------------------------
/// Public: methods
///----------------------------------------------------------------------------

/// @brief Adds this behavior to the behavior system on init.
void SplashScreenController::OnInit()
{
    Behaviors<Behavior>()->AddComponent(this);

    // Initialises all the references.
    m_RigidBody.Init(GetEntity());
    m_Transform.Init(GetEntity());
    m_Sprite.Init(GetEntity());

    for (LogoData& data : m_Logos)
    {
        data.m_LogoTexture.SetOwnerName(GetName());
        data.m_LogoTexture.Init();
    }
}

/// @brief Removes this behavior from the behavior system on exit.
void SplashScreenController::OnExit()
{
    Behaviors<Behavior>()->RemoveComponent(this);

    m_RigidBody.Exit();
    m_Transform.Exit();
    m_Sprite.Exit();
}


/// @brief On fixed update to display logos.
void SplashScreenController::OnFixedUpdate()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    if (m_RigidBody == nullptr ||
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

    // Swaps the logo when the current timer runs out.
    if (m_Logos.back().m_LogoTimer < 0)
    {
        m_Logos.pop_back();
        m_Transform->SetScale(m_Logos.back().m_LogoScale);
        m_Sprite->SetTexture(m_Logos.back().m_LogoTexture);
    }

    // Displays the texture for a set amount of time.
    if (m_Logos.back().m_LogoTimer > 0.0f)
    {
        m_Logos.back().m_LogoTimer -= GameEngine()->GetFixedFrameDuration();
        Debug() << m_Logos.back().m_LogoTimer << std::endl;
    }
    

    // When the last timer runs out switch scenes.
    if (m_Logos.front().m_LogoTimer < 0)
    {
        SceneSystem::GetInstance()->SetNextScene(m_NextSceneName);
    }
}

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

/// @brief  Writes all data in this class into a JSON file.
/// @return The JSON object containing the data.
nlohmann::ordered_json SplashScreenController::Write() const
{
    nlohmann::ordered_json data;

    data["NextSceneName"] = Stream::Write(m_NextSceneName);
    data["Logos"] = Stream::WriteArray(m_Logos);

    return data;
}

/// @brief  Clones this instance of splash screen controller.
/// @return A clone of this splash screen controller instance.
SplashScreenController* SplashScreenController::Clone() const
{
    return new SplashScreenController(*this);
}

/// @brief Copy Constructor.
/// @param other Reference to another splashscreen controller.
SplashScreenController::SplashScreenController(SplashScreenController const& other)
    :Behavior(other)
    ,m_NextSceneName(other.m_NextSceneName)
{}

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  Inspects the an instance of LogoData
/// @return Whether or not any data was changed
bool SplashScreenController::LogoData::Inspect()
{
    bool dataChanged = false;

    // Timer inspector component
    dataChanged |= ImGui::DragFloat("Logo Timer", &m_LogoTimer, 0.05f, 0.0f, INFINITY);

    // Aspect Ratio inspector component.
    dataChanged |= ImGui::DragFloat("Logo Aspect Ratio", &m_LogoAspectRatio, 0.05f, 0.0f, INFINITY);

    // Logo texture inspector component.
    dataChanged |= m_LogoTexture.Inspect("Logo Texture");

    dataChanged |= ImGui::DragFloat2("Logo Scale", &m_LogoScale[0], 0.05f, INFINITY);

    return dataChanged;
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief Read the data for the timer from a JSON file.
/// @param data the JSON file to read from.
void SplashScreenController::LogoData::readTimer(nlohmann::ordered_json const& data)
{
    Stream::Read(m_LogoTimer, data);
}

/// @brief Read the data for the aspect ratio from a JSON file.
/// @param data the JSON file to read from.
void SplashScreenController::LogoData::readAspectRatio(nlohmann::ordered_json const& data)
{
    Stream::Read(m_LogoAspectRatio, data);
}

/// @brief Read the logo texture from a JSON file.
/// @param data the JSON file to read from.
void SplashScreenController::LogoData::readLogo(nlohmann::ordered_json const& data)
{
    Stream::Read(m_LogoTexture, data);
}

/// @brief Read the scale of an individual logo.
/// @param data the JSON file to read from.
void SplashScreenController::LogoData::readScale(nlohmann::ordered_json const& data)
{
   m_LogoScale = Stream::Read< 2, float >(data);
}

/// @brief Return a map of the read methods for this class.
/// @return A map of this class' read methods.
ReadMethodMap<ISerializable> const& SplashScreenController::LogoData::GetReadMethods() const
{
    static ReadMethodMap<LogoData> const readMethodsMap = {

        // If this does not work include splashscreen controller
        { "Timer"       , &LogoData::readTimer       },
        { "AspectRatio" , &LogoData::readAspectRatio },
        { "Logo"        , &LogoData::readLogo        },
        { "Scale"       , &LogoData::readScale       }
    };

    return (ReadMethodMap< ISerializable > const&)readMethodsMap;
}

/// @brief Writes a logo's data to a file.
/// @return The JSON file containing the written ata.
nlohmann::ordered_json SplashScreenController::LogoData::Write() const
{
    nlohmann::ordered_json data;

    data["Timer"] = Stream::Write(m_LogoTimer);
    data["AspectRatio"] = Stream::Write(m_LogoAspectRatio);
    data["Logo"] = Stream::Write(m_LogoTexture);
    data["Scale"] = Stream::Write(m_LogoScale);

    return data;
}
