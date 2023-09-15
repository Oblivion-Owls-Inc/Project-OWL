/// @file Collider.h
/// @author Jax Clayton (jax.clayton)
/// 
/// 
///
#include "Component.h"
#include "rapidjson.h"

using namespace rapidjson;

template<typename ColliderType>
class Collider : public Component
{
	public:
		Collider() : Component(typeid(ColliderType)) {}
		~Collider();
		virtual Component* Clone() const override;
		virtual void Update();
		virtual bool isColliding(ColliderType* collider);
		void SetCollider(ColliderType* collider);
		ColliderType* GetCollider();
};
