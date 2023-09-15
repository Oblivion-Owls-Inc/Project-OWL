/// @file     SandboxSystem.cpp
/// @author   Anyone
/// 
/// @brief    Sandbox scene for experimenting.
#include "SandboxSystem.h"
#include "SceneSystem.h"
#include "imgui.h"
// ===========================================


#include "Entity.h"
#include "Sprite.h"
#include "Transform.h"

#include "EntitySystem.h"

// for archer
static float frametime = 0.0f, speed = 13.0f, opacity = 1.0f;
static int layer = 2;


static void SceneInit()
{
    // give this entity a Sprite and a Transform.
    Sprite* s = new Sprite("Elementals_leaf_ranger_288x128_SpriteSheet.png", 22, 17);
    Transform* t = new Transform, * t2 = new Transform;
    t->setScale({ 800, -800 * s->getHeightMultiplier(), 0 }); // for screen space, y is flipped, so flip the image.
    t->setTranslation({ 100,400,0 });

    Entity* testEnt = new Entity();
    testEnt->SetName( "testEnt" );
    testEnt->Add( s );
    testEnt->Add( t );
    Entities()->AddEntity( testEnt );

    // this one's just a square
    t2->setScale({ 100,100,0 });
    t2->setTranslation({ 100,500,0 });

    Entity* square = new Entity();
    square->SetName( "square" );
    square->Add( new Sprite( true ) );
    square->Add( t2 );
    Entities()->AddEntity( square );



}




static void SceneUpdate(float dt)
{
    // ImGui constrols
    ImGui::Begin("Sandbox Scene: archer");
    ImGui::SliderFloat("anim speed", &speed, 0.0f, 30.0f);
    ImGui::SliderInt("layer", &layer, 0, 4);
    ImGui::SliderFloat("opacity", &opacity, 0.0f, 1.0f);
    ImGui::End();

    // advance frames
    frametime += dt * speed;
    if (frametime >= 12.0f)
        frametime = 0.0f;

    // set sprite properties
    Entity* testEnt = Entities()->GetEntity( "testEnt" );

    Sprite* s = (Sprite*)testEnt->GetComponent< Sprite >();
    s->setFrame( (int)frametime );
    s->setLayer( layer );
    s->setOpacity( opacity );

}




static void SceneFixedUpdate()
{

}


static void SceneExit()
{

}







// ------------------- System needs ----------------- //

/// @brief      Calls SceneInit or SceneUpdate when current scene is Sandbox
void SandboxSystem::OnUpdate(float dt)
{
    if (_active)
    {
        if (SceneSystem::getInstance()->getSceneName() == "Sandbox")
            SceneUpdate(dt);
        else
        {
            _active = false;
            SceneExit();
        }
    }
    else if (SceneSystem::getInstance()->getSceneName() == "Sandbox")
    {
        _active = true;
        SceneInit();
    }
}

/// @brief      Calls SceneExit if active
void SandboxSystem::OnExit()
{
    if (_active)
        SceneExit();
}

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief      Calls static version if active
void SandboxSystem::OnFixedUpdate()
{
    if (_active)
        SceneFixedUpdate();
}

/// @brief    The singleton instance of SandboxSystem 
SandboxSystem* SandboxSystem::instance = nullptr;

/// @brief    Gets the instance of SandboxSystem
/// @return   SandboxSystem pointer: new or existing instance of this system
SandboxSystem* SandboxSystem::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SandboxSystem();
    }
    return instance;
}

//-----------------------------------------------------------------------------
