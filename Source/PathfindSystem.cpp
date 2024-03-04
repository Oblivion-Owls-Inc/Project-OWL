/// @file       PathfindSystem.cpp
/// @author     Eli Tsereteli (ilya.tserete;o@digipen.edu)
/// @brief      
/// @date       March 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Instutute of Technology

#include "PathfindSystem.h"


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------

/// @brief       Gets normalized direction vector at given world position.
///              The vector points to next nearest spot on the path towards
///              active target(s).
/// @param pos   Position of interest
/// @return      Direction vector towards target(s). If out of bounds, returns <0,0>
glm::vec2 PathfindSystem::GetDirectionAt(glm::vec2 pos) const
{
    // get coord (2D index), check bounds
    glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);
    if (coord.x == -1)
    {
        return { 0,0 };
    }

    glm::vec2 direction = m_Nodes[coord.y * m_Tilemap->GetDimensions().x + coord.x].direction;
    return direction == glm::vec2(0) ? direction : glm::normalize(direction);
}


/// @brief       Gets the travel distance (in tiles) to the destination
/// @param pos   Position from which to travel
/// @return      Amount of tiles to travel til destination. If out of bounds, 
///              returns -1.
int PathfindSystem::GetTravelDistanceAt(glm::vec2 pos) const
{
    if (!m_Tilemap)
    {
        glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);

        if (coord.x != -1)
            return m_Nodes[coord.y * m_Tilemap->GetDimensions().x + coord.x].cost;
    }

    return -1;
}


/// @brief       Checks if the given world position is walkable.
/// @param pos   World position
/// @return      Walkable or not
bool PathfindSystem::IsWalkable(glm::vec2 pos) const
{
    if (!m_Tilemap)
    {
        int width = m_Tilemap->GetDimensions().x;
        glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);

        if (coord.x != -1)
            return m_Nodes[width * coord.y + coord.x].type != Unwalkable;
    }

    return false;
}


/// @brief          Sets the entity (presumably, level map) which contains 
///                 the tilemap<int> to navigate
/// @param entity   Level map
void PathfindSystem::SetActiveTilemap(Entity* entity)
{
    m_Tilemap.Exit();
    m_Tilemap.Init(entity);
}



//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------

void PathfindSystem::OnUpdate(float dt)
{
    if (!m_Tilemap)
        return;

    if (m_Done.load())
    {
        if (m_Thread.joinable())
            m_Thread.join();

        m_Thread = std::thread(&PathfindSystem::explore, this);
    }

    (void) dt;
}

void PathfindSystem::OnExit()
{
    if (m_Thread.joinable())
        m_Thread.join();
}


/// @brief Gets Called by the Debug system to display debug information
void PathfindSystem::DebugWindow()
{}


