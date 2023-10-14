/// @file     Text.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    A version of Sprite specifically for rendering text. Uses GPU
///           instancing to draw multiple letters simultaneously.
#pragma once
#include "Component.h"
#include <sstream>  // accept new text


/// @brief       Uses TilemapSprite attached to parent entity to draw text.
class Text : public Component
{
public:
    /// @brief   Default constructor
    Text();

    // Inherited via Component
    virtual Component * Clone() const override;


    /// @brief  gets the text of this Text
    /// @return the text of this Text
    __inline std::string const& GetText() const { return m_Text; }


    /// @brief          sets the text of this Text
    /// @param  text    the text of this Text
    __inline void SetText( std::string const& text ) { m_Text = text; loadTextIntoSprite(); } 


//-----------------------------------------------------------------------------
private: // overrides
//-----------------------------------------------------------------------------
    /// @brief  called when entering a scene
    virtual void OnInit() override;


//-----------------------------------------------------------------------------
private: // member variables / helpers
//-----------------------------------------------------------------------------

    /// @brief  text displayed by the textSprite
    std::string m_Text;

    /// @brief  Loads text array into TilemapSprite of the parent for rendering.
    void loadTextIntoSprite();


 //-------------------------------------------------------------------------
private: // Reading
 //-------------------------------------------------------------------------

    /// @brief  Read in the text this Text displays
    /// @param  stream  The json to read from.
    void readText( nlohmann::ordered_json const& data );

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Text > const s_ReadMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    //-------------------------------------------------------------------------
};
