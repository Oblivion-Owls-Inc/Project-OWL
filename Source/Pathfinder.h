/*****************************************************************//**
 * \file   Pathfinder.h
 * \brief  Interface file of Pathfinder component.
 * 
 * \author Eli Tsereteli
 *********************************************************************/
#pragma once
#include "Tilemap.h"
#include <vector>

class Pathfinder : public Component
{
public:
    /// @brief   Default constructor
    Pathfinder();

    /// @brief   Returns a copy of this component
    virtual Component * Clone() const override;



//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------
public:

    /// @brief       Sets new destination based on world position
    /// @param pos   World position
    void SetDestination(glm::vec2 pos);

    /// @brief       Gets normalized direction vector at given world position.
    ///              The vector points to next nearest spot on the path towards
    ///              the destination.
    /// @param pos   Position of interest
    /// @return      Direction vector towards destination. If out of bounds, returns <0,0>
    glm::vec2 GetDirectionAt(glm::vec2 pos) const;


    /// @brief       Gets position of current destination
    /// @return      World position
    __inline glm::vec2 GetDestination() const { return m_DestPos; }


    /// @brief       Checks if the given world position is walkable.
    /// @param pos   World position
    /// @return      Walkable or not
    bool IsWalkable(glm::vec2 pos) const;


    /// @brief            Sets which tile IDs should be considered walkable.
    /// @param walkables  Vector of tiles
    __inline void SetWalkables(std::vector<int> const& walkables) { m_Walkables = walkables; }



//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  called when entering a scene - syncs with Tilemap
    virtual void OnInit() override;

    /// @brief  called when exiting a scene - un-syncs (removes callback)
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    
    /// @brief  Enum of node types, used by the algo.
    enum Type { Unwalkable, Undiscovered, Seen, Explored };

    /// @brief  Struct used by the algo. Each node corresponds to a tile.
    struct Node
    {
        Type type;              /// @brief  Node type (enum)
        int cost;               /// @brief  How far of a walk from destination
        glm::ivec2 direction;   /// @brief  Unit vector pointing toward destination
    };

    std::vector<Node> m_Nodes;          /// @brief  "grid" of nodes to navigate
    std::vector<int> m_Walkables;       /// @brief  Tile IDs of "not walls"
    glm::vec2 m_DestPos = {0,0};        /// @brief  Destination (actual position)
    glm::ivec2 m_DestTile = {1,1};      /// @brief  Destination tile coord
    Tilemap<int>* m_Tilemap = nullptr;  /// @brief  Cached parent tilemap




//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:
    /// @brief      Actual pathfinding. Updates all nodes.
    void explore();



//-----------------------------------------------------------------------------
//              Reading
//-----------------------------------------------------------------------------
private:

    /// @brief          Read in the destination position
    /// @param stream   The json to read from.
    void readDestination( nlohmann::ordered_json const& data );

    /// @brief          Read in the array of walkable tiles
    /// @param stream   The json to read from.
    void readWalkables( nlohmann::ordered_json const& data );


    /// @brief the map of read methods for this Component
    static ReadMethodMap< Pathfinder > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }



//-----------------------------------------------------------------------------
//              Writing
//-----------------------------------------------------------------------------
#if 0
public:

    /// @brief Write all Sprite component data to a JSON file.
    /// @return The JSON file containing the Sprite component data.
    virtual nlohmann::ordered_json Write() const override;
#endif
};



