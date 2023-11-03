///*****************************************************************/
/// @file	    EnemyBehavior.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @date	    10/1/2023
/// @brief      EnemyBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/


#pragma once
#include "BasicEntityBehavior.h"
#include "Pool.h"

class Pathfinder;
class Transform;
class RigidBody;
class Animation;
class AnimationAsset;

class EnemyBehavior : public BasicEntityBehavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

	EnemyBehavior();

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief initializes the component
    virtual void OnInit() override;

    /// @brief Called at a fixed interval
    virtual void OnFixedUpdate() override;

//-----------------------------------------------------------------------------
private: // Member Variables
//-----------------------------------------------------------------------------
    /// @brief  how fast this Enemy moves
    float m_Speed = 10.0f;

    int m_Damage = 1;

    /// @brief  the name of the Entity with the Pathfinder to follow
    std::string m_PathfinderName = "";

    /// @brief  the Pathfinder this Enemy follows
    Pathfinder* m_Pathfinder = nullptr;

    /// @brief  the transform attached to this Enemy
    Transform* m_Transform = nullptr;

    /// @brief  the RigidBody attached to this Enemy
    RigidBody* m_RigidBody = nullptr;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    ///  @brief moves towards the target
    void ChaseTarget();

///-----------------------------------------------------------------------------
private: // Reading
///-----------------------------------------------------------------------------
    /// @brief  reads the name of the pathfinder entity
    /// @param  data    the json data to read from
    void readPathfinderName( nlohmann::ordered_json const& data );

    /// @brief  reads the speed
    /// @param  data    the json data to read from
    void readSpeed( nlohmann::ordered_json const& data );

    /// @brief  reads the damage
    /// @param  data    the json data to read from
    void readDamage(nlohmann::ordered_json const& data);

    /// @brief  map of read methods
    static ReadMethodMap< EnemyBehavior > const s_ReadMethods;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    int GetDamage();

///-----------------------------------------------------------------------------
public: // reading / writing
///-----------------------------------------------------------------------------

    /// @brief  write all component data to a JSON object
    nlohmann::ordered_json Write() const;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    /// @brief  creates a new copy of this Component
    /// @return the newly created component
    virtual EnemyBehavior* Clone() const override
    {
        return new EnemyBehavior( *this );
    }

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  copy constructor
    /// @param  other   the other Transform to copy
    EnemyBehavior( EnemyBehavior const& other );

//-----------------------------------------------------------------------------
};
