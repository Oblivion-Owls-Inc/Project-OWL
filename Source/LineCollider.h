#pragma once

#include "basics.h"
#include "Collider.h"
#include <vector>

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
		void AddLineSegment(const vec2* p0, const vec2* p1);
		void AddLineSegment(vec2 p0, vec2 p1);
		bool CheckIfColliding(const Collider* other) override;
		virtual void OnFixedUpdate() override {};


	private:

		std::vector<ColliderLineSegment> m_lineSegments;

		unsigned int lineCount;

		unsigned int counter;

		bool doesDamage = false;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the read methods for this Component
    static std::map< std::string, ReadMethod< LineCollider > > s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual std::map< std::string, ReadMethod< Component > > const& GetReadMethods() override;

};

