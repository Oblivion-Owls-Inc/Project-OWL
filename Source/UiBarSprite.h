/// @file       UiBarSprite.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      sprite that renders a ui bar
/// @version    0.1
/// @date       2023-10-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Sprite.h"


/// @brief  sprite that renders a ui bar
class UiBarSprite : public Sprite
{
//-----------------------------------------------------------------------------
public: // class UiBarSection
//-----------------------------------------------------------------------------


    /// @brief  struct that represents a data section in the UiBar
    struct UiBarSection : public ISerializable
    {
    //-----------------------------------------------------------------------------
    public: // members
    //-----------------------------------------------------------------------------


        /// @brief  the color of this UiBarSection
        glm::vec4 M_Color = { 1.0f, 0.0f, 0.0f, 1.0f };

        /// @brief  the value of this UiBarSection
        float M_Value = 1.0f;
        
        /// @brief  the inverse slope of the edge of this UiBarSection
        float M_Slope = 0.0f;


    //-----------------------------------------------------------------------------
    public: // inspection
    //-----------------------------------------------------------------------------


        /// @brief  inspects this UiBarSection
        /// @return whether a change was made
        bool Inspect();
        
        
    //-----------------------------------------------------------------------------
    private: // reading
    //-----------------------------------------------------------------------------


        /// @brief  reads the color of this UiBarSection
        /// @param  data    the json data to read from
        void readColor( nlohmann::ordered_json const& data );

        /// @brief  reads the value of this UiBarSection
        /// @param  data    the json data to read from
        void readValue( nlohmann::ordered_json const& data );

        /// @brief  reads the slope of the border of this UiBarSection
        /// @param  data    the json data to read from
        void readSlope( nlohmann::ordered_json const& data );


    //-----------------------------------------------------------------------------
    public: // reading / writing
    //-----------------------------------------------------------------------------


        /// @brief  gets the map of read methods for this UiBarSection
        /// @return the map of read methods for this UiBarSection
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


        /// @brief  Write all UiBarSection data to a JSON file.
        /// @return The JSON file containing the UiBarSection data.
        virtual nlohmann::ordered_json Write() const override;


    //-----------------------------------------------------------------------------
    };


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------    


    /// @brief  gets the UiBarSections of this UiBarSprite
    /// @return the UiBarSections of this UiBarSprite
    std::vector< UiBarSection >& GetSections();


    /// @brief  gets the border positions of this UiBarSprite
    /// @return the border positions of this UiBarSprite (min, max)
    glm::vec2 const& GetBorderPositions() const;


//-----------------------------------------------------------------------------
public: //  methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  renders this UiBarSprite
    virtual void Draw() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the sections in this UiBar
    std::vector< UiBarSection > m_Sections = {};


    /// @breif  the offset from the left edge of the sprite that the bar starts and ends
    glm::vec2 m_BorderPositions = { 0.0f, 1.0f };

    /// @breif  the vertical position of the slope rotation
    float m_RotationPosition = 0.5f;


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


    /// @brief  prepares the shader to render
    void prepareShader() const;


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this UiBarSprite in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads the sections in this UiBar
    /// @param  data    the json data to read from
    void readSections( nlohmann::ordered_json const& data );


    /// @breif  reads the offset from the left edge of the sprite that the bar starts and ends
    /// @param  data    the json data to read from
    void readBorderPositions( nlohmann::ordered_json const& data );

    /// @breif  reads the vertical position of the slope rotation
    /// @param  data    the json data to read from
    void readRotationPosition( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all UiBarSprite data to a JSON file.
    /// @return The JSON file containing the UiBarSprite data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    UiBarSprite();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this UiBarSprite
    /// @return the newly created clone of this UiBarSprite
    virtual UiBarSprite* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the UiBarSprite
    /// @param  other   the other UiBarSprite to copy
    UiBarSprite( const UiBarSprite& other );

    // diable = operator
    void operator =( UiBarSprite const& ) = delete;


//-----------------------------------------------------------------------------
};
