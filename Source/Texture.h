/// @file     Texture.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Header for the OpenGL texture wrapper
#pragma once
#include "glm/glm.hpp"


/// @brief          Stores texture data, and allows to bind it for rendering.
class Texture
{
public:
    /// @brief              Constructor: loads texture image from file upon initialization
    /// @param file_path    File to load
    Texture(const char* file_path = nullptr);

    /// @brief              Loads texture image from file (deletes old one if present)
    /// @param file_path    File to load
    void LoadImage(const char* file_path);


    /// @brief      Destructor: deletes texture data from GPU
    ~Texture();


    /// @brief      Sets this texture as active, so it can be used by texture shader
    /// @param slot (optional) texture slot - for drawing multiple at once ig?
    void Bind(unsigned int slot = 0);

    /// @brief      Returns the size of the original image as a vec2.
    /// @return     vec2: x = width ,  y = height
    glm::vec2 GetImageDimensions();


private:
    unsigned int m_TextureID = 0;    /// @brief   ID that texture is tracked with on GPU
    int m_W = 0, m_H = 0;            /// @brief   Width and height of the original image
};
