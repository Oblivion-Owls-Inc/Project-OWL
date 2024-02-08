///*****************************************************************/
/// @file	    GeneratorBehavior.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      GeneratorBehavior class header
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#pragma once
#include "BasicEntityBehavior.h"
#include "Behavior.h"
#include "Pool.h"


class AudioPlayer;

class GeneratorBehavior : public Behavior
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
    virtual GeneratorBehavior* Clone() const override;


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief called when Generator initializes
    virtual void OnInit() override;

    /// @brief called when Generator exits
    virtual void OnExit() override;

    
    
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
    float GetPowerRadius() { return m_powerRadius; }

    /// @brief returns the radius a player can activate a generator within
    /// @return current radius for activating a generator
    float GetActivationRadius() { return m_activationRadius; }

    /// @brief returns if the generator is powered or not
    /// @return is the generator active
    bool GetActive() { return m_isActive;  }

    /// @brief activate the generator
    void Activate() { m_isActive = true; }

//-----------------------------------------------------------------------------
private: // variables
//-----------------------------------------------------------------------------

    /// @brief  audio
    AudioPlayer* m_AudioPlayer = nullptr;

    /// @brief  is the generator active or not
    bool m_isActive;        

    /// @brief  radius the generator power turrets within
    float m_powerRadius;         

    /// @brief  radius a player can activate the generator within
    float m_activationRadius;

    /// @brief  depth value of the generator, used for determening lowest
    int m_depth;            

//-----------------------------------------------------------------------------
private: // private functions
//-----------------------------------------------------------------------------

    /// @brief handles a collision with the generator
        /// @param other collider of colliding entity
        /// @param collision data for the collision
    void onCollisionEnter(Collider* other);

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

