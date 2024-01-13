/// @file     Light.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Light source component. It shines.
#pragma once
#include "Component.h"
#include <sstream>  // accept new Light


/// @brief       Light source.
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

    /// @brief  Sets the light radius
    __inline void SetRadius(float r) { m_Radius = r; }

    /// @brief  Sets the strength/brightness of the light 
    __inline void SetStrength(float s) { m_Strength = s; }

    /// @brief  Sets the position offset from parent entity
    __inline void SetOffset(glm::vec2 offset) { m_Offset = offset; }

    /// @return  Light radius
    __inline float GetRadius() const { return m_Radius; }

    /// @return  Light strength
    __inline float GetStrength() const { return m_Strength; }

    /// @return  Position offset from parent entity
    __inline glm::vec2 GetOffset() const { return m_Offset; }



//-----------------------------------------------------------------------------
//              Overrides
//-----------------------------------------------------------------------------
private:

    /// @brief  Initialization: light adds itself to the lighting system
    virtual void OnInit() override;

    /// @brief  Exit: light removes itself from lighting system
    virtual void OnExit() override;

    /// @brief  Tweak properties in debug window
    virtual void Inspector() override;



//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:

    /// @brief  Index to keep track of this light within lighting system
    int m_Index = -1;

    /// @brief  Position offset from parent entity
    glm::vec2 m_Offset = { 0,0 };

    /// @brief  Light radius
    float m_Radius = 2.0f;

    // @brief   Stength (brightness) of the light
    float m_Strength = 0.8f;


//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------
private:

    /// @brief       Reads in the radius of the light
    /// @param data  json data
    void readRadius( nlohmann::ordered_json const& data );

    /// @brief       Reads in the strength of the light
    /// @param data  json data
    void readStrength(nlohmann::ordered_json const& data);

    /// @brief       Reads in the position offset
    /// @param data  json data
    void readOffset(nlohmann::ordered_json const& data);

    /// @brief   the map of read methods for this Component
    static ReadMethodMap< Light > const s_ReadMethods;


public:

    /// @brief   gets the map of read methods for this Component
    /// @return  the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief      Write all Light component data to a JSON file.
    /// @return     The JSON file containing the Light component data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
