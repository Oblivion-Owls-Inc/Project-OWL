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
    Component* Clone() const override;

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
private:
//-----------------------------------------------------------------------------

    AudioPlayer* m_AudioPlayer = nullptr;

    bool m_isActive;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    void onCollision(Collider* other, CollisionData const& collisionData);

    /// @brief the map of read methods for this Component
    static ReadMethodMap< GeneratorBehavior > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------

    /// @brief Write all GeneratorBehavior data to a JSON file.
    /// @return The JSON file containing the WavesBehavior data.
    virtual nlohmann::ordered_json Write() const override;
};

