///*****************************************************************/
/// @file	 LineCollider.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/5/2023
/// @brief   LineCollider class header
/// @details This class contains the LineCollider component
///*****************************************************************/

#pragma once
///*****************************************************************/
/// Includes
///*****************************************************************/
#include "basics.h"
#include "Collider.h"
#include <vector>
#include <map>
#include <string>
#include "Stream.h"

using namespace glm;

///*****************************************************************/
/// LineCollider class
/// @brief This class contains the LineCollider component which is used
///  for collisions on a game object.
///*****************************************************************/
class LineCollider : public Collider
{
private:
	private:

		typedef struct ColliderLineSegment
		{
			// A single line segment (P0 and P1).
			glm::vec2 point[2];

		} ColliderLineSegment;

public:
	public:

		LineCollider();
		Component* Clone() const;
		void AddLineSegment(const vec2* p0, const vec2* p1);
		void AddLineSegment(vec2 p0, vec2 p1);

		virtual void Inspector() override;

private:

	std::vector<ColliderLineSegment> m_LineSegments;

	unsigned int m_LineCount;

	unsigned int m_Counter;

	bool doesDamage = false;

    // Number of line segments to make, read from file.
    unsigned int m_NumSegments;

private: // reading

	/// @brief		  Read in the data for the line vectors.
	/// @param stream The json data to read from.
	void readLineVectors( nlohmann::json const& data );
    /// @brief The map of read methods for the LineCollider.
    static ReadMethodMap< LineCollider > s_ReadMethods;

	/// @brief  Gets the map of read methods for this component.
	/// @return The map of read methods for this component.
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }
};

