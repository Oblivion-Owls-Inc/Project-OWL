/*********************************************************************
* \file   CameraBehavior.h
* \brief  Camera that smoothly follows specified entity.
*
* \author Eli Tsereteli
*********************************************************************/
#pragma once
#include "Behavior.h"

class CameraBehavior : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief  Default constructor
    CameraBehavior();

    /// @brief  Returns copy of this behavior
    Component* Clone() const override;
    
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

    /// @brief   Name of the entity to follow
    std::string m_TargetEntityName = {};

    /// @brief   Map bounds. Stops camera from passing them (as long as its
    ///          dimensions don't exceed it). 
    ///          If camera dimensions exceed bounds, it will always be centered.
    ///          If bound[0] == bound[1], it's unbounded.
    float m_xBounds[2] = {}, m_yBounds[2] = {};


    void clampOrCenter(float &val, float lo, float hi, float range);


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:
    /// @brief   the map of read methods for this Component
    static ReadMethodMap< CameraBehavior > const s_ReadMethods;

    void readTargetEntityName(nlohmann::ordered_json const& data);
    
    void readXBounds(nlohmann::ordered_json const& data);

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

