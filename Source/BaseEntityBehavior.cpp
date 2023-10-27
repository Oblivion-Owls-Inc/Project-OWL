#include "BaseEntityBehavior.h"
#include "BehaviorSystem.h"
#include "Entity.h"



///-----------------------------------------------------------------------------
/// public: methods
///-----------------------------------------------------------------------------

/// @brief Applies damage to the Enitity
/// @param damage - the amount of damage to apply
void BaseEntityBehavior::TakeDamage(int damage)
{
    m_Health -= damage;
    if (!m_Health)
    {
        GetParent()->Destroy();
    }
}

BaseEntityBehavior::BaseEntityBehavior(BaseEntityBehavior const& other):
	Behavior(other),
	m_Health(other.m_Health)
{
}

///-----------------------------------------------------------------------------
/// private: virtual override methods
///-----------------------------------------------------------------------------

/// @brief initializes the component and the health pool
void BaseEntityBehavior::OnInit()
{
	Behaviors<Behavior>()->AddBehavior(this);
	m_Health.OnInit();
}

/// @brief Called when the component is destroyed
void BaseEntityBehavior::OnExit()
{
	Behaviors<Behavior>()->RemoveBehavior(this);
}

/// @brief inspector for this component
void BaseEntityBehavior::Inspector()
{
    m_Health.Inspector();
}

///-----------------------------------------------------------------------------
/// private: reading
///-----------------------------------------------------------------------------

/// @brief Reads the health of the enemy from json
/// @param data - the json data to read from
void BaseEntityBehavior::readHealth(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Health, data);
}

/// @brief map of read methods
ReadMethodMap<BaseEntityBehavior> const BaseEntityBehavior::s_ReadMethods = {
	{ "Health", &BaseEntityBehavior::readHealth }
};
///----------------------------------------------------------------------------
/// Public: Writing
///----------------------------------------------------------------------------

/// @brief  write all component data to a JSON object
nlohmann::ordered_json BaseEntityBehavior::Write() const
{
    nlohmann::ordered_json data;
    data["Health"] = m_Health.Write();
    return data;
}

