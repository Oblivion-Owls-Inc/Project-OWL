/*****************************************************************//**
 * \file   Emitter.cpp
 * \brief  Controls timing of particle emissions, and their init data.
 * 
 * \author Eli
 * \date   October 2023
 *********************************************************************/
#include "Emitter.h"
#include "imgui.h"
#include "BehaviorSystem.h"


/// /// @brief	    Defualt constructor
Emitter::Emitter() : Behavior( typeid( Emitter ) ) {}


/// @return     A copy of this emitter
Component* Emitter::Clone() const { return new Emitter(*this); }


/// @brief  Initialization: emitter adds itself to behavior system
void Emitter::OnInit()
{
    Behaviors<Behavior>()->AddBehavior(this); // TODO: add to common behavior system instead

    m_Init = { 5, {}, 1.57f, 2.5f, 0.2f, 0.0f,
                  {}, 2.0f,  2.5f, 0.0f, 1.0f };
    m_Init.fade_startTime = 2.0f;
    
    m_PPS = 500.0f;
}


/// @brief  Controls timing of continuous emissions
void Emitter::OnUpdate(float dt)
{
    if (!m_Continuous)
        return;

    m_Emission += m_PPS * dt;

    // if there's delay, only emit when the timer reaches it
    if (m_Delay)
    {
        m_DelayTimer += dt;

        if (m_DelayTimer >= m_Delay)
            m_DelayTimer -= m_Delay;
        else
            return;
    }

    // emit the integer part, keep the fractional part
    int count = (int)m_Emission;
    m_Emission -= glm::floor(m_Emission);

    if (count && count <= Particles()->GetMaxParticleCount())  // TODO: personal max count
    {
        m_Init.amount = count;
        Particles()->SpawnParticles(m_Init);
    }
}


/// @brief  Exit: emitter removes itself from behavior system
void Emitter::OnExit()
{
    Behaviors<Emitter>()->RemoveBehavior(this);
}

/// @brief  Imgui
void Emitter::Inspector()
{
    ImGui::Checkbox("Continuous", &m_Continuous);
    if (!m_Continuous)
    {
        if (ImGui::Button("Emit"))
            Particles()->SpawnParticles(m_Init);

        int amt = (int)m_Init.amount;                   // TODO: personal max count
        if ( ImGui::SliderInt("Amount", &amt, 1, Particles()->GetMaxParticleCount()) )
            m_Init.amount = amt;
    }
    else
    {
        ImGui::Text("Particles per sec");
        ImGui::SliderFloat("###PPS", &m_PPS, 1.0f, 2000.0f);
        ImGui::SliderFloat("Delay", &m_Delay, 0.0f, 2.0f);
    }
    ImGui::Text("Lifetime");
    ImGui::SliderFloat("###lifetime", &m_Init.fade_startTime, 0.1f, 20.0f);
    ImGui::Text("Position");
    ImGui::SliderFloat2("###pos", &m_Init.position.x, -7.0f, 7.0f);
    ImGui::SliderFloat2("spread ###posspread", &m_Init.pos_spread.x, 0.0f, 7.0f);
    ImGui::Text("Direction");
    ImGui::SliderFloat("###Direction", &m_Init.direction, 0.0f, 6.28f);
    ImGui::SliderFloat("spread ###dirspread", &m_Init.dir_spread, 0.0f, 3.14f);
    ImGui::Text("Speed");
    ImGui::SliderFloat("###Speed", &m_Init.speed, 0.0f, 10.0f);
    ImGui::SliderFloat("spread ###spdspread", &m_Init.speed_spread, 0.0f, 10.0f);
    ImGui::Text("Size");
    ImGui::SliderFloat("###Size", &m_Init.size, 0.0f, 2.0f);
    ImGui::SliderFloat("spread ###szspread", &m_Init.size_spread, 0.0f, m_Init.size);
}


/// @brief  map of read methods
ReadMethodMap< Emitter > const Emitter::s_ReadMethods = {
//    {"Thing" , &function  }
};


