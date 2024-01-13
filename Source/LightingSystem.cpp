/*****************************************************************//**
 * \file   LightingSystem.cpp
 * \brief  Updates all Lightings using compute shader, spawns new ones
 *         in accordance with emitters' timing.
 * 
 * \author Eli Tsereteli
 * \date   November 2023
 *********************************************************************/
#include "glew.h"
#include "LightingSystem.h"
#include "RenderSystem.h"   // adding sprite
#include "PlatformSystem.h" // screen size
#include "Mesh.h"           // rendering
#include "CameraSystem.h"   // world-to-screen matrix for frag shader uniforms
#include "Entity.h"         // parent position
#include "Transform.h"
#include "imgui.h"


//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------


/// @brief  Called when system starts:
void LightingSystem::OnInit()
{
    m_Sprite = new LightingSprite;
    m_Sprite->SetLayer(m_ShadowLayer);

    Renderer()->AddShader("lights", new Shader("Data/shaders/vshader.vert", "Data/shaders/lighting.frag"));
    Renderer()->AddShader("spotlight", new Shader("Data/shaders/vshader.vert", "Data/shaders/spotlight.frag"));
    Platform()->AddOnWindowResizeCallback( GetId(), std::bind(&LightingSystem::reallocTexArray, this) );
    //reallocTexArray();
    glGenFramebuffers(1, &m_FBO);
}


/// @brief  Renders spotlights in advance, and (later) launches compute shader to
///         to pre-calculate shadows.
void LightingSystem::OnUpdate(float dt)
{
    if (!m_Enabled || !Cameras()->GetActiveCamera())
        return;

    // get matrix inverse only when it's changed
    glm::mat4 s2w = Cameras()->GetMat_ScreenToWorld();
    if (s2w != m_S2W)
    {
        m_S2W = s2w;
        m_W2S = glm::inverse(s2w);
    }

    // render stuff to texture array
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // common to all light sources: shader, transform matrix, VAO
    Shader* spotShader = Renderer()->SetActiveShader("spotlight");
    glm::mat4 m( glm::mat2(2.0f) );
    glUniformMatrix4fv(spotShader->GetUniformID("mvp"), 1, false, &m[0][0]);
    glBindVertexArray(Renderer()->GetDefaultMesh()->GetVAO());

    // render spotlights. one per layer.
    for (size_t i=0; i<m_LightSources.size(); ++i)
    {
        Transform* pt = m_LightSources[i]->GetEntity()->GetComponent<Transform>();
        if (!pt)
            continue;

        // convert position and radius to screen dimensions, set them on the shader
        glm::vec4 pos = m_W2S * glm::vec4(pt->GetTranslation() + m_LightSources[i]->GetOffset(), 0, 1);
        glUniform2f(spotShader->GetUniformID("light_pos"), pos.x, pos.y);

        // render this light source to its appropriate layer
        glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_TextureArrayID, 0, (int)i);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    glBindVertexArray(0);  // unbind VAO

    // later: Bind compute shader, give it buffers to work with, dispatch
    //        (render results later in sprite's draw call?)

    glBindFramebuffer(GL_FRAMEBUFFER, 0); // reset to default FBO (backbuffer)

    (void)dt;
}


/// @brief  Called when system exits
void LightingSystem::OnExit()
{
    delete m_Sprite;
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteTextures(1, &m_TextureArrayID);
}


/// @brief  Called when entering a scene
void LightingSystem::OnSceneInit() 
{
    Renderer()->AddSprite(m_Sprite);
}

/// @brief  Called when exiting a scene
void LightingSystem::OnSceneExit() { m_LightSources.clear(); }

void LightingSystem::DebugWindow()
{
    bool showWindow = GetDebugEnabled();

    ImGui::Begin("Lighting System", &showWindow);
    if ( ImGui::Checkbox("Enable", &m_Enabled) )
        m_Sprite->SetOpacity( m_Enabled ? 1.0f : 0.0f );
    ImGui::End();

    SetDebugEnable(showWindow);
}