//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief      Actual pathfinding. Updates all nodes. It ain't super optimized,
///             but it doens't need to be
void PathfindSystem::explore()
{
    if (!GetComponents().size())
        return;

    m_Done.store(false);

    // update walkability of tiles  TODO: not separately
    m_Nodes.resize(m_Tilemap->GetTilemap().size());
    for (int i = 0; i < m_Nodes.size(); i++)
    {
        m_Nodes[i].type = Unwalkable;
        m_Nodes[i].direction = glm::ivec2(0);

        for (int j : m_Walkables)
        {
            if (m_Tilemap->GetTilemap()[i] == j)
            {
                m_Nodes[i].type = Undiscovered;
                break;
            }
        }
    }

    // tile directions to pick from: counter clockwise, starting from left. (it's y-down)
    glm::ivec2 dir[8] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

    int width = m_Tilemap->GetDimensions().x;
    int height = (int)m_Nodes.size() / width;

    // init target destinations
    for (PathfinderTarget const* target : GetComponents())
    {
        if (!target->GetParentTransform())
            continue;

        glm::ivec2 tile = m_Tilemap->WorldPosToTileCoord(target->GetParentTransform()->GetTranslation());
        if (tile.x != -1)
        {
            int indx = tile.y * width + tile.x;
            m_Nodes[indx].type = Seen;
            m_Nodes[indx].direction = { 0,0 };
            m_Nodes[indx].cost = 0;
            m_Nodes[indx].priority = target->GetPriority() * 2;  // lower priority = higher number
        }
    }


    // keep going as long as at least one seen node is found
    bool found = true;

    while (found)
    {
        found = false;

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int indx = y * width + x;
                // only explore seen nodes.
                if (m_Nodes[indx].type != Seen)
                    continue;

                m_Nodes[indx].type = Explored;
                found = true;


                // and now the tough part: analyze nodes around the newly explored one.

                int k = 0;                              // 8 nodes around current one
                bool this_seen, previous_seen = false;  // for corner-checking (later)

                // this looks at all neighbor nodes one at a time
                while (k < 8)
                {
                    // i and j are x and y of this neighbor node
                    int i = x + dir[k].x;
                    int j = y + dir[k].y;

                    // check bounds
                    if (i < 0 || j < 0 || i >= width || j >= height
                        || m_Nodes[j * width + i].type == Unwalkable)
                        this_seen = false;
                    else
                    {
                        this_seen = true;

                        // cost for the node we're currently looking at:
                        // 14 if it's diagonal from current, 10 if it's straight
                        int new_cost = m_Nodes[indx].cost + ((k & 1) ? 14 : 10)
                            * (m_Nodes[indx].priority + 1);

                        // set new values if neighbor node isn't seen yet.
                        // or update it if new cost is lower.
                        Node& node = m_Nodes[j * width + i];

                        if (node.type == Undiscovered || node.cost > new_cost)
                        {
                            node.type = Seen;
                            node.cost = new_cost;
                            // direction: pointing back at current node  (y-up orientation)
                            node.direction = { -dir[k].x, dir[k].y };
                            node.priority = m_Nodes[indx].priority;
                        }
                    }

                    // The following is to ensure nodes don't point at corners (diagonally).
                    // Diagonal neighbor will be seen only if both non-diagonals around it were seen.

                    // even: non-diagonal neighbors.
                    if (~k & 1)
                    {
                        // if this and previous were seen, step back to the diagonal
                        // between them. Otherwise proceed to next non-diagonal.
                        if (this_seen && previous_seen)
                            k -= 1;
                        else
                            k += 2;

                        previous_seen = this_seen;
                    }
                    else
                    {
                        // odd: diagonals. If this diagonal was seen just now, that means
                        // the next non-diagonal is already seen too. Skip it, and skip
                        // the diagonal after it.
                        k += 3;
                    }

                    // after all that skipping around, backtrack to get the last node too
                    if (k == 8 && previous_seen && x - 1 >= 0   // (left of center node)
                        && m_Nodes[indx - 1].type == Seen)
                        k = 7;

                } // while k<8  (assessing nodes around current one)

            } // for x
        } // for y
    } // while found

    m_Done.store(true);  // signal that it's finished and ready to join.
}



//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief  gets this System's read methods
/// @return this System's read methods
ReadMethodMap< ISerializable > const& PathfindSystem::GetReadMethods() const
{
    static ReadMethodMap< PathfindSystem > const readMethods = {};
    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  writes this PathfindSystem to JSON
/// @return the JSON data of this PathfindSystem
nlohmann::ordered_json PathfindSystem::Write() const
{
    nlohmann::ordered_json json;
    return json;
}



//-----------------------------------------------------------------------------
//              Singleton stuff
//-----------------------------------------------------------------------------


/// @brief   gets the instance of PathfindSystem
/// @return  the instance of the PathfindSystem
PathfindSystem* PathfindSystem::GetInstance()
{
    static std::unique_ptr< PathfindSystem > s_Instance = nullptr;

    if (s_Instance == nullptr )
    {
        s_Instance.reset(new PathfindSystem());
    }
    return s_Instance.get();
}


/// @brief  Constructs the PathfindSystem
PathfindSystem::PathfindSystem() :
    ComponentSystem( "PathfindSystem" )
{}


//-----------------------------------------------------------------------------
