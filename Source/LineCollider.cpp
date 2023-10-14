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
	m_LineCount++;
}

void LineCollider::AddLineSegment( vec2 p0, vec2 p1 )
{
	ColliderLineSegment temp{};
	temp.point[0] = p0;
	temp.point[1] = p1;
	m_LineSegments.push_back(temp);
    m_LineCount++;
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
void LineCollider::readLineVectors( nlohmann::json const& data )
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

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief Map of all the component's read methods.
    ReadMethodMap< LineCollider > LineCollider::s_ReadMethods = {
        { "lineVectors" , &readLineVectors    }
    };

//-----------------------------------------------------------------------------
