/// @file     Texture.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Wrapper/manager for OpenGL texture.
#include "Texture.h"
#include "glew.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // loading images
#include <iostream>     // error msg


/// @brief              Constructor: loads texture image from file upon initialization
/// @param file_path    File to load
Texture::Texture(const char* file_path) 
{
    if (file_path)
        LoadImage(file_path);
}

/// @brief      Destructor: deletes texture data from GPU
Texture::~Texture()
{ 
    if (m_TextureID)
        glDeleteTextures(1, &m_TextureID);
}

/// @brief              Loads texture image from file (deletes old one if present)
/// @param file_path    File to load
void Texture::LoadImage(const char* file_path)
{
    if (m_TextureID)            // reloading with new data?
    {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = 0;
    }

    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    int BPP; // throwaway (don't need it) - bytes per pixel, the amount of channels received.
             // (I mean, why ever have less than 4?)

    // load the file as a chunk of pixels
    unsigned char* content = stbi_load(file_path, &m_W, &m_H, &BPP, 4); // 4 channels - r,g,b,a

    if (content)
    {
        // load those pixels into texture buffer, and free them from stb.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_W, m_H, 0, GL_RGBA, GL_UNSIGNED_BYTE, content);
        stbi_image_free(content);


        // sampling settings - scaling and wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
        std::cout << "TEXTURE ERROR: could not load file " << file_path << std::endl;
}

/// @brief       Sets this texture as active, so it can be used by texture shader
/// @param slot  (optional) texture slot - for drawing multiple at once ig?
void Texture::Bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

/// @brief    Returns the size of the original image as a vec2.
/// @return   vec2: x = width ,  y = height
glm::vec2 Texture::GetImageDimensions() { return {m_W, m_H}; }
