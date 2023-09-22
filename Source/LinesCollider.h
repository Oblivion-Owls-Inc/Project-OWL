#pragma once

#include "basics.h"
#include "Collider.h"
#include <vector>
#include <map>
#include <string>
#include "Stream.h"

using namespace glm;

class LineCollider : public Collider
{
private:

	typedef struct ColliderLineSegment
	{
		// A single line segment (P0 and P1).
		vec2 point[2];

	} ColliderLineSegment;

public:

	LineCollider();
	Component* Clone() const;
	void AddLineSegment( const vec2* p0, const vec2* p1 );
	void AddLineSegment( vec2 p0, vec2 p1 );
	bool CheckIfColliding( const Collider* other ) override;
	virtual void OnFixedUpdate() override {};


private:

	std::vector<ColliderLineSegment> m_lineSegments;

	unsigned int lineCount;

	unsigned int counter;

	bool doesDamage = false;

private: // reading

	// Number of line segments to make, read from file.
	unsigned int m_NumSegments;
	/// @brief The map of read methods for the LineCollider.
	static std::map< std::string, ReadMethod< LineCollider > > readMethods;

	/// @brief		  Read in the number of line segements to create.
	/// @param stream The json data to read from.
	void ReadNumLineSegments( Stream stream );

	/// @brief		  Read in the data for the line vectors.
	/// @param stream The json data to read from.
	void ReadLineVectors( Stream stream );

	/// @brief  Gets the map of read methods for this component.
	/// @return The map of read methods for this component.
	virtual std::map< std::string, ReadMethod< Component > > const& getReadMethods();
};

