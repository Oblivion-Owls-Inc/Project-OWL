/*********************************************************************
* \file   CameraBehavior.h
* \brief  Camera that smoothly follows specified entity.
* 
* \author Eli Tsereteli
*********************************************************************/
#pragma once
#include "Behavior.h"

#include "ComponentReference.h"
#include "Camera.h"
#include "Transform.h"

class CameraBehavior : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  Default constructor
    CameraBehavior();

    /// @brief  Returns copy of this behavior
    CameraBehavior* Clone() const override;
    
private:
    /// @brief  copy ctor
    CameraBehavior(const CameraBehavior& other);


//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Adds itself to behavior system
    virtual void OnInit() override;

    /// @brief  Performs the smooth following
    virtual void OnUpdate(float dt) override;

    /// @brief  Removes itself from behavior system
    virtual void OnExit() override;
    
    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief   Transform of the entity to follow
    ComponentReference< Transform > m_ParentTransform;

    /// @brief   Parent transform
    ComponentReference< Transform > m_Transform;

    /// @brief   Camera component of parent
    ComponentReference< Camera > m_Cam;

    /// @brief   Map bounds. Stops camera from passing them (as long as its
    ///          dimensions don't exceed it). 
    ///          If camera dimensions exceed bounds, it will always be centered.
    ///          If bound[0] == bound[1], it's unbounded.
    float m_xBounds[2] = {}, m_yBounds[2] = {};

    /// @brief   Low number - follows target slowly, does not bother centering;
    ///          High number - snaps firmly to target
    float m_Factor = 1.0f;


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:
    /// @brief		 Helper: clamps or centers coordinate between given bounds, 
    ///              depending on distance
    /// @param val	 Value to clamp/center
    /// @param lo	 Lower bound
    /// @param hi	 Upper bound
    /// @param dist  Distance to fit within bounds
    void clampOrCenter(float &val, float lo, float hi, float range);


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:
    /// @brief   the map of read methods for this Component
    static ReadMethodMap< CameraBehavior > const s_ReadMethods;
    
    /// @brief		 Reads the horizontal bounds
    /// @param data  json data to read
    void readXBounds(nlohmann::ordered_json const& data);

    /// @brief		 Reads the vertical bounds
    /// @param data  json data to read
    void readYBounds(nlohmann::ordered_json const& data);


public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all CameraBehavior component data to a JSON file.
    /// @return     The JSON file containing the CameraBehavior component data.
    virtual nlohmann::ordered_json Write() const override;
};

