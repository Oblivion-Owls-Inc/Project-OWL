/*********************************************************************
* \file   Pathfinder.cpp
* \brief  Implementation of Pathfinder component. Runs vector field
*         pathfinding algo whenever parent's Tilemap updates.
* 
* \author Eli Tsereteli
*********************************************************************/
#include "Pathfinder.h"
#include "Entity.h"     // parent
#include "DebugSystem.h"

                                                                   /// @brief      Default constructor
Pathfinder::Pathfinder() : Component(typeid(Pathfinder)) 
{
    m_Walkables.push_back(0);   // by default, 0 is considered empty space
    // (hence walkable)
}

/// @return     A copy of this component
Component * Pathfinder::Clone() const { return new Pathfinder(*this); }



//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------

/// @brief  called when entering a scene - syncs with Tilemap
void Pathfinder::OnInit()
{
    m_Tilemap = GetEntity()->GetComponent< Tilemap<int> >();

#ifndef NDEBUG
    if ( m_Tilemap == nullptr )
    {
        Debug() << "Warning: Pathfinder parent does not have Tilemap component." << std::endl;
        return;
    }
#endif

    m_Tilemap->AddOnTilemapChangedCallback( GetId(), std::bind(&Pathfinder::explore, this));
    m_Nodes.resize( m_Tilemap->GetTilemap().size() );
    SetDestination(m_DestPos);
}


/// @brief  called when exiting a scene - un-syncs (removes callback)
void Pathfinder::OnExit()
{
    m_Tilemap->RemoveOnTilemapChangedCallback( GetId() );
}


//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------

/// @brief       Sets new destination based on world position
/// @param pos   World position
void Pathfinder::SetDestination(glm::vec2 pos)
{
    // get coord (2D index), check bounds (and walkability of given tile)
    glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);
    if ( coord.x == -1 )
    {
        std::cerr << "Warning: Pathfinder destination ( " << pos.x << ", " << pos.y << " ) is not within the Tilemap" << std::endl;
        return;
    }

    m_DestPos = pos;
    m_DestTile = coord;
    explore();
}


/// @brief       Gets normalized direction vector at given world position.
///              The vector points to next nearest spot on the path towards
///              the destination.
/// @param pos   Position of interest
/// @return      Direction vector towards destination. If out of bounds, returns <0,0>
glm::vec2 Pathfinder::GetDirectionAt(glm::vec2 pos) const
{
    // get coord (2D index), check bounds
    glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);
    if (coord.x == -1)
    {
        return { 0,0 };
    }

    glm::vec2 direction = m_Nodes[coord.y * m_Tilemap->GetDimensions().x + coord.x].direction;
    return direction == glm::vec2( 0 ) ? direction : glm::normalize( direction );
}



/// @brief       Gets the travel distance (in tiles) to the destination
/// @param pos   Position from which to travel
/// @return      Amount of tiles to travel til destination. If out of bounds, 
///              returns -1.
int Pathfinder::GetTravelDistanceAt(glm::vec2 pos)
{
    glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);
    if (coord.x == -1)
        return -1;

    return m_Nodes[coord.y * m_Tilemap->GetDimensions().x + coord.x].cost;
}


/// @brief       Checks if the given world position is walkable.
/// @param pos   World position
/// @return      Walkable or not
bool Pathfinder::IsWalkable(glm::vec2 pos) const
{
    int width = m_Tilemap->GetDimensions().x;
    glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);

    if (coord.x != -1)
        return m_Nodes[width*coord.y + coord.x].type != Unwalkable;
    else
        return false;
}



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------


/// @brief      Actual pathfinding. Updates all nodes. It ain't super optimized,
///             but it doens't need to be
void Pathfinder::explore()
{
    // update walkability of tiles
    int size = (int) m_Nodes.size();
    for (int i=0; i<size; i++)
    {
        m_Nodes[i].type = Unwalkable;

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
    int height = size / width;

    // init first node
    m_Nodes[m_DestTile.y * width + m_DestTile.x].type = Seen;
    m_Nodes[m_DestTile.y * width + m_DestTile.x].direction = {0,0};


    // keep going as long as at least one seen node is found
    bool found = true;

    while (found)
    {
        found = false;

        for (int y=0; y<height; y++)
        {
            for (int x=0; x<width; x++)
            {
                // only explore seen nodes.
                if (m_Nodes[y*width + x].type != Seen)
                    continue;

                m_Nodes[y*width + x].type = Explored;
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
                        || m_Nodes[j*width + i].type == Unwalkable)
                        this_seen = false;
                    else
                    {
                        this_seen = true;

                        // cost for the node we're currently looking at:
                        // 14 if it's diagonal from current, 10 if it's straight
                        int new_cost = m_Nodes[y*width + x].cost + ((k&1) ? 14 : 10);

                        // set new values if neighbor node isn't seen yet.
                        // or update it if new cost is lower.
                        Node & node = m_Nodes[j*width + i];

                        if (node.type == Undiscovered || node.cost > new_cost)
                        {
                            node.type = Seen;
                            node.cost = new_cost;
                            // direction: pointing back at current node  (y-up orientation)
                            node.direction = {-dir[k].x, dir[k].y};
                        }
                    }

                    // The following is to ensure nodes don't point at corners (diagonally).
                    // Diagonal neighbor will be seen only if both non-diagonals around it were seen.

                    // even: non-diagonal neighbors.
                    if (~k&1)
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
                    if (k == 8 && previous_seen &&  x-1 >= 0 
                        &&  m_Nodes[y*width + x-1].type == Seen)
                        k = 7;

                } // while k<8  (assessing nodes around current one)

            } // for x
        } // for y
    } // while found
}




//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief          Read in the destination position
    /// @param stream   The json to read from.
    void Pathfinder::readDestination( nlohmann::ordered_json const& data )
    {
        m_DestPos = Stream::Read< 2, float>(data);
    }

    /// @brief          Read in the array of walkable tiles
    /// @param stream   The json to read from.
    void Pathfinder::readWalkables( nlohmann::ordered_json const& data )
    {
        m_Walkables.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            int x = Stream::Read<int>(data[i]);
            m_Walkables.push_back(x);
        }
    }


    /// @brief          The map of read methods for this Component
    ReadMethodMap< Pathfinder > const Pathfinder::s_ReadMethods = {
        { "Destination", &readDestination  },
        { "Walkables",   &readWalkables  }
    };


//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------


    /// @brief Write all Sprite component data to a JSON file.
    /// @return The JSON file containing the Sprite component data.
    nlohmann::ordered_json Pathfinder::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Destination" ] = Stream::Write( m_DestPos );
        json[ "Walkables" ] = Stream::Write( m_Walkables );

        return json;
    }


//-----------------------------------------------------------------------------
