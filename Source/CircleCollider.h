/// @file       CircleCollider.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Circle-shaped collider
/// @version    0.1
/// @date       2023-09-13
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "Collider.h"
#include "Stream.h"

/// @class  CircleCollider
/// @brief  Circle-shaped Collider
class CircleCollider : public Collider
{
//-----------------------------------------------------------------------------
public: // constructor / inpector
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    CircleCollider();

    virtual void Inspector() override;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  gets this CircleCollider's radius
    /// @return this CircleCollider's radius
    __inline float GetRadius() const { return m_Radius; }

    /// @brief  sets this CircleCollider's radius
    /// @param  radius  the radius
    __inline void SetRadius( float radius ) { m_Radius = radius; }

//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief The radius of this CircleCollider
    float m_Radius;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  Reads the radius
    /// @param  stream  The json data to read from
    void readRadius( nlohmann::ordered_json const& data );

    /// @brief map of the read methods for this Component
    static ReadMethodMap< CircleCollider > s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    /// @brief  virtual component clone function
    /// @return new clone of component
    virtual CircleCollider* Clone() const override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  copy-constructor
    /// @param  other   the collider to copy
    CircleCollider( CircleCollider const& other );
    
//-----------------------------------------------------------------------------
};
