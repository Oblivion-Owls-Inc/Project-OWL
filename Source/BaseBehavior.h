///*****************************************************************/
/// @file	    BaseBehavior.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      BaseBehavior class header
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#pragma once
#include "BasicEntityBehavior.h"
#include "Pool.h"

class BaseBehavior :
    public BasicEntityBehavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    BaseBehavior();
    /// @brief copy ctor
    BaseBehavior(const BaseBehavior& other);
    /// @brief dtor
    ~BaseBehavior();
    /// @brief Base clone
    Component* Clone() const override;

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    
    virtual void OnInit() override;

    virtual void OnExit() override;
    
private:
    void onCollision(Collider* other, CollisionData const& collisionData);

    /// @brief the map of read methods for this Component
    static ReadMethodMap< BaseBehavior > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

public:

    /// @brief Write all BaseBehavior data to a JSON file.
    /// @return The JSON file containing the WavesBehavior data.
    virtual nlohmann::ordered_json Write() const override;
};

