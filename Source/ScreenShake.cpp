/////////////////////////////////////////////////////////////////////////////////
/// @file       ScreenShake.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      
/// @version    0.1
/// @date       3/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////
#include "pch.h" // precompiled header has to be included first
#include "ScreenShake.h"
#include "BehaviorSystem.h"
#include "Engine.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
ScreenShake::ScreenShake() :
    Behavior(typeid(ScreenShake))
{}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
void ScreenShake::OnInit()
{
    m_Listener.SetFilterFunction([&](std::string const& EventName) -> bool
    {
            return (EventName == m_EventName);
    });

    m_Listener.SetResponseFunction([&](std::string const& EventName)
	{
            m_Active = !m_Active;
	});

    m_Transform.Init(GetEntity());

    Behaviors<Behavior>()->AddComponent(this);
}

void ScreenShake::OnUpdate(float deltaTime)
{
    // Lerp the rotation back to 0
    float targetRotation = 0.0f; // Target rotation is 0
    float currentRotation = m_Transform->GetRotation();

    float rotationChange = 2 * deltaTime;

    // Ensure the rotationChange doesn't overshoot the target
    if (currentRotation < targetRotation) 
    {
        rotationChange = std::min(rotationChange, targetRotation - currentRotation);
    }
    else 
    {
        rotationChange = std::max(-rotationChange, targetRotation - currentRotation);
    }

    // Apply the rotation change
    float newRotation = currentRotation + rotationChange;

    m_Transform->SetRotation(newRotation);
}


/// @brief  called every fixed frame step
void ScreenShake::OnFixedUpdate()
{
    float deltaTime = GameEngine()->GetFixedFrameDuration();
    if (m_Active)
	{
		ShakeScreen( deltaTime );
	}
}

/// @brief  called once when exiting the scene
void ScreenShake::OnExit()
{
    m_Listener.Exit();
    
    Behaviors<Behavior>()->RemoveComponent(this);
}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  shakes the screen
void ScreenShake::ShakeScreen(float deltaTime)
{
    /// the timer for the shake
    static float shakeTimer = 0.0f;

    /// if the shake timer is less than or equal to 0
    if (shakeTimer <= 0.0f)
    {
        shakeTimer = m_ShakeDuration; // reset the timer
        m_Active = false; // turn off the shake
        m_Transform->SetRotation(0.0f); // reset the rotation
    }

    shakeTimer -= deltaTime; // decrement the timer
    
    // Calculate random displacement within shake range
    float shakeX = glm::linearRand(m_ShakeXRange.x, m_ShakeXRange.y);
    float shakeY = glm::linearRand(m_ShakeYRange.x, m_ShakeYRange.y);

    // Get current position
    glm::vec2 pos = m_Transform->GetTranslation();

    // Apply shake
    m_Transform->SetTranslation( glm::vec2( pos.x + shakeX, pos.y + shakeY ) );

    float shakeRotation = glm::linearRand(m_ShakeRotationRange.x, m_ShakeRotationRange.y);

    m_Transform->SetRotation( m_Transform->GetRotation() + shakeRotation );
}

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------

/// @brief  shows the inspector for ScreenShake
void ScreenShake::Inspector()
{
    ImGui::InputText("Event Name", &m_EventName);

    ImGui::DragFloat("Shake Duration", &m_ShakeDuration, 0.1f, 0.0f);

    ImGui::DragFloat2("Shake X Range", &m_ShakeXRange.x, 0.1f);

    ImGui::DragFloat2("Shake Y Range", &m_ShakeYRange.x, 0.1f);

    ImGui::DragFloat2("Shake Rotation Range", &m_ShakeRotationRange.x, 0.1f);

    ImGui::Checkbox("Test Shake", &m_Active);
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief  reads the event name from the JSON data
/// @param data    the JSON data to read from
void ScreenShake::readEventName(nlohmann::ordered_json const& data)
{
    Stream::Read(m_EventName, data);
}

/// @brief reads the shake duration from the JSON data
/// @param data - the JSON data to read from
void ScreenShake::readShakeDuration(nlohmann::ordered_json const& data)
{
   m_ShakeDuration = Stream::Read<float>(data);
}

/// @brief reads the shake x range from the JSON data
/// @param data - the JSON data to read from
void ScreenShake::readShakeXRange(nlohmann::ordered_json const& data)
{
    Stream::Read(&m_ShakeXRange, data);
}

/// @brief reads the shake y range from the JSON data
/// @param data - the JSON data to read froma
void ScreenShake::readShakeYRange(nlohmann::ordered_json const& data)
{
	Stream::Read(&m_ShakeYRange, data);
}

/// @brief reads the shake rotation range from the JSON data
/// @param data - the JSON data to read from
void ScreenShake::readShakeRotationRange(nlohmann::ordered_json const& data)
{
	Stream::Read(&m_ShakeRotationRange, data);
}

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ScreenShake
    /// @return the map of read methods for this ScreenShake
ReadMethodMap< ISerializable > const& ScreenShake::GetReadMethods() const
{
    static ReadMethodMap< ScreenShake > const readMethods = {
        { "EventName"    , &ScreenShake::readEventName      },
		{ "ShakeDuration", &ScreenShake::readShakeDuration  },
		{ "ShakeXRange",   &ScreenShake::readShakeXRange    },
		{ "ShakeYRange",   &ScreenShake::readShakeYRange    },
        { "ShakeRotationRange", &ScreenShake::readShakeRotationRange }
    };

    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  writes this ScreenShake to JSON
/// @return the JSON data of this ScreenShake
nlohmann::ordered_json ScreenShake::Write() const
{
    nlohmann::ordered_json json = nlohmann::ordered_json::object();

    json["EventName"] = Stream::Write(m_EventName);
    json["ShakeDuration"] = Stream::Write(m_ShakeDuration);
    json["ShakeXRange"] = Stream::Write(m_ShakeXRange);
    json["ShakeYRange"] = Stream::Write(m_ShakeYRange);
    json["ShakeRotationRange"] = Stream::Write(m_ShakeRotationRange);

    return json;
}

//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ScreenShake
    /// @return the newly created clone of this ScreenShake
ScreenShake* ScreenShake::Clone() const
{
    return new ScreenShake(*this);
}


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ScreenShake
    /// @param  other   the other ScreenShake to copy
ScreenShake::ScreenShake(ScreenShake const& other) :
    Behavior(other),
    m_EventName(other.m_EventName),
    m_ShakeDuration(other.m_ShakeDuration),
    m_ShakeXRange(other.m_ShakeXRange),
    m_ShakeYRange(other.m_ShakeYRange),
    m_ShakeRotationRange(other.m_ShakeRotationRange)
{}


//-----------------------------------------------------------------------------
