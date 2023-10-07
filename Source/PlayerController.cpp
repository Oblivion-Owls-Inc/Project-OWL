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

void PlayerController::OnUpdate(float dt)
{

    Transform* m_Transform = GetParent()->GetComponent<Transform>();

	if (!m_Transform)
		return;

    if (MoveRight())
    {
        glm::vec3 POS = m_Transform->GetTranslation();

        // Calculate the desired velocity increment.
        glm::vec3 velocityIncrement = glm::vec3(5.0f * acceleration * dt, 0.0f, 0.0f);

        // Apply acceleration, but limit the speed.
        if (glm::length(velocityIncrement) > maxSpeed)
        {
            velocityIncrement = glm::normalize(velocityIncrement) * maxSpeed;
        }

        // Update the position gradually.
        m_Transform->SetTranslation(POS + velocityIncrement);
    }
     if (MoveLeft())
    {
        Transform* m_Transform = GetParent()->GetComponent<Transform>();

        glm::vec3 POS = m_Transform->GetTranslation();

        // Calculate the desired velocity increment.
        glm::vec3 velocityIncrement = glm::vec3(-5.0f * acceleration * dt, 0.0f, 0.0f);

        // Apply acceleration, but limit the speed.
        if (glm::length(velocityIncrement) > maxSpeed)
        {
            velocityIncrement = glm::normalize(velocityIncrement) * maxSpeed;
        }

        // Update the position gradually.
        m_Transform->SetTranslation(POS + velocityIncrement);
    }
    if (Jump())
    {
   		Transform* m_Transform = GetParent()->GetComponent<Transform>();
		glm::vec3 POS = m_Transform->GetTranslation();

		// Calculate the desired velocity increment.
		glm::vec3 velocityIncrement = glm::vec3(0.0f, 5.0f * acceleration * dt, 0.0f);

		// Apply acceleration, but limit the speed.
		if (glm::length(velocityIncrement) > maxSpeed)
		{
			velocityIncrement = glm::normalize(velocityIncrement) * maxSpeed;
		}

		// Update the position gradually.
		m_Transform->SetTranslation(POS + velocityIncrement);
    }
	if (MoveDown())
	{
		Transform* m_Transform = GetParent()->GetComponent<Transform>();
		glm::vec3 POS = m_Transform->GetTranslation();

		// Calculate the desired velocity increment.
		glm::vec3 velocityIncrement = glm::vec3(0.0f, -5.0f * acceleration * dt, 0.0f);

		// Apply acceleration, but limit the speed.
		if (glm::length(velocityIncrement) > maxSpeed)
		{
			velocityIncrement = glm::normalize(velocityIncrement) * maxSpeed;
		}

		// Update the position gradually.
		m_Transform->SetTranslation(POS + velocityIncrement);
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

bool PlayerController::MoveDown()
{
	if (input->GetKeyDown(GLFW_KEY_S))
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
	return (ReadMethodMap< Component > const&)readMethods;
}
