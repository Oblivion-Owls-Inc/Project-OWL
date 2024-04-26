/// @file       BeamSprite.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Sprite that renders a beam with a scrollable texture
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once




/// @brief  Sprite that renders a beam with a scrollable texture
class BeamSprite : public Sprite
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    BeamSprite();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the length of the beam
    /// @return the length of the beam
    float GetLength() const;

    /// @brief  sets the length of the beam
    /// @param  length  the length of the beam
    void SetLength( float length );


    /// @brief  gets the phase of the texture along the beam
    /// @return the phase of the texture along the beam
    float GetPhase() const;

    /// @brief  sets the phase of the texture along the beam
    /// @param  phase   the phase of the texture along the beam
    void SetPhase( float phase );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


    /// @brief  draws this BeamSprite
    virtual void Draw() override;


    /// @brief  checks if a a point in local space overlaps this Sprite (always false for beams)
    /// @return whether this Sprite overlaps the point
    virtual bool OverlapsLocalPoint( glm::vec2 const& ) const override { return false; }


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the length of the beam
    float m_Length = 1.0f;

    /// @brief  the phase of the texture along the beam
    float m_Phase = 0.0f;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  prepares the shader to render
    void prepareShader() const;


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for BeamSprite
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the length of the beam
    /// @param  data    the JSON data to read from
    void readLength( nlohmann::ordered_json const& data );

    /// @brief  reads the phase of the texture along the beam
    /// @param  data    the JSON data to read from
    void readPhase( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this BeamSprite
    /// @return the map of read methods for this BeamSprite
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this BeamSprite to JSON
    /// @return the JSON data of this BeamSprite
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this BeamSprite
    /// @return the newly created clone of this BeamSprite
    virtual BeamSprite* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the BeamSprite
    /// @param  other   the other BeamSprite to copy
    BeamSprite( BeamSprite const& other );


    // disable assignment operator
    void operator =( BeamSprite const& ) = delete;


//-----------------------------------------------------------------------------
};
