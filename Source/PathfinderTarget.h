/// @file       PathfinderTarget.h
/// @author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief      Marks parent entity as a target for the enemies.
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology
#pragma once

#include "pch.h" // precompiled header has to be included first



/// @brief      
class PathfinderTarget : public Component
{
public:
    /// @brief   Default constructor
    PathfinderTarget();

    // Inherited via Component
    virtual PathfinderTarget * Clone() const override;



//-----------------------------------------------------------------------------
//              Accessors
//-----------------------------------------------------------------------------
public:

    /// @brief   Gets the pointer to parent entity's Transform
    /// @return  Pointer to Transform component
    __inline Transform* GetParentTransform() const { return m_ParentTransform; }

    /// @return  This target's priority
    __inline unsigned int GetPriority() const { return static_cast<int>(m_Priority); }

    /// @return  This target's active status
    __inline bool GetActive() const { return m_Active; }

    /// @brief   Sets this target's active status
    __inline void SetActive(bool enabled) { m_Active = enabled; }


//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Initialization: PathfinderTarget adds itself to the PathfinderTargeting system
    virtual void OnInit() override;

    /// @brief  Exit: PathfinderTarget removes itself from PathfinderTargeting system
    virtual void OnExit() override;

    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief  Priority enumerator.
    enum Priority { Highest, High, Mid, Low };

    /// @brief  Reference to transform of parent entity
    ComponentReference<Transform> m_ParentTransform;

    /// @brief  How important this target is to enemies
    Priority m_Priority = Highest;

    /// @brief  Should this target be considered
    bool m_Active = true;


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief          Read the priority for this target
    /// @param  stream  The json to read from.
    void readPriority(nlohmann::ordered_json const& data);


    /// @brief          Read the active status for this target
    /// @param  stream  The json to read from.
    void readActive(nlohmann::ordered_json const& data);


    /// @brief   the map of read methods for this Component
    static ReadMethodMap< PathfinderTarget > const s_ReadMethods;


public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all PathfinderTarget component data to a JSON file.
    /// @return     The JSON file containing the PathfinderTarget component data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
//              Copying
//-----------------------------------------------------------------------------
private:

    /// @brief   Copy constructor
    PathfinderTarget(PathfinderTarget const& other);

    /// @brief   Disable assignment 
    void operator =(PathfinderTarget const&) = delete;


//-----------------------------------------------------------------------------
};
