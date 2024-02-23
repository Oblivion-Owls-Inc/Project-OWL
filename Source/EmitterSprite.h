/*****************************************************************//**
 * \file   EmitterSprite.h
 * \brief  Renders particles using data from SSBO.
 * 
 * \author Eli Tsereteli
 * \date   October 2023
 *********************************************************************/
#pragma once
#include "Sprite.h"

class Emitter;  // fwd ref

/// @brief      A version of Sprite for rendering tilemaps using GPU instancing.
class EmitterSprite : public Sprite
{
public:

    /// @brief        Default constructor
    EmitterSprite();

    /// @brief        Copy constructor
    /// @param other  Component to copy
    EmitterSprite(const EmitterSprite& other);

    /// @brief        Destructor: calls OnExit if needed
    ~EmitterSprite();

//-----------------------------------------------------------------------------
//          Virtual overrides
//-----------------------------------------------------------------------------
private:
    // from Component
    
    /// @brief  called when entering a scene
    virtual void OnInit() override;

    /// @brief  called when exiting a scene
    virtual void OnExit() override;

    /// @return  Copy of this component
    virtual EmitterSprite* Clone() const override;


public:
    // from Sprite

    /// @brief     Draws particles using gpu instancing.
    virtual void Draw() override;


    /// @brief  checks if a a point in local space overlaps this Sprite
    /// @return whether this Sprite overlaps the point
    virtual bool OverlapsLocalPoint( glm::vec2 const& ) const override { return false; }

//-----------------------------------------------------------------------------
//              Data
//-----------------------------------------------------------------------------
private:
    unsigned int m_VAO = 0;       /// @brief   VAO that links mesh and SSBO buffers
    Emitter* m_Emitter = nullptr; /// @brief   Parent's emitter component



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------
private:

    /// @brief  Creates the VAO needed to use ParticleSystem's SSBO for instancing
    void initInstancingStuff();
};
