///--------------------------------------------------------------------------//
/// @file   EntityHealth.h
/// @brief  Entity Health header file
/// 
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @date   14 January 2024
///
/// @copyright © 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#pragma once

///--------------------------------------------------------------------------//
/// Includes
///--------------------------------------------------------------------------//
#include "Component.h"
#include "Pool.h"
#include <functional>
#include <map>

/// @brief Entity health component.
class Health : public Component
{
//-----------------------------------------------------------------------------
public: // constructors / destructor
//-----------------------------------------------------------------------------
    
    /// @brief Constructor for the Health component.
    Health();

    /// @brief Destructor for the Health component.
    ~Health() = default;


private: // copying


    /// @brief Copy Constructor
    /// @param other - Reference to an Health component to copy.
    Health(Health const& other);


protected: // constructor 


    /// @brief Inherited constructor.
    /// @param type - the type of derived class.
    Health(std::type_index const& type);

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------
    
    /// @brief  Creates a copy of this Health component.
    /// @return A copy of this component (Component*).
    virtual Component* Clone() const override;

//-----------------------------------------------------------------------------
public: // Callback
//-----------------------------------------------------------------------------

    /// @brief Adds an OnHealthChanged callback to this Health component.
    /// @param ownerID  - The ID of the owner of the callback.
    /// @param callback - The callback function to add.
    void AddOnHealthChangedCallback(unsigned ownerID, std::function<void()> callback);

    /// @brief Removes an OnHealthChanged callback from this Health component.
    /// @param ownerID - Handle to the callback to remove.
    void RemoveOnHealthChangedCallback(unsigned ownerID);


//-----------------------------------------------------------------------------
public: // Accessors
//-----------------------------------------------------------------------------

    /// @brief Set the health of the attached entity.
    /// @param amount - The new health value.
    void SetHealth(int amount);

    /// @brief  Get the health of the attached entity.
    /// @return The health of the entity (Pool<int>*).
    Pool<int>* GetHealth();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief Apply damage to the entity.
    /// @param damage - the amount of damage to apply.
    void TakeDamage(int damage);


    /// @brief  resets the health to full
    void Reset();


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------
    
    /// @brief Inspector for this component.
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief The ehalth of the entity.
    Pool<int> m_health;

    /// @brief  callbacks to be called whenever this Health component changes.
    std::map< unsigned, std::function< void() > > m_OnHealthChangedCallbacks = {};

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief Reads the health of the entity from a JSON.
    /// @param data - the JSON to read from.
    void readHealth(nlohmann::ordered_json const& data);

    /// @brief Any computation that needs to occur after loading.
    virtual void AfterLoad() override;

    /// @brief  map of read methods
    static ReadMethodMap< Health > s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

public:

    /// @brief  Write all Health component data to a JSON file.
    /// @return The JSON file containing the Health component data.
    virtual nlohmann::ordered_json Write() const override;
};