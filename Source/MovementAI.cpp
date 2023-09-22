#include "MovementAI.h"
#include "BehaviorSystem.h"
#include "Entity.h"
#include "DebugSystem.h"

MovementAI::MovementAI(): Behavior(typeid(MovementAI))
{
    BehaviorSystem< MovementAI >::GetInstance()->AddBehavior(this);
}

MovementAI::~MovementAI()
{
    BehaviorSystem< MovementAI >::GetInstance()->RemoveBehavior(this);
}

Component* MovementAI::Clone() const
{
	return nullptr;
}


void MovementAI::OnUpdate(float dt)
{
    Transform* transform = (Transform*)Parent()->HasComponent(typeid(Transform));
	static float elapsedTime = 0.0f;
	static bool moveRight = true;

	static float moveSpeed = 100.0f; // Adjust the speed as needed
    // Create a window for the menu
    ImGui::Begin("Settings");
    ImGui::SliderFloat("Movement Speed", &moveSpeed, 0.0f, 500.0f); // Adjust the range as needed
    // End the menu window
    ImGui::End();

    // Update the elapsed time
    elapsedTime += dt;

    // Check if it's time to change direction
    if (elapsedTime >= 2.0f) // Change direction every 2 seconds (adjust as needed)
    {
        moveRight = !moveRight;
        elapsedTime = 0.0f;
    }

    // Calculate movement direction based on the current direction
    float movementDirection = moveRight ? 1.0f : -1.0f;

    glm::vec3 currentTranslation = *transform->GetTranslation();
    glm::vec3 newTranslation = currentTranslation + glm::vec3(movementDirection * moveSpeed * dt, 0.0f, 0.0f);

    transform->SetTranslation(newTranslation);
}


void MovementAI::MovementAIUpdateRotation(float dt)
{
}

void MovementAI::MovementAIUpdateVelocity(float dt)
{
}

void MovementAI::MovementAIUpdateWeapon(float dt)
{
}


void MovementAI::MovementAISpiral(float dt)
{
}

/// @brief the map of read methods for this Component
ReadMethodMap< MovementAI > const MovementAI::readMethods = {};

ReadMethodMap<Component> const& MovementAI::GetReadMethods()
{
    return (std::map< std::string, ReadMethod< Component > > const&)readMethods;
}

void MovementAI::MovementAISpawnBullet()
{
}
/// <summary>
/// UNUSED
/// </summary>
void MovementAI::OnFixedUpdate()
{
}