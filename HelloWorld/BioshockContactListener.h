#pragma once

#include "Box2D/Box2D.h"
#include "entt/entity/registry.hpp"

class BioshockContactListener : public b2ContactListener
{
public:
	BioshockContactListener() { };
	BioshockContactListener(entt::registry* reg);

	void BeginContact(b2Contact* contact) override;

private:
	entt::registry* m_register = nullptr;
};
