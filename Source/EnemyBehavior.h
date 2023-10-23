///*****************************************************************/
/// @file	 EnemyBehavior.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 10/1/2023
/// @brief   EnemyBehavior class header
/// @details This class is used to create a base Enemy Behavior
///*****************************************************************/


#pragma once
#include "behavior.h"
#include "Pool.h"

class Pathfinder;

class EnemyBehavior :
	public Behavior 
{
//-----------------------------------------------------------------------------
public: // constructor / destructor / inspector
//-----------------------------------------------------------------------------
	EnemyBehavior();

///-----------------------------------------------------------------------------
public: //Methods
///-----------------------------------------------------------------------------

    Pool<int>* GetHealth() { return &m_Health; }

    void TakeDamage(int damage);

//-----------------------------------------------------------------------------
private: // Member Variables
//----------------------------------------------------------------------------
    ///@   The Health of the Enemy
    Pool<int> m_Health; 
//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    virtual void OnInit() override;

    virtual void OnExit() override;

    /// @brief  inspector for this component
    virtual void Inspector() override;

    /// @brief  map of read methods
    static ReadMethodMap< EnemyBehavior > const s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    virtual void OnFixedUpdate() override;

public:

    /// @brief Write all Transform component data to a JSON file.
    /// @return The JSON file containing the Transform component data.
    virtual nlohmann::ordered_json Write() const override;

///-----------------------------------------------------------------------------
private: // Read Methods
///-----------------------------------------------------------------------------

    void readHealth(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
private: // 
//-----------------------------------------------------------------------------
    void ChaseTarget(Pathfinder* pathfinder, float dt);
    
 //-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

        /// @brief  creates a new copy of this Component
        /// @return the newly created component
    virtual Component* Clone() const override;

    /// @brief  copy constructor
    /// @param  other   the other Transform to copy
    EnemyBehavior(EnemyBehavior const& other);

    //-----------------------------------------------------------------------------
};
