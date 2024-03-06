/// @file       PathfinderSource.h
/// @author     Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief      Defines parent entity as the map for pathfinding.
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology
#pragma once
#include "Component.h"


/// @brief       Pathfinder source.
class PathfinderSource : public Component
{
public:
    /// @brief   Default constructor
    PathfinderSource();

    // Inherited via Component
    virtual PathfinderSource * Clone() const override;



//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:

    /// @brief   Sets this component's parent entity as the active tilemap for
    ///          pathfinding.
    void SetActive() const;



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Initialization: calls SetActive()
    virtual void OnInit() override;

    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief   List of tile IDs that are "not walls"
    std::vector<int> m_Walkables;



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

    /// @brief   For inspector. View/modify the array of walkable tile IDs
    void inspectWalkables();



//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief       reads the list of walkable tile ID's
    /// @param data  json data
    void readWalkables( nlohmann::ordered_json const& data );

    /// @brief   the map of read methods for this Component
    static ReadMethodMap< PathfinderSource > const s_ReadMethods;


public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all Pathfinder component data to a JSON file.
    /// @return     The JSON file containing the Pathfinder component data.
    virtual nlohmann::ordered_json Write() const override;



//-----------------------------------------------------------------------------
//              Copying
//-----------------------------------------------------------------------------
private:

    /// @brief   Copy constructor
    PathfinderSource(PathfinderSource const& other);

    /// @brief   Disable assignment 
    void operator =(PathfinderSource const&) = delete;

    
//-----------------------------------------------------------------------------
};
