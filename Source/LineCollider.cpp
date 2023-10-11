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

/// @brief		  Read in the number of line segements to create.
/// @param stream The json data to read from.
void LineCollider::ReadNumLineSegments(Stream stream)
{
    m_NumSegments = stream.Read<unsigned int>();
}

/// @brief		  Read in the data for the line vectors.
/// @param stream The json data to read from.
void LineCollider::ReadLineVectors(Stream stream)
{
    // Create and add as many line segments as possible.
    for(size_t i = 0; i < m_NumSegments; i++)
    {
        for (auto& lineData : stream.GetArray())
        {
            assert( lineData.IsArray() );

            glm::vec2 p1 = Stream( lineData[0] ).Read<glm::vec2>();
            glm::vec2 p2 = Stream( lineData[1] ).Read<glm::vec2>();
            AddLineSegment( p1, p2 );
        }
    }
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief Map of all the component's read methods.
    ReadMethodMap< LineCollider > LineCollider::s_ReadMethods = {
        { "numSegments" , &ReadNumLineSegments},
        { "lineVectors" , &ReadLineVectors    }
    };

//-----------------------------------------------------------------------------
