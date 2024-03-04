/// @file     PathfinderTarget.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    
#pragma once
#include "ComponentReference.h"
#include "Transform.h"

/// @brief      
class PathfinderTarget : public Component
{
public:
    /// @brief   Default constructor
    PathfinderTarget();

    /// @brief   Copy constructor
    PathfinderTarget(PathfinderTarget const& other);

    /// @brief   Destructor : removes itself from system, if not removed already
    ~PathfinderTarget();

    // Inherited via Component
    virtual Component * Clone() const override;



//-----------------------------------------------------------------------------
//              Accessors
//-----------------------------------------------------------------------------
public:

    __inline Transform* GetParentTransform() const { return m_ParentTransform; }

    __inline unsigned int GetPriority() const { return static_cast<int>(m_Priority); }


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

    enum Priority { Highest, High, Mid, Low };

    ComponentReference<Transform> m_ParentTransform;

    Priority m_Priority = Highest;


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

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
};
