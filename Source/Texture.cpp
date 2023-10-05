/// @file     Texture.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Wrapper/manager for OpenGL texture.
#include "Texture.h"
#include "glew.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // loading images
#include <iostream>     // error msg


//-----------------------------------------------------------------------------
// public: constructors/destructors
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    Texture::Texture() :
        m_Filepath(),
        m_PixelDimensions( { 0, 0 } ),
        m_SheetDimensions( { 1, 1 } ),
        m_TextureID( 0 )
    {}

    /// @brief  Constructor: loads texture image from file upon initialization
    /// @param  filepath        File to load
    /// @param  sheetDimensions how many tiles/frames are in the texture
    Texture::Texture( std::string const& filepath, glm::ivec2 const& sheetDimensions ) :
        m_Filepath( filepath ),
        m_PixelDimensions( { 0, 0 } ),
        m_SheetDimensions( sheetDimensions ),
        m_TextureID( 0 )
    {
        LoadImage();
    }

    /// @brief      Destructor: deletes texture data from GPU
    Texture::~Texture()
    { 
        if (m_TextureID)
            glDeleteTextures(1, &m_TextureID);
    }

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief       Sets this texture as active, so it can be used by texture shader
    /// @param slot  (optional) texture slot - for drawing multiple at once ig?
    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief  reads the filepath of the Texture
    /// @param  stream  the data to read from
    void Texture::readFilepath( Stream stream )
    {
        m_Filepath = stream.Read<std::string>();
    }

    /// @brief  reads the sheetDimension of the Texture
    /// @param  stream  the data to read from
    void Texture::readSheetDimensions( Stream stream )
    {
        m_SheetDimensions = stream.Read<glm::ivec2>();
    }

    /// @brief  gets called after reading all arugments 
    void Texture::afterLoad( Stream )
    {
        if ( m_Filepath.empty() == false )
        {
            LoadImage();
        }
    }

    /// @brief  the read methods for textures
    ReadMethodMap< Texture > const Texture::s_ReadMethods = {
        { "Filepath",        &readFilepath        },
        { "SheetDimensions", &readSheetDimensions },
        { "AFTERLOAD",       &afterLoad           }
    };

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  Loads texture image from file (deletes old one if present)
    void Texture::LoadImage()
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
        unsigned char* content = stbi_load(m_Filepath.c_str(), &m_PixelDimensions.x, &m_PixelDimensions.y, &BPP, 4); // 4 channels - r,g,b,a

        if (content)
        {
            // load those pixels into texture buffer, and free them from stb.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_PixelDimensions.x, m_PixelDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, content);
            stbi_image_free(content);


            // sampling settings - scaling and wrapping
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
            std::cout << "TEXTURE ERROR: could not load file " << m_Filepath << std::endl;
    }

//-----------------------------------------------------------------------------
