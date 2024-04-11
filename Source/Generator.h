///*****************************************************************/
/// @file	    Generator.h
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      Generator class header
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#pragma once

#include "Component.h"

#include "ComponentReference.h"
#include "AudioPlayer.h"
#include "Transform.h"
#include "Collider.h"
#include "Health.h"
#include "Sprite.h"
#include "Emitter.h"
#include "EmitterSprite.h"
#include "PathfinderTarget.h"
#include "Light.h"
#include "Interactable.h"


class Generator : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------


    /// @brief  constructor
    Generator();

    /// @brief Generator clone
    /// @return Component pointer of the cloned generator behavior
    virtual Generator* Clone() const override;


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief called when Generator initializes
    virtual void OnInit() override;

    /// @brief called when Generator exits
    virtual void OnExit() override;

    /// @brief  called every frame
    /// @param  dt delta time
    virtual void OnUpdate(float dt) override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------
    
    /// @brief copy ctor
    Generator(const Generator& other);

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets the lowest generator
    /// @return returns the lowest generator
    static Generator* GetLowestGenerator();

    /// @brief returns the radius within turrets are powered
    /// @return the power radius of the generator
    float GetPowerRadius() { return m_PowerRadius; }

    /// @brief returns the radius a player can activate a generator within
    /// @return current radius for activating a generator
    float GetActivationRadius() { return m_ActivationRadius; }

    /// @brief returns if the generator is powered or not
    /// @return is the generator active
    bool GetActive() { return m_IsActive;  }

    /// @brief activate the generator
    void Activate();

    /// @brief deactivate the generator
    void Deactivate();

    int GetCost() { return m_Cost; } const

    /// @brief  get the transform of the generator
    /// @return the generator transform
    Transform* GetTransform() { return m_Transform; }

//-----------------------------------------------------------------------------
private: // variables
//-----------------------------------------------------------------------------

    /// @brief  is the generator active or not
    bool m_IsActive = false;  

    /// @brief  is the generator active or not
    bool m_ChangeActive = false;

    /// @brief  is the generator active or not
    bool m_ActivateRing = false;

    /// @brief  is the generator active or not
    bool m_DeactivateRing = false;

    /// @brief  shrink ring to match edited value if true
    bool m_ShrinkRing = false;

    /// @brief  can activating the generator spawn a wave
    bool m_CanSpawnWave = true;

    /// @brief can the generator be activated
    bool m_CanActivate = true;

    /// @brief  speed the particle ring grows and shrinks at
    float m_RadiusSpeed = 1.0f;

    /// @brief  radius the generator power turrets within
    float m_PowerRadius = 1.0f;         

    /// @brief  radius the generator emits particles to
    float m_GrowthRadius = 1.0f;

    /// @brief  radius a player can activate the generator within
    float m_ActivationRadius = 1.0f;

    /// @brief Cost of the generator
    int m_Cost = 0;

    /// @brief  depth value of the generator, used for determening lowest
    int m_Depth = 0;

    /// @brief  the transform of the generator
    ComponentReference< Transform > m_Transform;

    /// @brief  audioPlayer for when the Generator takes damage
    ComponentReference< AudioPlayer > m_AudioPlayer;

    /// @brief  the Collider component attached to this Generator
    ComponentReference< Collider > m_Collider;

    /// @brief  the Health component attached to this Generator
    ComponentReference< Health > m_Health;

    /// @brief  the Sprite component attached to this Generator
    ComponentReference< Sprite > m_Sprite;

    /// @brief  the PathfinderTarget Component attached to this Generator
    ComponentReference< PathfinderTarget > m_PathfinderTarget;

    /// @brief  the Emitter component attached to this Generator
    ComponentReference< Emitter > m_Emitter;

    /// @brief  the Interactable Component attached to this Generator
    ComponentReference< Interactable, false > m_Interactable;

    /// @brief  the Light Component attached to this Generator
    ComponentReference< Light > m_Light;

    /// @brief  the wave prefab to spawn on generator activation
    AssetReference< Entity > m_WavePrefab;


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
    static ReadMethodMap< Generator > const s_ReadMethods;

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

    /// @brief	read the speed the particle ring changes
    void readSpeed(nlohmann::ordered_json const& json);

    /// @brief	read the attached wave prefab to spawn
    void readWavePrefab(nlohmann::ordered_json const& json);

    /// @brief Read the cost of the generator
    /// @param json - json object to read from
    void readCost(nlohmann::ordered_json const& json);
//-----------------------------------------------------------------------------
public: // writing
//-----------------------------------------------------------------------------
    
    /// @brief Write all Generator data to a JSON file.
    /// @return The JSON file containing the WavesBehavior data.
    virtual nlohmann::ordered_json Write() const override;
};

