/*********************************************************************
* \file   JetpackBoost.h
* \brief  s
*
* \author Eli Tsereteli
*********************************************************************/
#pragma once
#include "Behavior.h"
#include "ComponentReference.h"
#include "ActionReference.h"
class Emitter;
class Transform;
class RigidBody;

class JetpackBoost : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructors / destructors
//-----------------------------------------------------------------------------

    /// @brief  Default constructor
    JetpackBoost();

    /// @brief  Returns copy of this behavior
    JetpackBoost* Clone() const override;
    
private:
    /// @brief  copy ctor
    JetpackBoost(const JetpackBoost& other);



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:
    /// @brief  Adds itself to behavior system
    virtual void OnInit() override;

    /// @brief  
    virtual void OnUpdate(float dt) override;

    /// @brief  Removes itself from behavior system
    virtual void OnExit() override;
    
    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;

    /// @brief  What to do when entity is re-parented
    virtual void OnHierarchyChange(Entity* previousParent);


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    
    /// @brief  This entity's transform
    ComponentReference< Transform > m_Transform;

    /// @brief  Player's transsform
    ComponentReference< Transform > m_PTransform;

    /// @brief  Player's rigidbody
    ComponentReference< RigidBody > m_RBody;

    /// @brief  Flame emitter
    ComponentReference< Emitter > m_Flame;
    //ComponentReference< Emitter > m_Smoke;

    /// @brief  Keep track of player angle to change it gradually
    float m_Angle = 0.0f;

    /// @brief  up/down input
    ActionReference m_InputYAxis;

    /// @brief  left/right input
    ActionReference m_InputXAxis;


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:
    /// @brief   the map of read methods for this Component
    static ReadMethodMap< JetpackBoost > const s_ReadMethods;

    /// @brief       Reads horizontal input axis
    /// @param data  json to read from
    void readXAxisInput(nlohmann::ordered_json const& data);

    /// @brief       Reads vertical input axis
    /// @param data  json to read from
    void readYAxisInput(nlohmann::ordered_json const& data);
    

public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all JetpackBoost component data to a JSON file.
    /// @return     The JSON file containing the JetpackBoost component data.
    virtual nlohmann::ordered_json Write() const override;
};

