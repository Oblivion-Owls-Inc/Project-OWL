///*****************************************************************/
/// @file	    GeneratorBehavior.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      GeneratorBehavior class header
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#pragma once
#include "BasicEntityBehavior.h"
#include "Pool.h"


class AudioPlayer;

class GeneratorBehavior :
    public BasicEntityBehavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  constructor
    GeneratorBehavior();
    /// @brief dtor
    ~GeneratorBehavior();
    /// @brief Generator clone
    /// @return Component pointer of the cloned generator behavior
    Component* Clone() const override;


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief called when Generator initializes
    virtual void OnInit() override;

    /// @brief called when Generator exits
    virtual void OnExit() override;

    /// @brief handles a collision with the generator
    /// @param other collider of colliding entity
    /// @param collision data for the collision
    void onCollision(Collider* other, CollisionData const& collisionData);
    
//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------
    
    /// @brief copy ctor
    GeneratorBehavior(const GeneratorBehavior& other);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the lowest generator
    /// @return returns an entity pointer to the lowest generator
    static Entity* GetLowestGenerator();

    /// @brief returns the radius within turrets are powered
    /// @return the power radius of the generator
    float GetRadius() { return m_powerRadius; }

    /// @brief returns the radius a player can activate a generator within
    /// @return current radius for activating a generator
    float GetActivateRadius() { return m_activationRadius; }

    /// @brief activate the generator
    void Activate() { m_isActive = true; }

//-----------------------------------------------------------------------------
private: // variables
//-----------------------------------------------------------------------------

    AudioPlayer* m_AudioPlayer = nullptr;

    bool m_isActive;        // is the generator active
    float m_powerRadius;         // radius to power turrets
    float m_activationRadius; // radius a play can activate within
    int m_depth;            // depth value of the generator

//-----------------------------------------------------------------------------
private: // inspector methods
//-----------------------------------------------------------------------------

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief the map of read methods for this Component
    static ReadMethodMap< GeneratorBehavior > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief read the power radius
    void readRadius(nlohmann::ordered_json const& json);

    /// @brief read the power radius
    void readARadius(nlohmann::ordered_json const& json);

    /// @brief read the depth of the generator
    void readDepth(nlohmann::ordered_json const& json);

    /// @brief read if the generator starts on or off
    void readActive(nlohmann::ordered_json const& json);

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------
    
    /// @brief Write all GeneratorBehavior data to a JSON file.
    /// @return The JSON file containing the WavesBehavior data.
    virtual nlohmann::ordered_json Write() const override;
};

