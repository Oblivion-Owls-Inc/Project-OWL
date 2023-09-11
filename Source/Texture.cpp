#include "Texture.h"
#include "glew.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>        // temporary, until DebugConsole is complete

Texture::Texture(const char* file_path) : _textureID(0), _w(0), _h(0), _BPP(0)
{
    if (file_path)
        loadImage(file_path);
}

Texture::~Texture()
{ 
    if (_textureID)
        glDeleteTextures(1, &_textureID);
}


void Texture::loadImage(const char* file_path)
{
    if (_textureID)            // reloading with new data?
    {
        glDeleteTextures(1, &_textureID);
        _textureID = 0;
    }

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_2D, _textureID);

    // load the file as a chunk of pixels
    unsigned char* content = stbi_load(file_path, &_w, &_h, &_BPP, 4); // 4 channels - r,g,b,a

    if (content)
    {
        // load those pixels into texture buffer, and free them from stb.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _w, _h, 0, GL_RGBA, GL_UNSIGNED_BYTE, content);
        stbi_image_free(content);


        // sampling settings - scaling and wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else
        std::cout << "TEXTURE ERROR: could not load file " << file_path << std::endl;
}


void Texture::bind(unsigned int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _textureID);
}

glm::vec2 Texture::getImageDimensions() { return {_w, _h}; }
