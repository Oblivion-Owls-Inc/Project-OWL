/*****************************************************************//**
 * \file   Pathfinder.h
 * \brief  Interface file of Pathfinder component.
 * 
 * \author Eli Tsereteli
 *********************************************************************/
#pragma once

#include "ComponentReference.h"
#include "Tilemap.h"
#include <vector>
#include <thread>  // run the algo in the background
#include <atomic>  // thread sync

class Pathfinder : public Component
{
public:
    /// @brief   Default constructor
    Pathfinder();

    /// @brief   Copy constructor
    Pathfinder(Pathfinder const& other);

    /// @brief   Returns a copy of this component
    virtual Pathfinder * Clone() const override;

    /// @brief   Target prioritization enum. Declared here, because it's used by a
    ///          public function. It's a but weird... but it works.
    enum Priority { Highest, High, Mid, Low };


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
    __inline void SetWalkables( std::vector<int> const& walkables ) { m_Walkables = walkables; }


    /// @brief       Gets the travel distance (in tiles) to the destination
    /// @param pos   Position from which to travel
    /// @return      Amount of tiles to travel til destination. If out of bounds, 
    ///              returns -1.
    int GetTravelDistanceAt(glm::vec2 pos) const;


    /// @brief           Add target entity for pathfinding
    /// @param entity    Pointer to target entity
    /// @param priority  How important it is to enemies
    void AddTarget(Entity* entity, Priority priority);

    /// @brief           Remove target entity from the list
    /// @param entity    Pointer to target entity
    void RemoveTarget(Entity* entity);


//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  called when entering a scene - syncs with Tilemap
    virtual void OnInit() override;

    /// @brief  called when exiting a scene - un-syncs (removes callback)
    virtual void OnExit() override;

    /// @brief  displays this Pathfinder's inspector
    virtual void Inspector() override;


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
        unsigned priority;      /// @brief  Higher number = higher priority
    };

    /// @brief  Target transform + how important it is to enemies
    struct Target
    {
        // Constructors and operator= needed to pacify std::vector

        /// @brief         Parametric constructor.
        /// @param t       Target's Transform reference
        /// @param p       Target's priority enum
        Target(ComponentReference<Transform> t, Priority p);

        /// @brief         Move constructor (replacement for copy ctor)
        /// @param other   Target to copy/move
        Target(Target&& other) noexcept;

        /// @brief         Assignment operator. Does nothing. (not used, but compiler still wants it)
        Target& operator=(const Target& other) { return *this; }

        /// @brief  Reference to target's transform.
        ComponentReference<Transform> transform;

        /// @brief  Priority of the target. (enum)
        Priority priority;
    };

    /// @brief  Each target needs a priority.

    std::vector<Node> m_Nodes;          /// @brief  "grid" of nodes to navigate
    std::vector<int> m_Walkables;       /// @brief  Tile IDs of "not walls"
    glm::vec2 m_DestPos = {0,0};        /// @brief  Destination (actual position)
    glm::ivec2 m_DestTile = {0,0};      /// @brief  Destination tile

    /// @brief  Cached parent tilemap
    ComponentReference< Tilemap< int > > m_Tilemap;

    // leaving the singular destination too for now, for backwards compatibility.
    // it will pathfind to both the DestPos (if it's not 0,0), and the targets.


    /// @brief  Names of target entities (may or may not include priority as well)
    std::vector<std::string> m_TargetNames;
    std::vector<Target> m_Targets;           /// @brief  Targets to nagivate to
    std::thread m_Thread;                    /// @brief  Background thread for the actual algo
    std::atomic_bool m_Dirty = false,        /// @brief  Thread sync
                     m_Done = false;


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief  callback that gets called when the tilemap changes
    void onTilemapChangedCallback( Tilemap< int >*, glm::ivec2 const&, int const& );

    /// @brief      Actual pathfinding. Updates all nodes.
    void explore();

    /// @brief      For multithreading. Calls explore() in a separate thread if it's 
    ///             not running already, or tells it to run again after current iteration.
    void exploreQueue();

    /// @brief  inspector for choosing and displaying the destination of this Pathfinder
    void inspectDestination();

    /// @brief  inspector for choosing which tiles are walkable
    void inspectWalkables();

    /// @brief  Finds targets by their names, and stores their transforms for pathfinding
    void getTargets();


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief          Read in the destination position
    /// @param  stream  The json to read from.
    void readDestination( nlohmann::ordered_json const& data );

    /// @brief          Read in the array of walkable tiles
    /// @param  stream  The json to read from.
    void readWalkables( nlohmann::ordered_json const& data );

    /// @brief          Read in the target entities
    /// @param data     The json to read from.
    void readTargets( nlohmann::ordered_json const& data );


    /// @brief the map of read methods for this Component
    static ReadMethodMap< Pathfinder > const s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


    /// @brief Write all Sprite component data to a JSON file.
    /// @return The JSON file containing the Sprite component data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
