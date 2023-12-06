///*****************************************************************/
/// @file	    BasicEntityBehavior.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    11/10/2023
/// @brief      BasicEntityBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/
#include "Entity.h"
#include "BasicEntityBehavior.h"
#include "BehaviorSystem.h"

///-----------------------------------------------------------------------------
/// public: methods
///-----------------------------------------------------------------------------

/// @brief Applies damage to the Enitity
/// @param damage - the amount of damage to apply
void BasicEntityBehavior::TakeDamage(int damage)
{
    Debug() << "Entity taking damage: " << damage << "\n";
    m_Health -= damage;
    if (!m_Health)
    {
        GetEntity()->Destroy();
    }
}

BasicEntityBehavior::BasicEntityBehavior(BasicEntityBehavior const& other):
    Behavior(other),
	m_Health(other.m_Health)
{
}

///-----------------------------------------------------------------------------
/// private: virtual override methods
///-----------------------------------------------------------------------------

/// @brief initializes the component and the health pool
void BasicEntityBehavior::OnInit()
{
	Behaviors<Behavior>()->AddBehavior(this);
	m_Health.OnInit();
}

/// @brief Called when the component is destroyed
void BasicEntityBehavior::OnExit()
{
	Behaviors<Behavior>()->RemoveBehavior(this);
}

/// @brief inspector for this component
void BasicEntityBehavior::Inspector()
{
    m_Health.Inspector();
}

///-----------------------------------------------------------------------------
/// Protected: reading
///-----------------------------------------------------------------------------

/// @brief Reads the health of the enemy from json
/// @param data - the json data to read from
void BasicEntityBehavior::readHealth(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Health, data);
}

///----------------------------------------------------------------------------
/// Public: Writing
///----------------------------------------------------------------------------
