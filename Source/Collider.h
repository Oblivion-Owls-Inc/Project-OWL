///*************************************************************************/
///@file  Collider.cpp
/// @Author Jax Clayton (jax.clayton@digipen.edu)
/// @date   9/5/2021
/// @brief  This is the Collider class implementation file
///*************************************************************************/

///*************************************************************************/
/// Includes
///*************************************************************************/
#include "Component.h"

///*************************************************************************/
/// @Class Collider
/// @brief This class contains the Collider component which is used for handling
///			collisions
///************************************************************************/

class Collider : public Component
{
	protected:
		Collider();

	public:
		Collider(const Collider& other);
		~Collider() = default;
		virtual Component* Clone() const override;
		virtual void OnUpdate(float dt);
		void Check(const Collider* other);
		virtual bool IsColliding(const Collider* other);
		void SetCollider(Collider* collider);
		Collider* GetCollider();

	private:
		typedef enum ColliderType
		{
			ColliderTypeNone,
			ColliderTypeCircle,
			ColliderTypeLine,

		} ColliderType;

		ColliderType type;
};
