/*********************************************************************
* \file   Pathfinder.cpp
* \brief  Implementation of Pathfinder component. Runs vector field
*         pathfinding algo whenever parent's Tilemap updates.
* 
* \author Eli Tsereteli
*********************************************************************/
#include "Pathfinder.h"
#include "EntitySystem.h"   // parent, finding entities
#include "Transform.h"      // callback
#include "DebugSystem.h"    // error log
#include "InputSystem.h"    // inspector UI
#include "RenderSystem.h"   // inspector UI


/// @brief   Declaring them in header causes.. issues.


/// @brief      Default constructor
Pathfinder::Pathfinder() : Component(typeid(Pathfinder))
{
    m_Walkables.push_back(0);   // by default, 0 is considered empty space
                                // (hence walkable)
    //m_Targets.reserve(10);
}

/// @brief        Copy ctor
/// @param other  component to copy
Pathfinder::Pathfinder(Pathfinder const& other) : Component(other) { }

/// @return     A copy of this component
Pathfinder * Pathfinder::Clone() const { return new Pathfinder(*this); }



//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------

/// @brief  called when entering a scene - syncs with Tilemap
void Pathfinder::OnInit()
{
    m_Tilemap.SetOnConnectCallback(
        [ this ]()
        {
            m_Tilemap->AddOnTilemapChangedCallback(
                GetId(),
                std::bind(
                    &Pathfinder::onTilemapChangedCallback,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2,
                    std::placeholders::_3
                )
            );
        }
    );
    m_Tilemap.SetOnDisconnectCallback(
        [ this ]()
        {
            m_Tilemap->RemoveOnTilemapChangedCallback( GetId() );
        }
    );
    
    m_Tilemap.Init( GetEntity() );

    if ( m_Tilemap == nullptr )
    {
        return;
    }

    m_Nodes.resize( m_Tilemap->GetTilemap().size() );

    getTargets();

    // This will be depricated eventually.
    if (m_DestPos.x || m_DestPos.y)
        SetDestination(m_DestPos);

    exploreQueue();
}


/// @brief  called when exiting a scene - un-syncs (removes callback)
void Pathfinder::OnExit()
{
    m_Tilemap.Exit( );
    for (Target& t : m_Targets)
        if (t.transform)
            t.transform.Exit();

    if (m_Thread.joinable())
        m_Thread.join();
}


/// @brief  displays this Pathfinder's inspector
void Pathfinder::Inspector()
{
    inspectDestination();

    inspectWalkables();
}


//-----------------------------------------------------------------------------
//          Public methods
//-----------------------------------------------------------------------------

