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

    
}

/// @brief	called on exit, handles loss state
void DoomsDay::OnExit()
{
    Behaviors< DoomsDay >()->RemoveComponent(this);

    
}

/// @brief  called every frame
/// @param  dt delta time
void DoomsDay::OnUpdate(float dt)
{
    for (PlayerController* player : Behaviors< PlayerController >()->GetComponents())
    {
        float distance = glm::distance(
            player->GetEntity()->GetComponent<Transform>()->GetTranslation(),
            GetEntity()->GetComponent<Transform>()->GetTranslation()
        );

        if (distance <= m_LoseDistance)
        {
            Events()->BroadcastEvent< std::string >("LoseTheGame");
            Debug() << "Event Emitted: " << "LoseTheGame" << std::endl;
        }
    }
    
}

//-----------------------------------------------------------------------------
// copying
//-----------------------------------------------------------------------------

/// @brief  copy ctor
DoomsDay::DoomsDay(const DoomsDay& other) :
    Behavior(other)
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
}

void DoomsDay::setNormalSpeed()
{
}

void DoomsDay::setCloseSpeed()
{
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
    
};

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

/// @brief	write to json
nlohmann::ordered_json DoomsDay::Write() const
{
    nlohmann::ordered_json data;

    

    return data;
}

//-----------------------------------------------------------------------------