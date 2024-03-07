/// @file     Texture.cpp
/// @author   Eli Tsereteli (ilya.tsereteli@digipen.edu)
/// 
/// @brief    Wrapper/manager for OpenGL texture.



#include "pch.h" // precompiled header has to be included first
#include "Texture.h"
#include "RenderSystem.h"   // default mesh
#include "Inspection.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"      // loading images

//-----------------------------------------------------------------------------
// public: constructors/destructors
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    Texture::Texture() {}

    /// @brief                  Convenience constructor: loads texture image from 
    ///                         file upon initialization
    /// @param  filepath        File to load
    /// @param  sheetDimensions x=columns, y=rows (of the spritesheet)
    /// @param  pivot           the pivot point of this Texture
    Texture::Texture( std::string const& filepath, glm::ivec2 const& sheetDimensions, glm::vec2 const& pivot) :
        m_Filepath( filepath ),
        m_SheetDimensions( sheetDimensions ),
        m_Pivot( pivot ),
        m_TextureID( 0 )
    {
        loadImage();
    }


    /// @brief      Destructor: deletes texture data from GPU
    Texture::~Texture()
    { 
        if (m_TextureID)
            glDeleteTextures(1, &m_TextureID);

        if (m_Mesh != Renderer()->GetDefaultMesh())
            delete m_Mesh;
    }



