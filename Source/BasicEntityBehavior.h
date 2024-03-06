///*****************************************************************/
/// @file	    BasicEntityBehavior.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    11/10/2023
/// @brief      BasicEntityBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/


#include "Behavior.h"
#include "Pool.h"

#pragma once

class BasicEntityBehavior :
    public Behavior
{

protected:

    BasicEntityBehavior(std::type_index m_Type) :
        Behavior(m_Type), m_Health("Health")
    {}

///-----------------------------------------------------------------------------
public: // accessors
///-----------------------------------------------------------------------------

    /// @brief Returns the health of the enemy
    /// @return - the health of the enemy
    Pool<int>* GetHealth() { return &m_Health; }

//-----------------------------------------------------------------------------
protected: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief initializes the component
    virtual void OnInit() override;

    /// @brief Called when the component is destroyed
    virtual void OnExit() override;

    /// @brief  inspector for this component
    virtual void Inspector() override;

///-----------------------------------------------------------------------------
protected: // Reading
///-----------------------------------------------------------------------------

    /// @brief  reads the health of the enemy from json
    /// @param  data    the json data to read from
    void readHealth(nlohmann::ordered_json const& data);

///-----------------------------------------------------------------------------
public: // methods
///-----------------------------------------------------------------------------
    
    /// @brief Apply damage to the Entity 
    /// @param damage - the amount of damage to Entity
    void TakeDamage( int damage );


///-----------------------------------------------------------------------------
protected: // Member Variables
///-----------------------------------------------------------------------------

    /// @brief  the Health of the Enemy
    Pool<int> m_Health;

///-------------------------------------------------------------------------------------------
public: // copying
///-------------------------------------------------------------------------------------------

    /// @brief  creates a new clone of this Component
    /// @return the newly created clone
    virtual Component* Clone() const override
    {
        return new BasicEntityBehavior( *this );
    }

///-------------------------------------------------------------------------------------------
protected: // copying
///-------------------------------------------------------------------------------------------
    
    /// @brief  copy constructor
    /// @param  other   the BasicEntityBehavior to copy
    BasicEntityBehavior(BasicEntityBehavior const& other);

};

