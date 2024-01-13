/// @file     Light.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    
#pragma once
#include "Component.h"
#include <sstream>  // accept new Light


/// @brief       Uses TilemapSprite attached to parent entity to draw Light.
class Light : public Component
{
public:
    /// @brief   Default constructor
    Light();

    // Inherited via Component
    virtual Component * Clone() const override;



//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------
public:

    __inline void SetRadius(float r) { m_Radius = r; }

    __inline void SetOffset(glm::vec2 offset) { m_Offset = offset; }

    __inline float GetRadius() const { return m_Radius; }

    __inline glm::vec2 GetOffset() const { return m_Offset; }



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Initialization: emitter adds itself to particle system
    virtual void OnInit() override;

    /// @brief  Exit: emitter removes itself from particle system
    virtual void OnExit() override;

    /// @brief  Tweak properties in debug window
    //virtual void Inspector() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief  Index to keep track of this light within lighting system
    int m_Index = -1;

    /// @brief  Offset from parent
    glm::vec2 m_Offset = { 0,0 };

    /// @brief  Light radius
    float m_Radius = 1.0f;

    // TODO: add brightness


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief   the map of read methods for this Component
    static ReadMethodMap< Light > const s_ReadMethods;

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

public:

    /// @brief      Write all Light component data to a JSON file.
    /// @return     The JSON file containing the Light component data.
    //virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
