/// @file       Nineslice.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      sprite that renders a nineslice
/// @version    0.1
/// @date       2024-02-20
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#include "Sprite.h"


/// @brief  sprite that renders a nineslice
class Nineslice : public Sprite
{
//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


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


    /// @brief  renders this Nineslice
    virtual void Draw() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    
    /// @brief  the internal edges of the nineslice in negative directions
    glm::vec2 m_MinInternalEdge = glm::vec2( 0.25, 0.25 );

    /// @brief  the internal edges of the nineslice in positive directions
    glm::vec2 m_MaxInternalEdge = glm::vec2( 0.75, 0.75 );


    /// @brief  the widths of the minimum borders
    glm::vec2 m_MinBorderWidths = glm::vec2( 0.25, 0.25 );

    /// @brief  the widths of the maximum borders
    glm::vec2 m_MaxBorderWidths = glm::vec2( 0.25, 0.25 );


//-----------------------------------------------------------------------------
private: // helper methods
//-----------------------------------------------------------------------------


    /// @brief  prepares the shader to render
    void prepareShader() const;


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    
    /// @brief  displays this Nineslice in the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the internal edges of the nineslice in negative directions
    /// @param  data    the JSON data to read from
    void readMinInternalEdge( nlohmann::ordered_json const& data );

    /// @brief  reads the internal edges of the nineslice in positive directions
    /// @param  data    the JSON data to read from
    void readMaxInternalEdge( nlohmann::ordered_json const& data );


    /// @brief  reads the widths of the minimum borders
    /// @param  data    the JSON data to read from
    void readMinBorderWidths( nlohmann::ordered_json const& data );

    /// @brief  reads the widths of the maximum borders
    /// @param  data    the JSON data to read from
    void readMaxBorderWidths( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all Nineslice data to a JSON file.
    /// @return The JSON file containing the Nineslice data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    Nineslice();


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Nineslice
    /// @return the newly created clone of this Nineslice
    virtual Nineslice* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Nineslice
    /// @param  other   the other Nineslice to copy
    Nineslice( const Nineslice& other );

    // diable = operator
    void operator =( Nineslice const& ) = delete;


//-----------------------------------------------------------------------------
};
