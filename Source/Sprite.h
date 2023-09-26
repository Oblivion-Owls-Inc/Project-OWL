/// @file     Sprite.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Sprite component - stores mesh and texture that together draw an image on screen. Supports spritesheets.
#pragma once
#include "Component.h"
#include "glm/glm.hpp"  // vec4
#include "Stream.h"     // Stream, Read
#include <string>       // std::string

// fwd refs
class Mesh;
class Texture;

class Sprite : public Component
{
    int _rows, _columns;
    int _layer;
    int _frame = 0;
    float _heightMult = 1.0f;
    float _opacity = 1.0f;
    std::string m_filename;
    bool m_IsTextured;

    Mesh* _mesh = nullptr;
    Texture* _texture = nullptr;
    glm::vec4 _color;

public:
    /// @brief Default Sprite Constructor.
    Sprite();

    /// @brief Copy constructor: shallow copy. Flyweight mesh and texture (eventually). Do not use rn.
    Sprite(Sprite const& other);

    /// @brief Destructor: frees texture and mesh... for now. Resource library should take care of it.
    ~Sprite();

    /// @brief Draws the mesh with texture (if one is present), or color.
    void draw();

    /// @brief          Sets current frame of the spritesheet.
    /// @param frame    New frame index
    void setFrame(int frame);

    /// @brief          Sets the rendering layer : 0 - 4.  0 is back, 4 is front.
    /// @param layer    Rendering layer to move this sprite to.
    void setLayer(int layer);

    /// @brief          Sets the opacity.
    /// param opacity   I'm not explaining this.
    void setOpacity(float opacity);

    /// @brief       Number to multiply width by, to get proportional height based on original image
    /// @return      float: original image's (height/width)
    float getHeightMultiplier() const;

    // Inherited via Component
    virtual Component* Clone() const override;

private: // reading

    /// @brief        Read in the number of rows for a sprite.
    /// @param stream The json to read from.
    void ReadRows(Stream stream);

    /// @brief        Does the sprite have a texture?
    /// @param stream The json to read from.
    void ReadIsTextured(Stream stream);

    /// @brief        Read in the colour for a sprite.
    /// @param stream The json to read from.
    void ReadColor(Stream stream);

    /// @brief        Read in the layer for a sprite.
    /// @param stream The json to read from.
    void ReadLayer(Stream stream);

    /// @brief        Read in the file name for a sprite.
    /// @param stream The json to read from.
    void ReadName(Stream stream);

    /// @brief Read in the number of columns for a sprite.
    /// @param stream the json to read from.
    void ReadColumns(Stream stream);

    /// @brief Takes all the read in data and makes a sprite.
    void ReadSprite( Stream );

    /// @brief the map of read methods for this Component
    static ReadMethodMap< Sprite > const readMethods;

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< Component > const& getReadMethods() override;
};