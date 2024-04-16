/////////////////////////////////////////////////////////////////////////////////
/// @file       ScreenShake.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      
/// @version    0.1
/// @date       4/16/2024
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

    m_Transform.Exit();
    
    Behaviors<Behavior>()->RemoveComponent(this);
}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  shakes the screen
void ScreenShake::ShakeScreen(float deltaTime)
{
    static float shakeTimer = 0.0f;

    if (shakeTimer <= 0.0f)
    {
        shakeTimer = m_ShakeDuration;
        m_Active = false;
    }
    else
    {
        shakeTimer -= deltaTime;
    }

    glm::vec2 currentPOS = m_Transform->GetTranslation();

    /// use a ternary operator to make sure its not zero
    int shakeYRange = (m_ShakeYRange.y - m_ShakeYRange.x) ? (m_ShakeYRange.y - m_ShakeYRange.x) : 1;

    int shakeXRange = (m_ShakeXRange.y - m_ShakeXRange.x) ? (m_ShakeXRange.y - m_ShakeXRange.x) : 1;

    float y = m_ShakeYRange.x + (rand() % shakeYRange);
    float x = m_ShakeXRange.x + (rand() % shakeXRange);

    m_Transform->SetTranslation(glm::vec2(currentPOS.x + x, currentPOS.y + y) );
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
		{ "ShakeYRange",   &ScreenShake::readShakeYRange    }
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
    m_ShakeYRange(other.m_ShakeYRange)
{}


//-----------------------------------------------------------------------------
