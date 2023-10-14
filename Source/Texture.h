/// @file     Texture.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Header for the OpenGL texture wrapper
#pragma once
#include "glm/glm.hpp"

#include "ISerializable.h"

#include "Mesh.h"

/// @brief          Stores texture data, and allows to bind it for rendering.
class Texture : public ISerializable
{

//-----------------------------------------------------------------------------
public: // constructors/destructors
//-----------------------------------------------------------------------------
    
    /// @brief  default constructor
    Texture();

    /// @brief              Constructor: loads texture image from file upon initialization
    /// @param  filepath    File to load
    Texture( std::string const& filepath, glm::ivec2 const& sheetDimensions = { 1, 1 } );

    /// @brief      Destructor: deletes texture data from GPU
    ~Texture();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief      Sets this texture as active, so it can be used by texture shader
    /// @param slot (optional) texture slot - for drawing multiple at once ig?
    void Bind( unsigned int slot = 0 ) const;

    /// @brief Used by the Debug to display information about this Texture
    void Inspect();

//-----------------------------------------------------------------------------
public: // acccessors
//-----------------------------------------------------------------------------

    /// @brief  Returns the size of the original image in pixels
    /// @return ivec2: x = width ,  y = height
    __inline glm::ivec2 const& GetPixelDimensions() const { return m_PixelDimensions; }

    /// @brief  Returns the size of the original image in pixels
    /// @return float: the aspec ratio of the Texture
    __inline float GetAspectRatio() const { return (float)(m_PixelDimensions.x * m_SheetDimensions.y) /
                                                          (m_PixelDimensions.y * m_SheetDimensions.x); }

    /// @brief  Returns how many tiles/frames are in the texture
    /// @return ivec2: x = width ,  y = height
    __inline glm::ivec2 const& GetSheetDimensions() const { return m_SheetDimensions; }


#if 1
    /// @brief  gets the Mesh this Sprite is using
    /// @return the Mesh this Sprite is using
    __inline Mesh const* GetMesh() const { return m_Mesh; }

    /// @brief  sets the Mesh this Sprite is using
    /// @param  mesh    the Mesh to set this Sprite to use
    __inline void SetMesh( Mesh const* mesh ) { m_Mesh = mesh; }
#endif

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the filepath of the Texture
    /// @param  stream  the data to read from
    void readFilepath( nlohmann::ordered_json const& data );

    /// @brief  reads the sheetDimension of the Texture
    /// @param  stream  the data to read from
    void readSheetDimensions( nlohmann::ordered_json const& data );

    /// @brief  gets called after reading all arugments 
    void AfterLoad();

    /// @brief  the read methods for textures
    static ReadMethodMap< Texture > const s_ReadMethods;

    /// @brief  gets the read method map
    /// @return the read method map
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  Loads texture image from file (deletes old one if present)
    void LoadImage();
 
//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
    
    /// @brief  the filepath to the texture
    std::string m_Filepath;

    /// @brief   ID that texture is tracked with on GPU
    unsigned int m_TextureID = 0;

    /// @brief   Width and height of the original image
    glm::ivec2 m_PixelDimensions;

    /// @brief   How many tiles/frames are in the texture
    glm::ivec2 m_SheetDimensions;

    /// @brief   Mesh to render texture onto
    Mesh const* m_Mesh = nullptr;

//-----------------------------------------------------------------------------
};
