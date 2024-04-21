/// @file     Texture.h
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Header for the OpenGL texture wrapper
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
#pragma once
#include "pch.h" 

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

    /// @brief  Constructor: loads texture image from file upon initialization
    /// @param  filepath        File to load
    /// @param  sheetDimensions x=columns, y=rows (of the spritesheet)
    /// @param  pivot           the pivot point of this Texture
    Texture( std::string const& filepath, glm::ivec2 const& sheetDimensions = { 1, 1 }, glm::vec2 const& pivot = { 0.5f, 0.5f } );

    /// @brief      Destructor: deletes texture data from GPU
    ~Texture();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  calculates the uv offset for a given frame offset
    /// @param  frameIndex  the index of the frame to get the UV offset of
    /// @return the UV offset of the frame index
    glm::vec2 GetUvOffset( int frameIndex ) const;


    /// @brief      Sets this texture as active, so it can be used by texture shader
    /// @param slot (optional) texture slot - for drawing multiple at once ig?
    void Bind( unsigned int slot = 0 ) const;

    /// @brief Used by the Debug to display information about this Texture
    void Inspect();


    /// @brief  displays this Texture in ImGui
    /// @param  scale       the scale to display this image at
    /// @param  frameIndex  the index of the frame of this texture to display
    /// @param  tintColor   the tint color of the displayed button
    /// @param  borderColor the border color of the displayed button
    void DisplayInInspector(
        int frameIndex = 0,
        float scale = 100.0f,
        glm::vec4 const& tintColor = { 1.0f, 1.0f, 1.0f, 1.0f },
        glm::vec4 const& borderColor = { 0.0f, 0.0f, 0.0f, 0.0f }
    ) const;


//-----------------------------------------------------------------------------
public: // acccessors
//-----------------------------------------------------------------------------

    /// @brief  Returns the size of the original image in pixels
    /// @return ivec2: x = width ,  y = height
    __inline glm::ivec2 const& GetPixelDimensions() const { return m_PixelDimensions; }

    /// @brief  Returns the aspect ratio (x / y) of the texture
    /// @return float: the aspect ratio of the Texture
    __inline float GetAspectRatio() const { return (float)(m_PixelDimensions.x * m_SheetDimensions.y) /
                                                          (m_PixelDimensions.y * m_SheetDimensions.x); }

    /// @brief  Returns how many tiles/frames are in the texture
    /// @return ivec2: x = width ,  y = height
    __inline glm::ivec2 const& GetSheetDimensions() const { return m_SheetDimensions; }


    /// @brief  gets the Mesh this Sprite is using
    /// @return the Mesh this Sprite is using
    __inline Mesh const* GetMesh() const { return m_Mesh; }

    /// @brief  sets the Mesh this Sprite is using
    /// @param  mesh    the Mesh to set this Sprite to use
    __inline void SetMesh( Mesh const* mesh ) { m_Mesh = mesh; }


    /// @brief  gets the OpenGL ID of this Texture
    /// @return the OpenGL ID of this Texture
    unsigned GetTextureId() const;


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  the filepath to the texture
    std::string m_Filepath = "";

    /// @brief  How many tiles/frames are in the texture
    glm::ivec2 m_SheetDimensions = { 1, 1 };

    /// @brief  The pivot of the mesh this Texture uses
    glm::vec2 m_Pivot = { 0.5f, 0.5f };

    /// @brief  whether to use the aspect ratio of the texture instead of a square mesh
    bool m_UseAspectRatio = false;

    /// @brief  whether the texture will repeat when sampled out of bounds
    bool m_RepeatOutOfBounds = false;


    /// @brief  Width and height of the original image
    glm::ivec2 m_PixelDimensions = { 0, 0 };

    /// @brief  ID that texture is tracked with on GPU
    unsigned int m_TextureID = 0;

    /// @brief   Mesh to render texture onto
    Mesh const* m_Mesh = nullptr;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  Loads texture image from file (deletes old one if present)
    void loadImage();

    /// @brief  reloads this Texture's mesh
    void reloadMesh();

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the filepath of the Texture
    /// @param  stream  the data to read from
    void readFilepath( nlohmann::ordered_json const& data );

    /// @brief  reads the sheetDimension of the Texture
    /// @param  stream  the data to read from
    void readSheetDimensions( nlohmann::ordered_json const& data );

    /// @brief  reads the pivot of this Texture
    /// @param  data    the data to read from
    void readPivot( nlohmann::ordered_json const& data );

    /// @brief  reads whether to use the aspect ratio of the texture instead of a square mesh
    /// @param  data    the data to read from
    void readUseAspectRatio( nlohmann::ordered_json const& data );

    /// @brief  reads whether the texture will repeat when sampled out of bounds
    /// @param  data    the data to read from
    void readRepeatOutOfBounds( nlohmann::ordered_json const& data );

    /// @brief  the read methods for textures
    static ReadMethodMap< Texture > const s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets called after reading all arugments 
    void AfterLoad();

    /// @brief  gets the read method map
    /// @return the read method map
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief  Write all Texture data to a JSON file.
    /// @return The JSON file containing the Texture data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};
