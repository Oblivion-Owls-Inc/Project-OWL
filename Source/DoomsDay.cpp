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
    Behaviors< DoomsDay >()->AddComponent(this);

    m_RigidBody.Init(GetEntity());
    m_Transform.Init(GetEntity());
    
}

/// @brief	called on exit, handles loss state
void DoomsDay::OnExit()
{
    Behaviors< DoomsDay >()->RemoveComponent(this);

    m_RigidBody.Exit();
    m_Transform.Exit();
    
}

/// @brief  called every frame
/// @param  dt delta time
void DoomsDay::OnFixedUpdate()
{
    for (PlayerController* player : Behaviors< PlayerController >()->GetComponents())
    {
        float distance =
            player->GetTransform()->GetTranslation().y -
            m_Transform->GetTranslation().y;

        distance -= 25;

        if (distance <= m_LoseDistance && m_HasLost == false)
        {
            Events()->BroadcastEvent< std::string >(m_LossEventName);
            Debug() << "Event Emitted: " << m_LossEventName << std::endl;
            
            for (DoomsDay* doomsday : Behaviors< DoomsDay >()->GetComponents())
            {
                doomsday->SetHasLost();
            }
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
    m_LoseDistance(other.m_LoseDistance),
    m_LossEventName(other.m_LossEventName),
{}


//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

void DoomsDay::SetHasLost()
{
    m_HasLost = true;
}

//-----------------------------------------------------------------------------
// private functions
//-----------------------------------------------------------------------------

void DoomsDay::setCatchupSpeed()
{
    if (m_RigidBody)
    {
        m_RigidBody->SetVelocity( glm::vec2{ 0, m_CatchupSpeed });
    }
}

void DoomsDay::setNormalSpeed()
{
    if (m_RigidBody)
    {
        m_RigidBody->SetVelocity(glm::vec2{ 0, m_NormalSpeed });
    }
}

void DoomsDay::setCloseSpeed()
{
    if (m_RigidBody)
    {
        m_RigidBody->SetVelocity(glm::vec2{ 0, m_CloseSpeed });
    }
}

//-----------------------------------------------------------------------------
// inspector methods
//-----------------------------------------------------------------------------

/// @brief	inspector for DoomsDays
void DoomsDay::Inspector()
{
    ImGui::DragFloat("Close Speed", &m_CloseSpeed, 0.05f, 0.0f, INFINITY);
    ImGui::DragFloat("Lose distance", &m_LoseDistance, 0.05f, 0.0f, INFINITY);

    ImGui::Separator();
    ImGui::NewLine();

    ImGui::DragFloat("Normal Distance", &m_NormalDistance, 0.05f, 0.0f, INFINITY);
    ImGui::DragFloat("Normal Speed", &m_NormalSpeed, 0.05f, 0.0f, INFINITY);

    ImGui::Separator();
    ImGui::NewLine();

    ImGui::DragFloat("Catchup Distance", &m_CatchupDistance, 0.05f, 0.0f, INFINITY);
    ImGui::DragFloat("Catchup Speed", &m_CatchupSpeed, 0.05f, 0.0f, INFINITY);

    ImGui::Separator();
    ImGui::NewLine();


    ImGui::InputText("Loss Event Name", &m_LossEventName);
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
    { "LoseDistance"    , &DoomsDay::readLoseDistance    },
    { "LossEventName"   , &DoomsDay::readLossEventName   }
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

void DoomsDay::readLossEventName(nlohmann::ordered_json const& json)
{
    Stream::Read(m_LossEventName, json);
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
    data[ "LossEventName"   ] = m_LossEventName;

    return data;
}

//-----------------------------------------------------------------------------