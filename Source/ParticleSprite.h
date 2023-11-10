/*****************************************************************//**
 * \file   ParticleSprite.h
 * \brief  Renders particles using data from SSBO.
 * 
 * \author Eli Tsereteli
 * \date   October 2023
 *********************************************************************/
#pragma once
#include "Sprite.h"

class Emitter;  // fwd ref

/// @brief      A version of Sprite for rendering tilemaps using GPU instancing.
class ParticleSprite : public Sprite
{
public:

    /// @brief        Default constructor
    ParticleSprite();

    /// @brief        Copy constructor
    /// @param other  Component to copy
    ParticleSprite(const ParticleSprite& other);

    /// @brief        Destructor: calls OnExit if needed
    ~ParticleSprite();

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
    virtual Component* Clone() const override;


public:
    // from Sprite

    /// @brief     Draws particles using gpu instancing.
    virtual void Draw() override;


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
