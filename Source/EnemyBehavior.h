///*****************************************************************/
/// @file	    EnemyBehavior.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    10/1/2023
/// @brief      EnemyBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#pragma once

#include "Behavior.h"
#include "Pool.h"

#include "ComponentReference.h"
#include "AssetReference.h"
#include "Transform.h"
#include "RigidBody.h"
#include "AudioPlayer.h"
#include "Health.h"
#include "Tilemap.h"

class EnemyBehavior : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------


	/// @brief  default constructor
	EnemyBehavior();

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the amount of damage that this EnemyBehavior deals
    /// @return the amount of damage that this EnemyBehavior deals
    int GetDamage() const;


    /// @brief  gets this EnemyBehavior's attached Transform Component
    /// @return this EnemyBehavior's attached Transform Component
    Transform const* GetTransform() const;


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  initializes the component
    virtual void OnInit() override;

    /// @brief  called when this Component's Entity is removed from the Scene
    virtual void OnExit() override;


    /// @brief  Called at a fixed interval
    virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: // Member Variables
//-----------------------------------------------------------------------------


    /// @brief  how fast this Enemy moves
    float m_Speed = 10.0f;

    /// @brief how much damage the enemy does.
    int m_Damage = 1;


    /// @brief the cached reward that will be dropped 
    Entity* m_RewardEntity = nullptr;

    /// @brief  the transform attached to this Enemy
    ComponentReference< Transform > m_Transform;

    /// @brief  the RigidBody attached to this Enemy
    ComponentReference< RigidBody > m_RigidBody;

    /// @brief  the AudioPlayer attached to this Enemy
    ComponentReference< AudioPlayer > m_AudioPlayer;

    /// @brief  Health of this entity.
    ComponentReference< Health > m_Health;

    /// @brief  the entity Prefab that will be dropped when this enemy dies.
    AssetReference< Entity > m_Reward;
//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    ///  @brief moves towards the target
    void chaseTarget();

    /// @brief What to do when the enemy gets damaged.
    void onDamageTaken();

    /// @brief The Event that is called when the enemy dies.
    void onDeath();
    
///-----------------------------------------------------------------------------
public: // inspection
///-----------------------------------------------------------------------------


    /// @brief  inspector for this component
    virtual void Inspector() override;


///-----------------------------------------------------------------------------
private: // Reading
///-----------------------------------------------------------------------------

    /// @brief  reads the speed
    /// @param  data    the json data to read from
    void readSpeed( nlohmann::ordered_json const& data );

    /// @brief  reads the damage
    /// @param  data    the json data to read from
    void readDamage(nlohmann::ordered_json const& data);

    /// @brief  reads the reward
    void readReward(nlohmann::ordered_json const& data);


///-----------------------------------------------------------------------------
public: // reading / writing
///-----------------------------------------------------------------------------

    
    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  write all component data to a JSON object
    nlohmann::ordered_json Write() const;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new copy of this Component
    /// @return the newly created component
    virtual EnemyBehavior* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the other Transform to copy
    EnemyBehavior( EnemyBehavior const& other );


    // disable assignment operator 
    void operator =( EnemyBehavior const& ) = delete;


//-----------------------------------------------------------------------------
};
