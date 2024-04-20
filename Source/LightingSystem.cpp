/*****************************************************************//**
 * \file       LightingSystem.cpp
 * \brief      Renders lights in the scene.
 * 
 * \author     Eli Tsereteli
 * \date       Jan 2024
 * \copyright  Copyright (c) 2024 Digipen Instutute of Technology
 *********************************************************************/

#include "pch.h" // precompiled header has to be included first
#include "LightingSystem.h"
#include "RenderSystem.h"   // sprite implementation
#include "PlatformSystem.h" // screen size
#include "Mesh.h"           // rendering
#include "CameraSystem.h"   // world-to-screen matrix for frag shader uniforms
#include "Entity.h"         // parent position
#include "Transform.h"      // parent position

#include "InputSystem.h"



//-----------------------------------------------------------------------------
//              Virtual overrides
//-----------------------------------------------------------------------------


/// @brief  Called when system starts:
void LightingSystem::OnInit()
{
    m_Sprite = new LightingSprite;
    m_Sprite->SetLayer(0);  // scene transition will take care of this

    Renderer()->AddShader("lights", new Shader("Data/shaders/vshader.vert", "Data/shaders/lighting.frag"));
    Renderer()->AddShader("spotlight", new Shader("Data/shaders/vshader.vert", "Data/shaders/spotlight.frag"));
    Platform()->AddOnWindowResizeCallback( GetId(), std::bind(&LightingSystem::reallocTexArray, this) );
    glGenFramebuffers(1, &m_FBO);

    // uniform blocks
    //glGenBuffers(1, &m_UBOrad);
    //glGenBuffers(1, &m_UBOstr);         // TODO: do I need to bind them?
    //glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_UBOrad);
    //glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_UBOstr);

    glGenBuffers(1, &m_UBOpos);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 20, m_UBOpos);
    glGenBuffers(1, &m_UBOrad);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 25, m_UBOrad);
    glGenBuffers(1, &m_UBOstr);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 30, m_UBOstr);


    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


/// @brief  Renders spotlights in advance, and (later) launches compute shader to
///         to pre-calculate shadows.
void LightingSystem::OnUpdate(float dt)
{
    if (!m_Enabled || !GetComponents().size())
        return;
    
    // for inverted screen y-coordinates
    float scrheight = (float)Platform()->GetWindowDimensions().y;

    // get matrix inverse only when it's changed
    glm::mat4 s2w = Cameras()->GetMat_ScreenToWorld();
    if (s2w != m_S2W)
    {
        m_S2W = s2w;
        m_W2S = glm::inverse(s2w);
    }

    // put individual light source stats into vectors
    size_t size = GetComponents().size();
    m_Positions.clear();
    m_Radii.clear();
    m_Strengths.clear();
    for (Light* thislight : GetComponents())
    {
        // light position
        glm::vec4 pos = glm::vec4( thislight->GetOffset(), 0, 1 );
        Transform* pt = thislight->GetEntity()->GetComponent<Transform>();
        if (pt)
            pos += glm::vec4(pt->GetTranslation(), 0, 0);

        // convert it to screen coords
        glm::vec4 scrpos = m_W2S * pos;
        scrpos.y = scrheight - scrpos.y; // TODO: does W2S need fixing, or am I doing something wrong here?

        m_Positions.push_back(scrpos);
        m_Radii.push_back(thislight->GetRadius() * m_W2S[0][0]);
        m_Strengths.push_back(thislight->GetStrength());
    }

    (void)dt;
}

void LightingSystem::DrawLights()
{
    if (!GetComponents().size() || !m_Enabled)
        return;

    Shader* spotShader = Renderer()->SetActiveShader("spotlight");

    // common to all light sources: transform matrix (covers full screen), ambient light?
    glm::mat4 m(glm::mat2(2.0f));
    glUniformMatrix4fv(spotShader->GetUniformID("mvp"), 1, false, &m[0][0]);

    // light count
    //spotShader->GetUniformID("ambient");
    glUniform1i(spotShader->GetUniformID("light_count"), (int)GetComponents().size());

    // send the vectors to GPU
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOpos);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * (int)m_Positions.size(),
        &m_Positions[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOrad);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * (int)m_Radii.size(),
        &m_Radii[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, m_UBOstr);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * (int)m_Strengths.size(),
        &m_Strengths[0], GL_DYNAMIC_DRAW);
    

    // draw it all
    glBindVertexArray(Renderer()->GetDefaultMesh()->GetVAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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
void LightingSystem::OnSceneExit() 
{
    Renderer()->RemoveSprite(m_Sprite);
}

void LightingSystem::DebugWindow()
{
    bool showWindow = GetDebugEnabled();

    ImGui::Begin("Lighting System", &showWindow);

    if ( ImGui::Checkbox("Enable", &m_Enabled) )
        m_Sprite->SetOpacity( m_Enabled ? 1.0f : 0.0f );

    ImGui::Text("Active Light Count:  %i", (int)GetComponents().size());
    ImGui::Spacing();
    ImGui::TextWrapped("Adjust lighting layer per-scene in the SceneTransition entity.");

    ImGui::End();

    SetDebugEnable(showWindow);
}



/// @brief  Reallocates texture array - for when the amount of lights or the
///         screen resolution changes. (This is why Ligthing System keeps track of it,
///         rather than the sprite)
void LightingSystem::reallocTexArray()
{
    if (!GetComponents().size())
        return;

    // replace old texture array, if present
    if (m_TextureArrayID)
        glDeleteTextures(1, &m_TextureArrayID);

    // create new one, match texture resolution to screen size
    glGenTextures(1, &m_TextureArrayID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_TextureArrayID);
    glm::ivec2 dims = Platform()->GetWindowDimensions();
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, dims.x, dims.y, (GLsizei)GetComponents().size());

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    m_PrevSize = GetComponents().size();
}



//-----------------------------------------------------------------------------
//              Reading / Writing
//-----------------------------------------------------------------------------

/// @brief       Reads whether lighting system should be enabled on launch
/// @param data  json data
void LightingSystem::readEnabled(nlohmann::ordered_json const& data)
{
    m_Enabled = Stream::Read< bool >(data);
}


/// @brief map of the LightingSystem read methods
ReadMethodMap< LightingSystem > const LightingSystem::s_ReadMethods = {
    { "Enabled", &readEnabled }
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
    ComponentSystem( "LightingSystem" )
{}

/// @return   The singleton instance
LightingSystem * LightingSystem::GetInstance()
{
   static std::unique_ptr< LightingSystem > s_Instance = nullptr;

    if ( s_Instance == nullptr )
    {
        s_Instance.reset( new LightingSystem());
    }
    return s_Instance.get();
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
    Lights()->DrawLights();
}

// ========================================================================== //

