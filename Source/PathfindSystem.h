/// @file       PathfindSystem.h
/// @author     Eli Tsereteli (ilya.tserete;o@digipen.edu)
/// @brief      System in charge of pathfinding. Keeps track of targets,
///             runs the flowfield algorithm.
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology
#pragma once

#include "pch.h" // precompiled header has to be included first
#include "ComponentSystem.h"
#include "ComponentReference.h"
#include "PathfinderTarget.h"

#include "Tilemap.h"


/// @brief  
class PathfindSystem : public ComponentSystem<PathfinderTarget>
{
//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:

    /// @brief       Gets normalized direction vector at given world position.
    ///              The vector points to next nearest spot on the path towards
    ///              active target(s).
    /// @param pos   Position of interest
    /// @return      Direction vector towards target(s). If out of bounds, returns <0,0>
    glm::vec2 GetDirectionAt(glm::vec2 pos) const;

    /// @brief       Gets the travel distance (in tiles) to the destination
    /// @param pos   Position from which to travel
    /// @return      Amount of tiles to travel til destination. If out of bounds, 
    ///              returns -1.
    int GetTravelDistanceAt(glm::vec2 pos) const;

    /// @brief       Checks if the given world position is walkable.
    /// @param pos   World position
    /// @return      Walkable or not
    bool IsWalkable(glm::vec2 pos) const;

    /// @brief            Sets which tile IDs should be considered walkable.
    /// @param walkables  Vector of tiles
    __inline void SetWalkables(std::vector<int> const& walkables) { m_Walkables = walkables; }

    /// @brief          Sets the tilemap (presumably, level map) which contains 
    ///                 the tilemap<int> to navigate
    /// @param entity   Level map entity (usage: PathfindMap should pass its parent here)
    void SetActiveTilemap(Entity* entity);

    /// @brief   Marks dirty - vector field needs to be updated.
    __inline void MarkDirty() { m_Dirty.store(true); }

    /// @brief    Makes sure MarkDirty() is called whenever given Transform changes.
    /// @param t  Transform component
    void AddTransformCallback(Transform* t);

    /// @brief    Remove the callback from transform
    /// @param t  Transform component
    void RemoveTransformCallback(Transform* t);


//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Called every frame: manages the background thread
    virtual void OnUpdate(float dt) override;

    /// @brief  Called when system exits
    virtual void OnExit() override;

    /// @brief  ImGui
    //virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief  Enum of node types, used by the algo.
    enum Type { Unwalkable, Undiscovered, Seen, Explored };

    /// @brief  Struct used by the algo. Each node corresponds to a tile.
    struct Node
    {
        Type type;              /// @brief  Node type (enum)  TODO: checking this for each node when iterating is cache-inefficient
        int cost;               /// @brief  How far of a walk from destination
        glm::ivec2 direction;   /// @brief  Unit vector pointing toward destination
        unsigned priority;      /// @brief  Higher number = higher priority
    };

    /// @brief   Cached tilemap reference (map to navigate)
    ComponentReference< Tilemap<int> > m_Tilemap;

    /// @brief   Array of nodes for navigation. Corresponds to tilemap.
    std::vector<Node> m_Nodes;

    /// @brief  Tile IDs of "not walls"
    std::vector<int> m_Walkables;

    /// @brief  Background thread for the actual algo
    std::thread m_Thread;

    /// @brief  When true, vector field is finished updating.
    std::atomic_bool m_Done = true;

    /// @brief  When true, vector field needs to be updated.
    std::atomic_bool m_Dirty = true;



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief      Actual pathfinding. Updates all nodes.
    void explore();



//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
public:

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this PathfindSystem to JSON
    /// @return the JSON data of this PathfindSystem
    virtual nlohmann::ordered_json Write() const override;



//-----------------------------------------------------------------------------
//              Singleton stuff
//-----------------------------------------------------------------------------
public:

    /// @brief  gets the instance of PathfindSystem
    /// @return the instance of the PathfindSystem
    static PathfindSystem* GetInstance();


private:

    /// @brief  Constructs the PathfindSystem
    PathfindSystem();

    // Prevent copying
    PathfindSystem( PathfindSystem const& ) = delete;
    void operator =( PathfindSystem const& ) = delete;

//-----------------------------------------------------------------------------
};

/// @brief      Convenient function for getting PathfindSystem instance.
__inline PathfindSystem* Pathfinder() { return PathfindSystem::GetInstance(); }

