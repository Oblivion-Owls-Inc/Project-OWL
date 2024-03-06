#include "glm/glm.hpp"
#include "Entity.h"
#include "Sprite.h"
#include "RigidBody.h"
#include "Transform.h"

#include "DebugSystem.h"
#include "LineCollider.h"
#include "CircleCollider.h"

LineCollider::LineCollider() :
    Collider( typeid( LineCollider ) )
{}

Component* LineCollider::Clone() const
{
	return new LineCollider( *this );
}

/// @brief    Add a line segment to the line collider's list.
/// @param p0 The line segments starting position.
/// @param p1 The line segments ending position.
void LineCollider::AddLineSegment(const vec2* p0, const vec2* p1)
{
	ColliderLineSegment temp{};
	temp.point[0] = *p0;
	temp.point[1] = *p1;
	m_LineSegments.push_back(temp);
}

void LineCollider::AddLineSegment( vec2 p0, vec2 p1 )
{
	ColliderLineSegment temp{};
	temp.point[0] = p0;
	temp.point[1] = p1;
	m_LineSegments.push_back(temp);
}

void LineCollider::Inspector()
{
    int i = 0;
    ///see a list of the lines
    for (auto line : m_LineSegments)
    {
        ++i;
		ImGui::Text("Line %d : [%f, %f] - [%f, %f]", i ,  line.point[0].x, line.point[0].y, line.point[1].x, line.point[1].y);
	}
}

/// @brief		  Read in the data for the line vectors.
/// @param stream The json data to read from.
void LineCollider::readLineVectors( nlohmann::ordered_json const& data )
{
    assert( data.is_array() );
    // Create and add as many line segments as needed.
    for( int i = 0; i < data.size(); ++i )
    {
        assert( data[i].is_array() );

        glm::vec2 p1 = Stream::Read< 2, float >( data[ i ][ 0 ] );
        glm::vec2 p2 = Stream::Read< 2, float >( data[ i ][ 1 ] );
        AddLineSegment( p1, p2 );
    }
}

/// @brief  Write all LineCollider component data to a JSON file.
/// @return The JSON file containing the LineCollider component data.
nlohmann::ordered_json LineCollider::Write() const
{
    nlohmann::ordered_json data;

    nlohmann::ordered_json& lines = data["LineVectors"];
    for (ColliderLineSegment const& segment : m_LineSegments)
    {
        lines[0] = Stream::Write( segment.point[0] );
        lines[1] = Stream::Write( segment.point[1] );
    }

    return nlohmann::ordered_json();
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief Map of all the component's read methods.
    ReadMethodMap< LineCollider > LineCollider::s_ReadMethods = {
        { "lineVectors" , &readLineVectors    }
    };

//-----------------------------------------------------------------------------
