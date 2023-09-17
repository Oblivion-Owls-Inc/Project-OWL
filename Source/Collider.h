/// @file Collider.h
/// @author Jax Clayton (jax.clayton)
/// 
/// 
///
///
#include "Component.h"
#include "rapidjson.h"

using namespace rapidjson;

class Collider : public Component
{
	protected:
		Collider() : Component(typeid(Collider)), type() {}

	public:
		Collider(const Collider& other);
		~Collider();
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
