#include "BaseEntityBehavior.h"
#include "BehaviorSystem.h"
#include "Entity.h"



///-----------------------------------------------------------------------------
/// public: methods
///-----------------------------------------------------------------------------

void BaseEntityBehavior::TakeDamage(int damage)
{
    m_Health -= damage;
    if (!m_Health)
    {
        GetParent()->Destroy();
    }
}

///-----------------------------------------------------------------------------
/// private: virtual override methods
///-----------------------------------------------------------------------------

void BaseEntityBehavior::OnInit()
{
	Behaviors<Behavior>()->AddBehavior(this);
	m_Health.OnInit();
}

void BaseEntityBehavior::OnExit()
{
	Behaviors<Behavior>()->RemoveBehavior(this);
}

void BaseEntityBehavior::Inspector()
{
    m_Health.Inspector();
}

///-----------------------------------------------------------------------------
/// private: reading
///-----------------------------------------------------------------------------

void BaseEntityBehavior::readHealth(nlohmann::ordered_json const& data)
{
    Stream::Read(m_Health, data);
}


ReadMethodMap<BaseEntityBehavior> const BaseEntityBehavior::s_ReadMethods = {
	{ "Health", &BaseEntityBehavior::readHealth }
};
///----------------------------------------------------------------------------
/// Public: Writing
///----------------------------------------------------------------------------


nlohmann::ordered_json BaseEntityBehavior::Write() const
{
    nlohmann::ordered_json data;
    data["Health"] = m_Health.Write();
    return data;
}

