#include "PlayerController.h" 
#include "BehaviorSystem.h"
#include "InputSystem.h"
#include "RigidBody.h"
#include "Transform.h"

static InputSystem * input = InputSystem::GetInstance();

PlayerController::PlayerController(): Behavior(typeid(PlayerController))
{
}

PlayerController::PlayerController(const PlayerController& other)
	             : Behavior(typeid(PlayerController))
{
}

PlayerController::~PlayerController()
{
}

Component* PlayerController::Clone() const
{
	return nullptr;
}

void PlayerController::OnInit()
{
	BehaviorSystem<PlayerController>::GetInstance()->AddBehavior(this);
}

void PlayerController::OnExit()
{
	BehaviorSystem< PlayerController >::GetInstance()->RemoveBehavior(this);
}

void PlayerController::OnFixedUpdate()
{

    glm::vec2 direction = { 0.0f, 0.0f };

    if (MoveRight())
    {
        direction += glm::vec2( 1.0f, 0.0f );
    }
    if (MoveLeft())
    {
        direction += glm::vec2( -1.0f, 0.0f );
    }
    if (Jump())
    {
        direction += glm::vec2( 0.0f, 1.0f );
    }
	if (MoveDown())
	{
        direction += glm::vec2( 0.0f, -1.0f );
	}

    if ( direction != glm::vec2( 0 ) )
    {
        direction = glm::normalize( direction );
    }

    GetParent()->GetComponent<RigidBody>()->SetVelocity( direction * maxSpeed );

}


bool PlayerController::MoveRight()
{
	return input->GetKeyDown(GLFW_KEY_D);
}

bool PlayerController::MoveLeft()
{
	return input->GetKeyDown(GLFW_KEY_A);
}

bool PlayerController::Jump()
{
	return input->GetKeyDown(GLFW_KEY_W);
}

bool PlayerController::MoveDown()
{
	return input->GetKeyDown(GLFW_KEY_S);
}