/// @brief       Sets new destination based on world position
/// @param pos   World position
void Pathfinder::SetDestination(glm::vec2 pos)
{
    if ( m_Tilemap == nullptr )
    {
        return;
    }

    // get coord (2D index), check bounds (and walkability of given tile)
    glm::ivec2 coord = m_Tilemap->WorldPosToTileCoord(pos);
    if ( coord.x == -1 )
    {
        std::cerr << "Warning: Pathfinder destination ( " << pos.x << ", " << pos.y << " ) is not within the Tilemap" << std::endl;
        return;
    }

    m_DestPos = pos;
    m_DestTile = coord;
    exploreQueue();
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
int Pathfinder::GetTravelDistanceAt(glm::vec2 pos) const
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


/// @brief         Add target entity for pathfinding
/// @param entity  Pointer to target entity
void Pathfinder::AddTarget(Entity* entity, Priority priority)
{
    Transform* t = entity->GetComponent<Transform>();
    if (!t)
        return;

    
    if (m_Targets.size() >= 10) // hard limit for now
        return;
    m_Targets.push_back({ ComponentReference<Transform>(), priority });
    m_Targets.back().transform.Init(entity);
    t->AddOnTransformChangedCallback( GetId(), std::bind(&Pathfinder::exploreQueue, this) );
}


/// @brief         Remove target entity from the list
/// @param entity  Pointer to target entity
void Pathfinder::RemoveTarget(Entity* entity)
{
    Transform* t = entity->GetComponent<Transform>();
    if (!t)
        return;
    
    t->RemoveOnTransformChangedCallback( GetId() );

    for (std::vector<Target>::iterator it = m_Targets.begin(); it < m_Targets.end(); ++it)
    {
        if (it->transform == t)
        {
            it->transform.Exit();
            m_Targets.erase(it);
            break;
        }
    }
}



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------


/// @brief  callback that gets called when the tilemap changes
/// @param  tilemap the tilemap that was changed
void Pathfinder::onTilemapChangedCallback( Tilemap< int >*, glm::ivec2 const&, int const& )
{
    exploreQueue();
}

/// @brief      For multithreading. Calls explore() in a separate thread if it's 
///             not running already, or tells it to run again after current iteration.
void Pathfinder::exploreQueue()
{
    // Done flag is true when explore() is finished and is waiting to join
    if (m_Done.load())
    {
        m_Thread.join();
        m_Done = false;   
    }
    
    // if the thread is already running, it'll keep running after current iteration
    m_Dirty.store(true);

    // if it isn't running already, start it up.
    if (!m_Thread.joinable())
        m_Thread = std::thread(&Pathfinder::explore, this);
}


/// @brief      Actual pathfinding. Updates all nodes. It ain't super optimized,
///             but it doens't need to be
void Pathfinder::explore()
{
    // keep going as long as exploreQueue() keeps setting this to true
    while (m_Dirty.load())
    {
        m_Dirty.store(false);

        // update walkability of tiles
        int size = (int) m_Nodes.size();
        for (int i=0; i<size; i++)
        {
            m_Nodes[i].type = Unwalkable;
            m_Nodes[i].direction = glm::ivec2( 0 );

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

        // init first node (if it's not default)
        if (m_DestTile.x || m_DestTile.y)
        {
            int indx = m_DestTile.y * width + m_DestTile.x;
            m_Nodes[indx].type = Seen;
            m_Nodes[indx].direction = {0,0};
            m_Nodes[indx].cost = 0;
            m_Nodes[indx].priority = 0;
        }

        // and the target destinations
        int ti = (int)m_Targets.size();  // target index (start from back)
        while (ti--)
        {
            // if this target is gone, reference to its transform will be null
            //if (!m_Targets[ti].transform)
            //{
            //    m_Targets.erase(m_Targets.begin() + ti);
            //    continue;
            //}

            glm::ivec2 tile = m_Tilemap->WorldPosToTileCoord(m_Targets[ti].transform->GetTranslation());
            if (tile.x != -1)
            {
                int indx = tile.y * width + tile.x;
                m_Nodes[indx].type = Seen;
                m_Nodes[indx].direction = { 0,0 };
                m_Nodes[indx].cost = 0;
                m_Nodes[indx].priority = static_cast<int>(m_Targets[ti].priority) * 2;  // lower priority = higher number
            }
        }


        // keep going as long as at least one seen node is found
        bool found = true;

        while (found)
        {
            found = false;

            for (int y=0; y<height; y++)
            {
                for (int x=0; x<width; x++)
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
                            || m_Nodes[j*width + i].type == Unwalkable)
                            this_seen = false;
                        else
                        {
                            this_seen = true;

                            // cost for the node we're currently looking at:
                            // 14 if it's diagonal from current, 10 if it's straight
                            int new_cost = m_Nodes[indx].cost + ((k&1) ? 14 : 10)
                                         * (m_Nodes[indx].priority+1);

                            // set new values if neighbor node isn't seen yet.
                            // or update it if new cost is lower.
                            Node & node = m_Nodes[j*width + i];

                            if (node.type == Undiscovered || node.cost > new_cost)
                            {
                                node.type = Seen;
                                node.cost = new_cost;
                                // direction: pointing back at current node  (y-up orientation)
                                node.direction = {-dir[k].x, dir[k].y};
                                node.priority = m_Nodes[indx].priority;
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
                        if (k == 8 && previous_seen &&  x-1 >= 0   // (left of center node)
                                &&  m_Nodes[indx-1].type == Seen)
                            k = 7;

                    } // while k<8  (assessing nodes around current one)

                } // for x
            } // for y
        } // while found
    }// while is dirty (atomic)

    m_Done.store(true);  // signal that it's finished and ready to join.
}



/// @brief  inspector for choosing and displaying the destination of this Pathfinder
void Pathfinder::inspectDestination()
{
    if ( m_Tilemap == nullptr )
    {
        return;
    }

    // widget to set destination
    if ( ImGui::DragFloat2( "Destination", &m_DestPos[0], 0.05f ) )
    {
        SetDestination( m_DestPos );
    }

    // button to click destination point in scene
    static bool selectTargetMode = false;
    if ( ImGui::Button( selectTargetMode ? "click in the scene" : "Select Destination" ) )
    {
        selectTargetMode = !selectTargetMode;
    }

    if ( selectTargetMode && Input()->GetMouseTriggered( GLFW_MOUSE_BUTTON_1 ) )
    {
        SetDestination( Input()->GetMousePosWorld() );
        selectTargetMode = false;
    }

    // debug draw the destination tile
    glm::mat4 tileToWorld = m_Tilemap->GetTilemapToWorldMatrix();
    Renderer()->DrawRect(
        (glm::vec2)( tileToWorld * glm::vec4( (glm::vec2)m_DestTile + glm::vec2(0.5f), 0, 1)),
        m_Tilemap->GetTileScale(),
        GetEntity()->GetComponent< Transform >()->GetRotation(),
        glm::vec4( -1.0f, 0.0f, -1.0f, 0.0f )
    );
}

/// @brief  inspector for choosing which tiles are walkable
void Pathfinder::inspectWalkables()
{
    if ( ImGui::BeginListBox( "walkable tiles", ImVec2( 82.0f, (float)m_Walkables.size() * 24.0f + 26.0f ) ) == false )
    {
        return;
    }

    for ( int i = 0; i < m_Walkables.size(); ++i )
    {
        ImGui::PushID( i );

        ImGui::PushItemWidth( 50 );
        ImGui::DragInt( "", &m_Walkables[ i ], 0.05f, -1, INT_MAX );

        ImGui::SameLine();

        if ( ImGui::Button( "X", ImVec2( 20, 20 ) ) )
        {
            m_Walkables.erase( m_Walkables.begin() + i );
        }

        ImGui::PopID();
    }

    if ( ImGui::Button( "+", ImVec2( 50, 20 ) ) )
    {
        m_Walkables.push_back( 0 );
    }

    ImGui::EndListBox();
}


/// @brief  Finds targets by their names, and stores their transforms for pathfinding
void Pathfinder::getTargets()
{
    m_Targets.clear();

    for (std::string const& s : m_TargetNames)
    {
        size_t space_loc = s.find(' ');
        Entity* entity = nullptr;
        Priority p = Highest;

        if (space_loc == std::string::npos)
        {
            entity = Entities()->GetEntity(s);
            if (!entity)
                Debug() << "Pathfinder warning: entity \"" << s
                        << "\" does not exist." << std::endl;
        }
        else
        {
            entity = Entities()->GetEntity(s.substr(0, space_loc));
            if (!entity)
                Debug() << "Pathfinder warning: entity \"" << s.substr(0, space_loc)
                        << "\" does not exist." << std::endl;

            size_t last_space_loc = s.rfind(' ');
            std::string s_priority = s.substr(last_space_loc + 1, s.size() - last_space_loc - 1);
            if (s_priority == "high")
                p = High;
            else if (s_priority == "highest")
                p = Highest;
            else if (s_priority == "mid")
                p = Mid;
            else if (s_priority == "low")
                p = Low;
            else
                Debug() << "Pathfinder warning: unknown priority \"" << s_priority << "\". "
                        << "Priority names are : low, mid, high, highest.\n"
                        << "Setting priority for " << s.substr(0, space_loc)
                        << " to default (highest)." << std::endl;
        }

        if (entity)
            AddTarget(entity, p);
    }
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

/// @brief          Read in the target entities
/// @param data     The json to read from.
void Pathfinder::readTargets( nlohmann::ordered_json const& data )
{
    m_TargetNames.clear();

    for (std::string const& s : data)
        m_TargetNames.push_back(s);
}


/// @brief          The map of read methods for this Component
ReadMethodMap< Pathfinder > const Pathfinder::s_ReadMethods = {
    { "Destination", &readDestination  },
    { "Walkables",   &readWalkables  },
    { "Targets",     &readTargets  }
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



// Custom constructors for Target

Pathfinder::Target::Target(Target&& other) noexcept
{
    priority = other.priority;
    transform = std::move(other.transform);
}

Pathfinder::Target::Target(ComponentReference<Transform> t, Priority p)
{
    priority = p;
    transform = std::move(t);
}

//-----------------------------------------------------------------------------