//-----------------------------------------------------------------------------
//              Public methods
//-----------------------------------------------------------------------------

/// @brief        Adds a new light source to keep track of.
/// @param light  Light component
/// @return       Index of the added light
int LightingSystem::AddLightSource(Light* light)
{
    m_LightSources.push_back(light);
    reallocTexArray();

    return (int)m_LightSources.size() - 1;
}


void LightingSystem::RemoveLightSource(int index)
{
    if (index < (int)m_LightSources.size())
    m_LightSources.erase(m_LightSources.begin() + index);
}



//-----------------------------------------------------------------------------
//              Helpers
//-----------------------------------------------------------------------------

/// @brief  Reallocates texture array - for when the amount of lights or the
///         screen resolution changes. (This is why Ligthing System keeps track of it,
///         rather than the sprite)
void LightingSystem::reallocTexArray()
{
    // replace old texture array, if present
    if (m_TextureArrayID)
        glDeleteTextures(1, &m_TextureArrayID);

    // create new one, match texture resolution to screen size
    glGenTextures(1, &m_TextureArrayID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_TextureArrayID);
    glm::ivec2 dims = Platform()->GetWindowDimensions();
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, dims.x, dims.y, (GLsizei)m_LightSources.size());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}



//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

void LightingSystem::readEnabled(nlohmann::ordered_json const& data)
{
    m_Enabled = Stream::Read< bool >(data);
}

/// @brief       Reads the rendering layer of shadows
/// @param data  json data
void LightingSystem::readLayer(nlohmann::ordered_json const& data)
{
    m_ShadowLayer = Stream::Read< int >(data);
}


/// @brief map of the LightingSystem read methods
ReadMethodMap< LightingSystem > const LightingSystem::s_ReadMethods = {
    { "Enabled", &readEnabled },
    { "Layer",   &readLayer }
};


/// @brief   writes this System config
/// @return  the writting System config
nlohmann::ordered_json LightingSystem::Write() const
{
    nlohmann::ordered_json json;

    json["Enabled"] = Stream::Write< bool >(m_Enabled);

    return json;
}


//-----------------------------------------------------------------------------
//              Singleton stuff
//-----------------------------------------------------------------------------

/// @brief    Private constructor
LightingSystem::LightingSystem() :
    System( "LightingSystem" )
{}

/// @brief    The singleton instance of LightingSystem
LightingSystem * LightingSystem::s_Instance = nullptr;

/// @return   The singleton instance
LightingSystem * LightingSystem::GetInstance()
{
    if ( s_Instance == nullptr )
    {
        s_Instance = new LightingSystem();
    }
    return s_Instance;
}




// ========================================================================= //
//                    Lighting Sprite implementation                         //
// ========================================================================= //
// Lighting system just uses 1 sprite to render all lights. (Or rather, shades)
// It's just a rectangle stretched over the entire screen. The lighting system
// has already rendered all individual light sources to the texture array.
// They are combined together by this sprite's shader.

/// @brief   Default constructor
LightingSystem::LightingSprite::LightingSprite() : 
    Sprite(typeid(LightingSystem::LightingSprite)) {}

/// @brief   Called by RenderSystem in accordance with its layer.
void LightingSystem::LightingSprite::Draw()
{
    if (!Lights()->GetLightingEnabled())
        return;

    Shader* shdr = Renderer()->SetActiveShader("lights");

    // uniforms
    glm::mat4 m( glm::mat2(2.0f) );
    glUniformMatrix4fv(shdr->GetUniformID("mvp"), 1, false, &m[0][0]);
    glUniform1f(shdr->GetUniformID("opacity"), m_Opacity);
    glUniform1i(shdr->GetUniformID("light_count"), Lights()->GetLightCount());
    glUniform2f(shdr->GetUniformID("UV_offset"), 0.0f, 0.0f);

    // draw the combination of textures in the array
    glBindVertexArray(Renderer()->GetDefaultMesh()->GetVAO());
    glBindTexture(GL_TEXTURE_2D_ARRAY, Lights()->GetTexArrayID());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// ========================================================================== //

