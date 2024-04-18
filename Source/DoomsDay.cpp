///*****************************************************************/
/// @file	    DoomsDay.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    04/14/2024
/// @brief      DoomsDay class implimentation
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#include "pch.h" // precompiled header has to be included first
#include "DoomsDay.h"

#include "ComponentSystem.h"
#include "BehaviorSystem.h"
#include "EventSystem.h"

#include "CollisionSystem.h"
#include "EnemyBehavior.h"
#include "PlayerController.h"

#include "ComponentReference.t.h"

#include "Health.h"
#include "Emitter.h"
#include "EmitterSprite.h"
#include "Interactor.h"
#include "Inventory.h"

//-----------------------------------------------------------------------------
// constructor / destructor 
//-----------------------------------------------------------------------------

/// @brief  constructor
DoomsDay::DoomsDay() :
    Behavior(typeid(DoomsDay))
{}

/// @brief  clone
DoomsDay* DoomsDay::Clone() const
{
    return new DoomsDay(*this);
}

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief	initialize DoomsDay
void DoomsDay::OnInit()
{
    Behaviors< Behavior >()->AddComponent(this);

    
}

/// @brief	called on exit, handles loss state
void DoomsDay::OnExit()
{
    Behaviors< Behavior >()->RemoveComponent(this);

    
}

/// @brief  called every frame
/// @param  dt delta time
void DoomsDay::OnUpdate(float dt)
{
    for (PlayerController* player : Behaviors< PlayerController >()->GetComponents())
    {
        float distance =
            player->GetEntity()->GetComponent<Transform>()->GetTranslation().y -
            GetEntity()->GetComponent<Transform>()->GetTranslation().y;

        distance -= 25;

        if (distance <= m_LoseDistance && m_HasLost == false)
        {
            Events()->BroadcastEvent< std::string >("DoomsdayLoseTheGame");
            Debug() << "Event Emitted: " << "DoomsdayLoseTheGame" << std::endl;
            m_HasLost = true;
        }

        if (distance <= m_NormalDistance)
        {
            setCloseSpeed();
        }
        else if (distance <= m_CatchupDistance)
        {
            setNormalSpeed();
        }
        else
        {
            setCatchupSpeed();
        }
    }
    
}

//-----------------------------------------------------------------------------
// copying
//-----------------------------------------------------------------------------

/// @brief  copy ctor
DoomsDay::DoomsDay(const DoomsDay& other) :
    Behavior(other),
    m_CatchupSpeed(other.m_CatchupSpeed),
    m_NormalSpeed(other.m_NormalSpeed),
    m_CloseSpeed(other.m_CloseSpeed),
    m_CatchupDistance(other.m_CatchupDistance),
    m_NormalDistance(other.m_NormalDistance),
    m_LoseDistance(other.m_LoseDistance)
{}


//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// private functions
//-----------------------------------------------------------------------------

void DoomsDay::setCatchupSpeed()
{
    GetEntity()->GetComponent<RigidBody>()->SetVelocity(
        glm::vec2{ 0, m_CatchupSpeed });
}

void DoomsDay::setNormalSpeed()
{
    GetEntity()->GetComponent<RigidBody>()->SetVelocity(
        glm::vec2{ 0, m_NormalSpeed });
}

void DoomsDay::setCloseSpeed()
{
    GetEntity()->GetComponent<RigidBody>()->SetVelocity(
        glm::vec2{ 0, m_CloseSpeed });
}

//-----------------------------------------------------------------------------
// inspector methods
//-----------------------------------------------------------------------------

/// @brief	inspector for DoomsDays
void DoomsDay::Inspector()
{
    ImGui::DragFloat("Close Speed", &m_CloseSpeed, 0.05f, 0.0f, INFINITY);
    ImGui::DragFloat("Lose distance", &m_LoseDistance, 0.05f, 0.0f, INFINITY);

    ImGui::DragFloat("Normal Distance", &m_NormalDistance, 0.05f, 0.0f, INFINITY);
    ImGui::DragFloat("Normal Speed", &m_NormalSpeed, 0.05f, 0.0f, INFINITY);
    
    ImGui::DragFloat("Catchup Distance", &m_CatchupDistance, 0.05f, 0.0f, INFINITY);
    ImGui::DragFloat("Catchup Speed", &m_CatchupSpeed, 0.05f, 0.0f, INFINITY);
}

//-----------------------------------------------------------------------------
// reading
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<DoomsDay> const DoomsDay::s_ReadMethods =
{
    { "CatchupSpeed"    , &DoomsDay::readCatchupSpeed    },
    { "NormalSpeed"     , &DoomsDay::readNormalSpeed     },
    { "CloseSpeed"      , &DoomsDay::readCloseSpeed      },
    { "CatchupDistance" , &DoomsDay::readCatchupDistance },
    { "NormalDistance"  , &DoomsDay::readNormalDistance  },
    { "LoseDistance"    , &DoomsDay::readLoseDistance    }
};

void DoomsDay::readCatchupSpeed(nlohmann::ordered_json const& json)
{
    Stream::Read(m_CatchupSpeed, json);
}

void DoomsDay::readNormalSpeed(nlohmann::ordered_json const& json)
{
    Stream::Read(m_NormalSpeed, json);
}

void DoomsDay::readCloseSpeed(nlohmann::ordered_json const& json)
{
    Stream::Read(m_CloseSpeed, json);
}

void DoomsDay::readCatchupDistance(nlohmann::ordered_json const& json)
{
    Stream::Read(m_CatchupDistance, json);
}

void DoomsDay::readNormalDistance(nlohmann::ordered_json const& json)
{
    Stream::Read(m_NormalDistance, json);
}

void DoomsDay::readLoseDistance(nlohmann::ordered_json const& json)
{
    Stream::Read(m_LoseDistance, json);
}

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

/// @brief	write to json
nlohmann::ordered_json DoomsDay::Write() const
{
    nlohmann::ordered_json data;

    data[ "CatchupSpeed"    ] = m_CatchupSpeed;
    data[ "NormalSpeed"     ] = m_NormalSpeed;
    data[ "CloseSpeed"      ] = m_CloseSpeed;
    data[ "CatchupDistance" ] = m_CatchupDistance;
    data[ "NormalDistance"  ] = m_NormalDistance;
    data[ "LoseDistance"    ] = m_LoseDistance;

    return data;
}

//-----------------------------------------------------------------------------