///--------------------------------------------------------------------------//
/// @file   EntityHealth.h
/// @brief  Entity Health cpp file
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   14 January 2024
///
/// @copyright © 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#include "pch.h" // precompiled header has to be included first
#include "Health.h"

///--------------------------------------------------------------------------//
/// @brief Constructor for entity health
///--------------------------------------------------------------------------//
Health::Health()
    : Component(typeid(Health))
{}

/// @brief Copy Constructor
/// @param other - Reference to an EntityHealth component to copy.
Health::Health(Health const& other)
    : Component(other)
    , m_health(other.m_health)
    , m_OnHealthChangedCallbacks()
{}

///--------------------------------------------------------------------------//
/// @brief Inherited Constructor
/// @param type - type of derived class
///--------------------------------------------------------------------------//
Health::Health(std::type_index const& type)
    : Component(type)
{}

///--------------------------------------------------------------------------//
/// @brief  Creates a new clone of this component.
/// @return The newly created clone (Component*).
///--------------------------------------------------------------------------//
Component* Health::Clone() const { return new Health(*this); }

///--------------------------------------------------------------------------//
/// @brief Adds an OnEntityHealthChanged callback to this EntityHealth.
/// @param ownerID  - The ID of the owner of the callback.
/// @param callback - The callback function to add.
///--------------------------------------------------------------------------//
void Health::AddOnHealthChangedCallback(unsigned ownerID, std::function<void()> callback)
{
    m_OnHealthChangedCallbacks.emplace(ownerID, std::move(callback));
}

///--------------------------------------------------------------------------//
/// @brief Removes an OnEntityHealthChanged callback from this EntityHealth.
/// @param ownerID - Handle to the callback to remove.
///--------------------------------------------------------------------------//
void Health::RemoveOnHealthChangedCallback(unsigned ownerID)
{
    m_OnHealthChangedCallbacks.erase(ownerID);
}

///--------------------------------------------------------------------------//
/// @brief Set the health of the attached entity.
/// @param amount - The new health value.
///--------------------------------------------------------------------------//
void Health::SetHealth(int amount) 
{ 
    m_health.SetCurrent(amount);

    for ( auto& [ id, callback ] : m_OnHealthChangedCallbacks )
    {
        callback();
    }
}

///--------------------------------------------------------------------------//
/// @brief  Get the health of the attached entity.
/// @return The health of the entity (Pool<int>*).
///--------------------------------------------------------------------------//
Pool<int>* Health::GetHealth() { return &m_health; }

///--------------------------------------------------------------------------//
/// @brief Apply damage to the entity.
/// @param damage - the amount of damage to apply.
///--------------------------------------------------------------------------//
void Health::TakeDamage(int damage)
{ 
   m_health -= damage; 

   for ( auto& callback : m_OnHealthChangedCallbacks )
   {
       callback.second();
   }
}


/// @brief  resets the health to full
void Health::Reset()
{
    m_health.Reset();

    for ( auto& callback : m_OnHealthChangedCallbacks )
    {
        callback.second();
    }
}


///--------------------------------------------------------------------------//
/// @brief Inspector for this component.
///--------------------------------------------------------------------------//
void Health::Inspector() 
{ 
    if(m_health.Inspect())
    {
        for (auto callback : m_OnHealthChangedCallbacks)
        {
            callback.second();
        }
    }
}

///--------------------------------------------------------------------------//
/// @brief Reads the health of the entity from a JSON.
/// @param data - the JSON to read from.
///--------------------------------------------------------------------------//
void Health::readHealth(nlohmann::ordered_json const& data)
{
    Stream::Read(m_health, data);
}

///--------------------------------------------------------------------------//
/// @brief Any computation that needs to occur after loading.
///--------------------------------------------------------------------------//
void Health::AfterLoad() {}

///--------------------------------------------------------------------------//
/// @brief  Write all Health component data to a JSON file.
/// @return The JSON file containing the Health component data.
///--------------------------------------------------------------------------//
nlohmann::ordered_json Health::Write() const
{
    nlohmann::ordered_json data;

    data["Health"] = m_health.Write();

    return data;
}

/// @brief Map of all the read methods for this component.
ReadMethodMap<Health> Health::s_ReadMethods = {
    { "Health" , &readHealth }
};