//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  calculates the uv offset for a given frame offset
    /// @param  frameIndex  the index of the frame to get the UV offset of
    /// @return the UV offset of the frame index
    glm::vec2 Texture::GetUvOffset( int frameIndex ) const
    {
        if (m_Mesh == nullptr)
        {
            return glm::vec2(0.0f);
        }
        int column = frameIndex % m_SheetDimensions.x;
        int row = frameIndex / m_SheetDimensions.x;
        return m_Mesh->GetUVsize() * glm::vec2( column, row );
    }


    /// @brief       Sets this texture as active, so it can be used by texture shader
    /// @param slot  (optional) texture slot - for drawing multiple at once ig?
    void Texture::Bind(unsigned int slot) const
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
    }


    /// @brief Used by the Debug to display information about this Texture
    void Texture::Inspect()
    {
        if ( Inspection::SelectFileFromDirectory( "Filepath", &m_Filepath, "Data/Textures" ) )
        {
            loadImage();
        }

        if ( ImGui::DragInt2( "Sheet Dimensions", &m_SheetDimensions[0], 0.02f, 1, INT_MAX ) )
        {
            reloadMesh();
        }

        if ( ImGui::DragFloat2( "Pivot", &m_Pivot[0], 0.01f, 0.0f, 1.0f ) )
        {
            reloadMesh();
        }

        if ( ImGui::Checkbox( "Use Aspect Ratio", &m_UseAspectRatio ) )
        {
            reloadMesh();
        }

        if ( ImGui::Button( "Reload Texture" ) )
        {
            loadImage();
        }

        for ( int i = 0; i < m_SheetDimensions.x * m_SheetDimensions.y; ++i )
        {
            if ( i % m_SheetDimensions.x != 0 )
            {
                ImGui::SameLine();
            }
            DisplayInInspector( i );
        }
    }


    /// @brief  displays this Texture in ImGui
    /// @param  scale       the scale to display this image at
    /// @param  frameIndex  the index of the frame of this texture to display
    /// @param  tintColor   the tint color of the displayed button
    /// @param  borderColor the border color of the displayed button
    void Texture::DisplayInInspector( int frameIndex, float scale, glm::vec4 const& tintColor, glm::vec4 const& borderColor ) const
    {
        if ( m_TextureID == 0 || m_Mesh == nullptr )
        {
            return;
        }

        glm::vec2 uvMin = GetUvOffset( frameIndex );
        glm::vec2 uvMax = uvMin + m_Mesh->GetUVsize();

        #pragma warning ( push )
        #pragma warning ( disable: 4312 )
        ImGui::Image(
            (ImTextureID)m_TextureID,
            ImVec2( scale, scale / GetAspectRatio() ),
            ImVec2( uvMin.x, uvMin.y ),
            ImVec2( uvMax.x, uvMax.y ),
            *reinterpret_cast< ImVec4 const* >( &tintColor ),
            *reinterpret_cast< ImVec4 const* >( &borderColor )
        );
        #pragma warning( pop )
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the OpenGL ID of this Texture
    /// @return the OpenGL ID of this Texture
    unsigned Texture::GetTextureId() const
    {
        return m_TextureID;
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


/// @brief  reads the filepath of the Texture
/// @param  data    the data to read from
void Texture::readFilepath( nlohmann::ordered_json const& data )
{
    m_Filepath = Stream::Read<std::string>( data );
}


/// @brief  reads the sheetDimension of the Texture
/// @param  data    the data to read from
void Texture::readSheetDimensions( nlohmann::ordered_json const& data )
{
    m_SheetDimensions = Stream::Read< 2, int >( data );
}

/// @brief  reads the pivot of this Texture
/// @param  data    the data to read from
void Texture::readPivot( nlohmann::ordered_json const& data )
{
    m_Pivot = Stream::Read< 2, float >( data );
}

/// @brief  reads whether to use the aspect ratio of the texture instead of a square mesh
/// @param  data    the data to read from
void Texture::readUseAspectRatio( nlohmann::ordered_json const& data )
{
    Stream::Read( m_UseAspectRatio, data );
}


/// @brief  gets called after reading all arugments 
void Texture::AfterLoad()
{
    if ( m_Filepath.empty() == false )
    {
        loadImage();
    }
}

/// @brief  Write all Texture data to a JSON file.
/// @return The JSON file containing the Texture data.
nlohmann::ordered_json Texture::Write() const
{
    nlohmann::ordered_json data;

    data["Filepath"] = m_Filepath;
    data["SheetDimensions"] = Stream::Write(m_SheetDimensions);
    data["Pivot"] = Stream::Write( m_Pivot );
    data["UseAspectRatio"] = Stream::Write( m_UseAspectRatio );

    return data;
}



/// @brief  the read methods for textures
ReadMethodMap< Texture > const Texture::s_ReadMethods = {
    { "Filepath"       , &readFilepath        },
    { "SheetDimensions", &readSheetDimensions },
    { "Pivot"          , &readPivot           },
    { "UseAspectRatio" , &readUseAspectRatio  }
};



//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  Loads texture image from file (deletes old one if present)
void Texture::loadImage()
{
    if (m_TextureID)            // reloading with new data?
    {
        glDeleteTextures(1, &m_TextureID);
        m_TextureID = 0;
        if (m_Mesh != Renderer()->GetDefaultMesh())
            delete m_Mesh;
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f )[ 0 ] );

        // attach mesh
        if ((m_UseAspectRatio == false || GetAspectRatio() == 1.0f) && m_SheetDimensions == glm::ivec2( 1 ) && m_Pivot == glm::vec2( 0.5f ) )
            m_Mesh = Renderer()->GetDefaultMesh();
        else
            m_Mesh = new Mesh( m_UseAspectRatio ? glm::vec2( GetAspectRatio(), 1.0f ) : glm::vec2( 1.0f ), m_SheetDimensions, m_Pivot);
    }
    else
    {
        Debug() << "ERROR: could not load texture " << m_Filepath << std::endl;
    }
}


    /// @brief  reloads this Texture's mesh
    void Texture::reloadMesh()
    {
        if ( m_Mesh != Renderer()->GetDefaultMesh() )
        {
            delete m_Mesh;
        }

        if ( (m_UseAspectRatio == false || GetAspectRatio() == 1.0f) && m_SheetDimensions == glm::ivec2( 1 ) && m_Pivot == glm::vec2( 0.5f ) )
        {
            m_Mesh = Renderer()->GetDefaultMesh();
        }
        else
        {
            m_Mesh = new Mesh( m_UseAspectRatio ? glm::vec2( GetAspectRatio(), 1.0f ) : glm::vec2( 1.0f ), m_SheetDimensions, m_Pivot );
        }
    }

//-----------------------------------------------------------------------------
