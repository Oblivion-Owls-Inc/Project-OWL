/// @file     Texture.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Header for the OpenGL texture wrapper
#pragma once
#include "glm/glm.hpp"

class Texture
{
    unsigned int _textureID;
    int _w, _h, _BPP;

public:
    /// @brief              Constructor: loads texture image from file upon initialization
    /// @param file_path    File to load
    Texture(const char* file_path = nullptr);

    /// @brief      Destructor: deletes texture data from GPU
    ~Texture();

    /// @brief              Loads texture image from file (deletes old one if present)
    /// @param file_path    File to load
    void loadImage(const char* file_path);

    /// @brief      Binds this texture (sets it as active, so it can be used by texture shader)
    void bind(unsigned int slot = 0);

    /// @brief      Returns the size of the original image as a vec2.
    /// @return     vec2: x = width ,  y = height
    glm::vec2 getImageDimensions();
};
