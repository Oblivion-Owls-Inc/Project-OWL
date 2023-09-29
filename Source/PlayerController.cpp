#include "PlayerController.h" 
#include "BehaviorSystem.h"
#include "InputSystem.h"
#include "RigidBody.h"
#include "Transform.h"

static InputSystem * input = InputSystem::GetInstance();

PlayerController::PlayerController(): Behavior(typeid(PlayerController))
{
	BehaviorSystem<PlayerController>::GetInstance()->AddBehavior(this);
}

PlayerController::PlayerController(const PlayerController& other)
	             : Behavior(typeid(PlayerController))
{
	BehaviorSystem< PlayerController >::GetInstance()->RemoveBehavior(this);
}

PlayerController::~PlayerController()
{
}

Component* PlayerController::Clone() const
{
	return nullptr;
}

void PlayerController::OnUpdate(float dt)
{
    if (MoveRight())
    {
        Transform* transform = Parent()->GetComponent<Transform>();
        vec3 POS = *transform->GetTranslation();

        // Calculate the desired velocity increment.
        vec3 velocityIncrement = vec3(5.0f * acceleration * dt, 0.0f, 0.0f);

        // Apply acceleration, but limit the speed.
        if (glm::length(velocityIncrement) > maxSpeed)
        {
            velocityIncrement = glm::normalize(velocityIncrement) * maxSpeed;
        }

        // Update the position gradually.
        transform->SetTranslation(POS + velocityIncrement);
    }
    else if (MoveLeft())
    {
        Transform* transform = Parent()->GetComponent<Transform>();
        vec3 POS = *transform->GetTranslation();

        // Calculate the desired velocity increment.
        vec3 velocityIncrement = vec3(-5.0f * acceleration * dt, 0.0f, 0.0f);

        // Apply acceleration, but limit the speed.
        if (glm::length(velocityIncrement) > maxSpeed)
        {
            velocityIncrement = glm::normalize(velocityIncrement) * maxSpeed;
        }

        // Update the position gradually.
        transform->SetTranslation(POS + velocityIncrement);
    }
    if (Jump())
    {
   		Transform* transform = Parent()->GetComponent<Transform>();
		vec3 POS = *transform->GetTranslation();

		// Calculate the desired velocity increment.
		vec3 velocityIncrement = vec3(0.0f, 5.0f * acceleration * dt, 0.0f);

		// Apply acceleration, but limit the speed.
		if (glm::length(velocityIncrement) > maxSpeed)
		{
			velocityIncrement = glm::normalize(velocityIncrement) * maxSpeed;
		}

		// Update the position gradually.
		transform->SetTranslation(POS + velocityIncrement);
    }
}

bool PlayerController::MoveRight()
{
	if (input->GetKeyDown(GLFW_KEY_D))
	{
		return true;
	}
	{
		return false;
	}
}

bool PlayerController::MoveLeft()
{
	if (input->GetKeyDown(GLFW_KEY_A))
	{
		return true;
	}
	{
		return false;
	}
}

bool PlayerController::Jump()
{
	if (input->GetKeyDown(GLFW_KEY_W))
	{
		return true;
	}
	{
		return false;
	}
}


/// @brief the map of read methods for this Component
ReadMethodMap< PlayerController > const PlayerController::readMethods = {};

ReadMethodMap<Component> const& PlayerController::GetReadMethods() const
{
	return (std::map< std::string, ReadMethod< Component > > const&)readMethods;
}
