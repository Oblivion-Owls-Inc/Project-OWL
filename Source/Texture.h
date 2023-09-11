#pragma once
#include "glm/glm.hpp"

class Texture
{
    unsigned int _textureID;
    int _w, _h, _BPP;

public:
    Texture(const char* file_path = nullptr);
    ~Texture();

    void loadImage(const char* file_path);

    void bind(unsigned int slot = 0);

    glm::vec2 getImageDimensions();
};
